/******************************************************************************
** (C) Chris Wood 1995.
**
** IPXCODE.C - All the IPX specific code.
**
** This is based on the GetNetworkMap() routine in Ralph Davis' NetWare
** Programmers Guide.
******************************************************************************* 
*/

#include <memory.h>
#include <malloc.h>
#include <time.h>
#include <string.h>
#include <nwcalls.h>
#include <nwipxspx.h>
#include <nwdiag.h>
#include <nwgfx.h>
#include "ipxcode.h"
#include "nodes.h"

/**** Defintions. ************************************************************/

#define MAX_SEND_ECBS    1                   /* Max number of send ECBs. */
#define MAX_RECV_ECBS    20                  /* Max number of receive ECBs. */

#define DELAY_FUDGE      3L                  /* Extra time to wait for responses. */

/**** Global Vars. ***********************************************************/
ECB SendECB;                                 /* ECB for sending. */
ECB RecvECB[MAX_RECV_ECBS];                  /* ECBs for receiving. */

WORD wMySocket;                              /* Socket to receive on. */

NODECFG RecvBuffer[MAX_RECV_ECBS];           /* Array of receiving data buffers. */
NODECFG NewCfg;                              /* New node configuration. */

IPXHeader SendHeader;                        /* Header for sending. */
IPXHeader RecvHeader[MAX_RECV_ECBS];         /* Headers for receiving. */

EXCLNODES ExclNodes;                         /* List of nodes to exclude. */

unsigned long lPktDelay;                     /* Delay for all packets to be received. */

int       iNumNodes;                         /* Nodes in network. */
NODELIST  Network;                           /* The network nodes. */
PNETNODE  pTail;                             /* Network tail. */

/**** Function Prototypes. ***************************************************/
void FreeNetwork(void);
ECB * PacketReceived(void);
void AddNode(PNETNODE pNew);

extern char * FindNodeName(BYTE *pAddr);

