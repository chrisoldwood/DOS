/******************************************************************************
** (C) Chris Wood 1995.
**
** IPXCODE.C - All the IPX specific code.
**
** NB: This code is heavily based on IPXCHAT1.C, the ipxchat sample program
** supplied with the SDK volume3.
******************************************************************************* 
*/

#include <memory.h>
#include <nwcalls.h>
#include <nwipxspx.h>
#include <nwgfx.h>

#define MAX_MSG_LEN 80                       /* Max. length of message. */
#define SOCKET_ID   0x534B                   /* Socket ID number. */

/**** Global Vars. ***********************************************************/
char SendBuffer[MAX_MSG_LEN];                /* Send buffer. */
char RecvBuffer[MAX_MSG_LEN];                /* Recieve buffer. */
WORD Socket;                                 /* Socket for comms. */
NWINET_ADDR DstAddr;                         /* Destination address. */
struct ECB SendECB;                          /* The ECB for sending. */
struct ECB RecvECB;                          /* The ECB for receiving. */
struct IPXHeader SendHeader;                 /* The packet header for sending. */
struct IPXHeader RecvHeader;                 /* The packet header for recieving. */

/**** Function Prototypes. ***************************************************/

/******************************************************************************
** Setup IPX for a connection to the specified user. We only use 2 ECBs as the
** data flow will be very small between the stations.
*/
NWGBOOL IPXSetup(NWCONN_NUM wConn)
{
     int  iRetVal;                      /* IPX return code. */
     NWCONN_HANDLE  hServer;            /* Default server. */
     int  iSendTime;                    /* Est. time to send a packet. */
     
     /* Get socket number in high-lo format. */
     Socket = NWWordSwap(SOCKET_ID);
     
     /* Create a SHORT_LIVED socket. */
     iRetVal = IPXOpenSocket((BYTE NWFAR*) &Socket, SHORT_LIVED);
     if (iRetVal)
          return FALSE;
          
     /* Get the default server. */
     NWGetPrimaryConnectionID(&hServer);
     
     /* Get the dstination address. */
     iRetVal = NWGetInetAddr(hServer, wConn, &DstAddr);
     if (iRetVal)
          return FALSE;

     /* Set destination socket number. */
     DstAddr.socket = Socket;

     /* Setup the send header. */
     memcpy(&SendHeader.destination.network, &DstAddr.networkAddr, 4);
     memcpy(&SendHeader.destination.node,    &DstAddr.netNodeAddr, 6);
     memcpy(&SendHeader.destination.socket,  &DstAddr.socket,      2);
     
     /* Setup the send ECB. */
     SendECB.ESRAddress    = (void (FAR *) ()) NULL;
     SendECB.socketNumber  = Socket;
     SendECB.fragmentCount = 2;
     SendECB.fragmentDescriptor[0].address = &SendHeader;
     SendECB.fragmentDescriptor[0].size    = sizeof(SendHeader);
     SendECB.fragmentDescriptor[1].address = &SendBuffer;
     SendECB.fragmentDescriptor[1].size    = sizeof(SendBuffer);
     
     /* Get the ECBs immediate address. */
     iRetVal = IPXGetLocalTarget(SendHeader.destination.network, 
                    SendECB.immediateAddress, &iSendTime);
     if (iRetVal)
          return FALSE;
     
     /* Setup the receive ECB. */
     RecvECB.ESRAddress    = (void (FAR *) ()) NULL;
     RecvECB.socketNumber  = Socket;
     RecvECB.fragmentCount = 2;
     RecvECB.fragmentDescriptor[0].address = &RecvHeader;
     RecvECB.fragmentDescriptor[0].size    = sizeof(RecvHeader);
     RecvECB.fragmentDescriptor[1].address = &RecvBuffer;
     RecvECB.fragmentDescriptor[1].size    = sizeof(RecvBuffer);

     /* Start listening for packets. */
     IPXListenForPacket(&RecvECB);
     
     /* Setup okay. */
     return TRUE;
}

/******************************************************************************
** Shutdown IPX connection.
*/
void IPXShutDown(void)
{
     /* Disconnect and close the socket. */
     IPXDisconnectFromTarget((BYTE NWFAR*) &DstAddr);
     IPXCloseSocket(Socket);
}

/******************************************************************************
** Check whether a packet has been received.
*/
NWGBOOL ReceivedData(void)
{
     if (RecvECB.inUseFlag)
          return FALSE;
     else
          return TRUE;
}

/******************************************************************************
** Get the received data.
*/
void ReceiveData(char *pBuffer)
{
     /* Copy the data to the buffer. */
     memcpy(pBuffer, RecvBuffer, MAX_MSG_LEN);
     
     /* Listen again. */
     IPXListenForPacket(&RecvECB);
}

/******************************************************************************
** Check whether a packet has been sent.
*/
NWGBOOL SentPacket(void)
{
     if (SendECB.inUseFlag)
          return FALSE;
     else
          return TRUE;
}

/******************************************************************************
** Send some data.
*/
void SendData(char *pBuffer)
{
     /* Wait for previous data to go. */
     while (SendECB.inUseFlag)
          IPXRelinquishControl();
          
     /* Copy the data to the send buffer. */
     memcpy(SendBuffer, pBuffer, MAX_MSG_LEN);
     
     /* Send packet. */
     IPXSendPacket(&SendECB);
}

/******************************************************************************
** Yield to another task.
*/
void Yield(void)
{
     IPXRelinquishControl();
}