/******************************************************************************
** Build the network list.
*/
void ScanNetwork(NWINET_ADDR *pNetwork)
{
     unsigned long  lStartTime;         /* Time we started waiting. */
     ECB            *pECB;              /* New packet. */
     PNETNODE       pNewNode;           /* New node. */
     IPXHeader      *pHeader;           /* Nodes header. */
     int            iLoop;              /* Component loop. */
     
     /* Setup the IPX routines. */
     ScanSetup(pNetwork);

     /* Post the broadcast packet. */
     IPXSendPacket(&SendECB);

     /* Wait till it's been sent. */
     while (SendECB.inUseFlag)
          Yield();

     /* Delete previous network. */
     FreeNetwork();
     
     /* Wait RTT for responses. */
     time(&lStartTime);
     while ( (time(NULL) - lStartTime) < lPktDelay )
     {
          /* Check for new packet. */
          pECB = PacketReceived();
          if (pECB)
          {
               /* Allocate memory for new node. */
               pNewNode = (PNETNODE) malloc(sizeof(NETNODE));
               pNewNode->lFlags = 0L;
               
               /* Get header & configuration data. */
               pHeader = (IPXHeader *) pECB->fragmentDescriptor[0].address;
               memcpy(&NewCfg.packet, pECB->fragmentDescriptor[1].address, 
                         pECB->fragmentDescriptor[1].size);
               
               /* Save nodes' address and network. */
               memcpy(pNewNode->Address, &(pHeader->source.node), 6);
               memcpy(pNewNode->Network, &(pNetwork->networkAddr), 4);
               memcpy(pNewNode->Socket, &NewCfg.packet.SPXDiagnosticSocket, 2);
           
               /* Save the number of components. */
               pNewNode->iNumComps = NewCfg.packet.numberOfComponents;
           
               /* Loop through configuration data. */
               for (iLoop=0; iLoop < (int) NewCfg.packet.numberOfComponents; iLoop++)
               {
                    /* Decode component type. */
                    switch(NewCfg.packet.componentStructure[iLoop])
                    {
                         case IPX_SPX_COMPONENT:
                              pNewNode->lFlags |= COMPONENT_IPXSPX;
                              break;

                         case BRIDGE_DRIVER_COMPONENT:
                              pNewNode->lFlags |= COMPONENT_BRIDGEDRIVER;
                              break;

                         case SHELL_DRIVER_COMPONENT:
                              pNewNode->lFlags |= COMPONENT_SHELLDRIVER;
                              break;

                         case SHELL_COMPONENT:
                              pNewNode->lFlags |= COMPONENT_SHELL;
                              break;

                         case VAP_SHELL_COMPONENT:
                              pNewNode->lFlags |= COMPONENT_VAPSHELL;
                              break;

                         case BRIDGE_COMPONENT:
                              pNewNode->lFlags |= COMPONENT_BRIDGE;
                              break;

                         case FILE_SERVER_COMPONENT:
                              pNewNode->lFlags |= COMPONENT_FILESERVER;
                              break;

                         case NONDEDICATED_IPX_SPX_COMPONENT:
                              pNewNode->lFlags |= COMPONENT_IPXSPX_NONDED;
                              break;

                         case IPX_ONLY:
                              pNewNode->lFlags |= COMPONENT_IPXONLY;
                              break;
                         
                         /* Safe. */
                         default:
                              break;
                    }
               }
               
               /* Work out node type. */
               if (pNewNode->lFlags & COMPONENT_SHELL)
               {
                    /* Is probably a workstation. */
                    pNewNode->iType = NODE_WORKSTATION;
               }
               else if ( (pNewNode->lFlags & COMPONENT_IPXSPX)
                      && (pNewNode->lFlags & COMPONENT_FILESERVER) 
                      && !(pNewNode->lFlags & COMPONENT_BRIDGEDRIVER) )
               {
                    /* Is probably a dedicated file server. */
                    pNewNode->iType = NODE_FILESERVER;
               }
               else if ( (pNewNode->lFlags & COMPONENT_IPXSPX)
                      && (pNewNode->lFlags & COMPONENT_FILESERVER) 
                      && (pNewNode->lFlags & COMPONENT_BRIDGEDRIVER) )
               {
                    /* Is probably a dedicated file server & bridge. */
                    pNewNode->iType = NODE_FILESERVERBRIDGE;
               }
               else if ( (pNewNode->lFlags & COMPONENT_IPXSPX_NONDED)
                      && (pNewNode->lFlags & COMPONENT_FILESERVER) 
                      && !(pNewNode->lFlags & COMPONENT_BRIDGEDRIVER) )
               {
                    /* Is probably a non-dedicated file server. */
                    pNewNode->iType = NODE_NONDEDFILESERVER;
               }
               else if (pNewNode->lFlags & COMPONENT_BRIDGE)
               {
                    /* Is probably an external bridge. */
                    pNewNode->iType = NODE_BRIDGE;
               }
               else if ( (pNewNode->lFlags & COMPONENT_IPXSPX_NONDED)
                      && !(pNewNode->lFlags & COMPONENT_FILESERVER) 
                      && (pNewNode->lFlags & COMPONENT_BRIDGEDRIVER) )
               {
                    /* Is probably a non-dedicated bridge. */
                    pNewNode->iType = NODE_NONDEDBRIDGE;
               }
               else if ( (pNewNode->lFlags & COMPONENT_IPXSPX_NONDED)
                      && (pNewNode->lFlags & COMPONENT_FILESERVER) 
                      && (pNewNode->lFlags & COMPONENT_BRIDGEDRIVER) )
               {
                    /* Is probably a non-dedicated file server & bridge. */
                    pNewNode->iType = NODE_NONDEDSERVBRIDGE;
               }
               else
               {
                    /* Is something else. */
                    pNewNode->iType = NODE_UNKNOWN;
               }
               
               /* Get the nodes' name. */
               strcpy(pNewNode->szName, FindNodeName(pNewNode->Address));
               
               /* Add to list. */
               AddNode(pNewNode);
               
               /* Repost. */
               IPXListenForPacket(pECB);
          }
          
          Yield();
     }

     /* End the scan. */
     ScanTerm();
}

/******************************************************************************
** Setup the ECBs for scanning a network.
*/
NWGBOOL ScanSetup(NWINET_ADDR *pNetwork)
{
     int            iLoop;         /* ECB counter. */
     NWINET_ADDR    MyAddr;        /* My network address. */
     int            iSendTime;     /* Time to send a packet. */
     
     /* Get a dynamic socket to receive data on. */
     if (IPXOpenSocket((BYTE NWFAR*) &wMySocket, SHORT_LIVED) != SUCCESSFUL)
          return FALSE;
          
     /* Get our address. */
     GetLocalNetwork(&MyAddr);
     
     /* 
     ** Set up the send ECB.
     */
     
     /* The source. */
     memcpy(&SendHeader.source, &MyAddr, 10);
     memcpy(&SendHeader.source.socket, &wMySocket, 2);

     /* The destination. (Its a broadcast so use 0xFFFFFFFFFFFF on socket 0x0456). */
     memcpy(&SendHeader.destination.network, &(pNetwork->networkAddr), 4);
     memset(&SendHeader.destination.node, 0xFF, 6);
     SendHeader.destination.socket[0] = 0x04;
     SendHeader.destination.socket[1] = 0x56;

     /* Clear ECB. */
     memset(&SendECB, 0, sizeof(ECB));

     /* Set socket number and fragment count. */
     memcpy(&SendECB.socketNumber, &wMySocket, 2);
     SendECB.fragmentCount = 2;
     
     /* Set up first fragment for IPX header. */
     SendECB.fragmentDescriptor[0].address = &SendHeader;
     SendECB.fragmentDescriptor[0].size    = sizeof(IPXHeader);
     
     /* Set up second fragment for data. */
     memset(&ExclNodes, 0, sizeof(EXCLNODES));
     SendECB.fragmentDescriptor[1].address = &ExclNodes;
     SendECB.fragmentDescriptor[1].size    = sizeof(EXCLNODES);
     
     /* 
     ** Set up the receive ECBs and post. 
     */
     for (iLoop=0; iLoop < MAX_RECV_ECBS; iLoop++)
     {
          /* Clear ECB. */
          memset(&RecvECB[iLoop], 0, sizeof(ECB));
          
          /* Set socket number and fragment count. */
          memcpy(&RecvECB[iLoop].socketNumber, &wMySocket, 2);
          RecvECB[iLoop].fragmentCount = 2;
          
          /* Set up first fragment for IPX header. */
          RecvECB[iLoop].fragmentDescriptor[0].address = &RecvHeader[iLoop];
          RecvECB[iLoop].fragmentDescriptor[0].size    = sizeof(IPXHeader);
          
          /* Set up second fragment for data. */
          RecvECB[iLoop].fragmentDescriptor[1].address = &RecvBuffer[iLoop].packet;
          RecvECB[iLoop].fragmentDescriptor[1].size    = sizeof(RecvBuffer[iLoop].packet);
     
          /* Post ECB for receiving. */
          IPXListenForPacket(&RecvECB[iLoop]);
     }
          
     /* 
     ** Calculate time to receive all responses. 
     ** The send time is in ~1/20ths of a second.
     */
     IPXGetLocalTarget((BYTE FAR*) &SendHeader.destination, 
                       (BYTE FAR*) &SendECB.immediateAddress, &iSendTime);
     lPktDelay = ((unsigned long) iSendTime / 20L ) + DELAY_FUDGE;
}

/******************************************************************************
** Terminate the scan.
*/
void ScanTerm(void)
{
     IPXCloseSocket(wMySocket);
}

/******************************************************************************
** Build the network list.
*/
ECB * PacketReceived(void)
{
     int iLoop;          /* ECB counter. */
     
     /* Find a received ECB. */
     for (iLoop=0; iLoop < MAX_RECV_ECBS; iLoop++)
     {
          /* Check flags. */
          if ( (!RecvECB[iLoop].inUseFlag) && (RecvECB[iLoop].completionCode == SUCCESSFUL) )
          {
               /* New packet. */
               return &RecvECB[iLoop];
          }
          else if (!RecvECB[iLoop].inUseFlag)
          {
               /* Received but not successful. */
               IPXListenForPacket(&RecvECB[iLoop]);
          }
          
          Yield();
     }
     
     /* None received. */
     return (ECB*) NULL;
}

/******************************************************************************
** Get the address of the network we are joined to.
*/
void GetLocalNetwork(NWINET_ADDR *pLocalAddr)
{
     NWCONN_HANDLE  hConn;         /* Preferred connection handle. */
     NWCONN_NUM     ConnID;        /* Our connection ID. */

     /* Get our address. */
     NWGetPrimaryConnectionID(&hConn);
     NWGetConnectionNumber(hConn, &ConnID);
     NWGetInetAddr(hConn, ConnID, pLocalAddr);
}

/******************************************************************************
** Add a new node to the network list.
*/
void AddNode(PNETNODE pNew)
{
     /* Check for null list. */
     if (Network == NULL)
     {
          /* Set as root. */
          pNew->Next = NONE;
          pNew->Prev = NONE;
          pTail      = pNew;
          Network    = pNew;
     }
     else /* List not empty. */
     {
          /* Tag on end. */
          pNew->Next  = NONE;
          pNew->Prev  = pTail;
          pTail->Next = pNew;
          pTail = pNew;
     }
     
     /* Update count. */
     iNumNodes++;
}

/******************************************************************************
** Free up the memory from the previous network.
*/
void FreeNetwork(void)
{
     PNETNODE  pTmp, pPrev;     /* List scanning node. */

     /* Check for previous. */
     if (Network == NONE)
          return;
     
     /* Run along the list. */
     pTmp = Network;
     while(pTmp)
     {
          /* Free entry. */
          pPrev = pTmp;
          pTmp  = pTmp->Next;
          free(pPrev);
     }
     
     /* Reset. */
     iNumNodes = 0;
     Network = NONE;
     pTail = NONE;
}

/******************************************************************************
** Yield to another task.
*/
void Yield(void)
{
     IPXRelinquishControl();
}
