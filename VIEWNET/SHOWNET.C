/******************************************************************************
** (C) Chris Wood 1995.
**
** SHOWNET.C - Scans and displays the network.
**
******************************************************************************* 
*/

#include <stdio.h>
#include <nwcalls.h>
#include <nwgfx.h>
#include "helpids.h"
#include "nodes.h"
#include "icons.h"

#define NODE_WIDTH  (ICON_WIDTH+1)      /* Width of node. */
#define NETFRMWIDTH 72                  /* Width of window. */

/**** Global Vars. ***********************************************************/
static NWGFRAME     NetFrame;      /* The network frame. */
static int          iFirstNode;    /* First node visible. */
static int          iLastNode;     /* Last node visible. */
static int          iVisible;      /* Number of visible nodes. */
static NWINET_ADDR  ViewedNet;     /* Network to view. */

extern int          iNumNodes;     /* Nodes in network. */
extern NODELIST     Network;       /* The current network. */

/**** Function Prototypes. ***************************************************/
void DrawNetworkFrame(NWGBOOL bActive);
void DrawNetwork(void);
void ShowNetworkInfo(void);
void ShowNodeInfo(PNETNODE pNode);
void DrawIcon(NWGSTR *pIcon, NWGPOS iX, NWGPOS iY);
void DrawIconHighlight(int iIcon, NWGBOOL bHighlight);

extern void ScanNetwork(NWINET_ADDR *pNetwork);
extern void GetLocalNetwork(NWINET_ADDR *pLocalAddr);
extern void WSDialog(PNETNODE pNode);
extern void BridgeMenu(PNETNODE pNode);

/******************************************************************************
** Scan the network and display it.
*/
void DoScan(void)
{
     NWGBOOL   bDone=FALSE;        /* Finished viewing. */
     NWGKEY    wKey;               /* The last key pressed. */
     int       iCurrent;           /* Currently highlighted node. */
     PNETNODE  pCurrent;           /* Currently highlighted node. */
     
     /* Set up network frame. */
     NetFrame.iPosType = FP_ABSOLUTE;
     NetFrame.fFlags   = FF_DOUBLE;
     NetFrame.pTitle   = NULL;
     NetFrame.iSX      = 3;
     NetFrame.iSY      = 4;
     NetFrame.iDX      = 76;
     NetFrame.iDY      = 23;
     
     /* Create network frame. */
     NWGFXCreateWindow(&NetFrame);

     /* Draw it. */
     DrawNetworkFrame(TRUE);
     
     /* Show wait message. */
     NWGFXWaitMessage(TRUE);

     /* Get local network and scan. */
     GetLocalNetwork(&ViewedNet);
     ScanNetwork(&ViewedNet);
     
     /* Hide wait message. */
     NWGFXWaitMessage(FALSE);

     /* Check we found some! */
     if (!iNumNodes)
     {
          NWGFXErrorMessage("No Network Nodes Found!");
          return;
     }
     
     /* Set to first node. */
     iCurrent = 0;
     pCurrent = Network;

     /* Calculate visible nodes. */
     iVisible = NETFRMWIDTH / NODE_WIDTH;
     if (iVisible > iNumNodes)
          iVisible = iNumNodes;
          
     iFirstNode = 0;
     iLastNode = iVisible - 1;

     /* Draw it. */
     ShowNetworkInfo();
     DrawNetwork();
     ShowNodeInfo(pCurrent);
     
     /* Highlight first node. */
     DrawIconHighlight(iCurrent, TRUE);
          
     /* Process keyboard input. */
     while(!bDone)
     {
          /* Wait for keypress. */
          wKey = NWGFXWaitForKeyPress();
          
          /* Decode keycode. */
          switch(wKey)
          {
               /* Cursor left. */
               case KEY_LEFT:
                    /* Check for start. */
                    if (iCurrent > iFirstNode)
                    {
                         /* Retreat highlight. */
                         DrawIconHighlight(iCurrent, FALSE);
                         iCurrent--;
                         pCurrent = pCurrent->Prev;
                         DrawIconHighlight(iCurrent, TRUE);
                         ShowNodeInfo(pCurrent);
                    }
                    else if ( (iCurrent == iFirstNode) && (iFirstNode != 0) )
                    {
                         /* Retreat. */
                         iCurrent--;
                         iFirstNode--;
                         iLastNode--;
                         pCurrent = pCurrent->Prev;
                         
                         /* Redisplay. */
                         NWGFXClearRect(NetFrame.iSX+1, NetFrame.iSY+1, 
                                        NetFrame.iDX-1, NetFrame.iSY+ICON_HEIGHT+2);
                         DrawNetwork();
                         DrawIconHighlight(iCurrent, TRUE);
                         ShowNodeInfo(pCurrent);
                    }
                    break;
               
               /* Cursor right. */
               case KEY_RIGHT:
                    /* Check for start. */
                    if (iCurrent < iLastNode)
                    {
                         /* Advance highlight. */
                         DrawIconHighlight(iCurrent, FALSE);
                         iCurrent++;
                         pCurrent = pCurrent->Next;
                         DrawIconHighlight(iCurrent, TRUE);
                         ShowNodeInfo(pCurrent);
                    }
                    else if ( (iCurrent == iLastNode) && (iLastNode != (iNumNodes-1)) )
                    {
                         /* Advance. */
                         iCurrent++;
                         iFirstNode++;
                         iLastNode++;
                         pCurrent = pCurrent->Next;
                         
                         /* Scroll right. */
                         NWGFXClearRect(NetFrame.iSX+1, NetFrame.iSY+1, 
                                        NetFrame.iDX-1, NetFrame.iSY+ICON_HEIGHT+2);
                         DrawNetwork();
                         DrawIconHighlight(iCurrent, TRUE);
                         ShowNodeInfo(pCurrent);
                    }
                    break;
               
               /* Select node. */
               case KEY_RETURN:
                    /* What type of node? */
                    switch(pCurrent->iType)
                    {
                         case NODE_WORKSTATION:
                              /* Draw this window inactive. */
                              DrawNetworkFrame(FALSE);
                              ShowNetworkInfo();
                              DrawNetwork();
                              ShowNodeInfo(pCurrent);
                              
                              /* Show the info. */
                              WSDialog(pCurrent);

                              /* Draw this window active again. */
                              DrawNetworkFrame(TRUE);
                              ShowNetworkInfo();
                              DrawNetwork();
                              ShowNodeInfo(pCurrent);
                              DrawIconHighlight(iCurrent, TRUE);
                              break;
                              
                         case NODE_FILESERVERBRIDGE:
                         case NODE_BRIDGE:
                         case NODE_NONDEDBRIDGE:
                         case NODE_NONDEDSERVBRIDGE:
                              BridgeMenu(pCurrent);
                              break;
                              
                         case NODE_NONDEDFILESERVER:
                         case NODE_FILESERVER:
                         case NODE_UNKNOWN:
                         default:
                              /* Nothing to do. */
                              break;
                    }
                    break;

               /* Cancel view. */
               case KEY_ESCAPE:
                    bDone = TRUE;
                    break;
               
               /* Safe. */
               default:
                    break;
          }
     }
     
     /* Destroy window. */
     NWGFXDestroyWindow();
}

/******************************************************************************
** Draw network window frame.
*/
void DrawNetworkFrame(NWGBOOL bActive)
{
     NWGCOLOUR cText;         /* Text colour. */
     
     /* Draw the frame border. */
     NWGFXDrawWindow(bActive);
     
     /* Get colours. */
     if (bActive)
          cText = (NWGCOLOUR) NWGFXGetScreenMetric(COLOUR_ACTIVE);
     else
          cText = (NWGCOLOUR) NWGFXGetScreenMetric(COLOUR_INACTIVE);
     
     /* Draw window divider. */
     NWGFXDrawHorizLine(NetFrame.iSX+1, NetFrame.iDX-1, NetFrame.iSY+ICON_HEIGHT+4, D_HORZBAR, cText);
     NWGFXPlotChar(NetFrame.iSX, NetFrame.iSY+ICON_HEIGHT+4, D_SIDELEFT, cText);
     NWGFXPlotChar(NetFrame.iDX, NetFrame.iSY+ICON_HEIGHT+4, D_SIDERIGHT, cText);
     
     /* Draw bottom window divider. */
     NWGFXDrawVertLine(NetFrame.iSX+36, NetFrame.iSY+ICON_HEIGHT+5, NetFrame.iDY-1, D_VERTBAR, cText);
     NWGFXPlotChar(NetFrame.iSX+36, NetFrame.iSY+ICON_HEIGHT+4, D_TOPMID, cText);
     NWGFXPlotChar(NetFrame.iSX+36, NetFrame.iDY, D_BOTMID, cText);

     /* Draw bottom title bars. */
     NWGFXDrawHorizLine(NetFrame.iSX+1, NetFrame.iDX-1, NetFrame.iSY+ICON_HEIGHT+6, D_HORZBAR, cText);
     NWGFXPlotChar(NetFrame.iSX, NetFrame.iSY+ICON_HEIGHT+6, D_SIDELEFT, cText);
     NWGFXPlotChar(NetFrame.iDX, NetFrame.iSY+ICON_HEIGHT+6, D_SIDERIGHT, cText);
     NWGFXPlotChar(NetFrame.iSX+36, NetFrame.iSY+ICON_HEIGHT+6, D_CROSS, cText);
     
     /* Draw window titles. */
     NWGFXDrawString(NetFrame.iSX+2, NetFrame.iSY+ICON_HEIGHT+5, TEXT_LEFT, "Network Info", cText);
     NWGFXDrawString(NetFrame.iSX+38, NetFrame.iSY+ICON_HEIGHT+5, TEXT_LEFT, "Node Info", cText);
     
     /* Draw "Network" labels. */
     NWGFXDrawString(NetFrame.iSX+2, NetFrame.iSY+ICON_HEIGHT+8, TEXT_LEFT, "Address:", cText);
     NWGFXDrawString(NetFrame.iSX+2, NetFrame.iSY+ICON_HEIGHT+9, TEXT_LEFT, "Nodes:", cText);

     /* Draw "Node" labels. */
     NWGFXDrawString(NetFrame.iSX+38, NetFrame.iSY+ICON_HEIGHT+8, TEXT_LEFT, "Address:", cText);
     NWGFXDrawString(NetFrame.iSX+38, NetFrame.iSY+ICON_HEIGHT+9, TEXT_LEFT, "Components:", cText);
}

/******************************************************************************
** Draw network information.
*/
void ShowNetworkInfo(void)
{
     char szTmp[20];          /* Valus as string buffer. */

     /* Show network address. */
     sprintf(szTmp, "%.2X%.2X%.2X%.2X", (int) ViewedNet.networkAddr[0], 
               (int) ViewedNet.networkAddr[1], (int) ViewedNet.networkAddr[2], 
               (int) ViewedNet.networkAddr[3]);
     NWGFXDrawString(NetFrame.iSX+11, NetFrame.iSY+ICON_HEIGHT+8, TEXT_LEFT, szTmp, WHITE);
     
     /* Show number of nodes. */
     sprintf(szTmp, "%d", iNumNodes);
     NWGFXDrawString(NetFrame.iSX+11, NetFrame.iSY+ICON_HEIGHT+9, TEXT_LEFT, szTmp, WHITE);
}

/******************************************************************************
** Draw node information.
*/
void ShowNodeInfo(PNETNODE pNode)
{
     char      szTmp[20];          /* Valus as string buffer. */
     NWGPOS    iX, iY;                 /* Component position. */

     /* Show nodes' address. */
     sprintf(szTmp, "%.2X%.2X%.2X%.2X%.2X%.2X", (int) pNode->Address[0],
               (int) pNode->Address[1], (int) pNode->Address[2],
               (int) pNode->Address[3], (int) pNode->Address[4],
               (int) pNode->Address[5]);
     NWGFXDrawString(NetFrame.iSX+50, NetFrame.iSY+ICON_HEIGHT+8, TEXT_LEFT, szTmp, WHITE);

     /* Clear previous components. */
     iX = NetFrame.iSX+50;
     iY = NetFrame.iSY+ICON_HEIGHT+9;
     NWGFXClearRect(iX, iY,  NetFrame.iDX-1, NetFrame.iDY-1);
     
     /* Show nodes' components. */
     if (pNode->lFlags & COMPONENT_SHELL)
     {
          NWGFXDrawString(iX, iY, TEXT_LEFT, "Shell", WHITE);
          iY++;
     }
     if (pNode->lFlags & COMPONENT_FILESERVER)
     {
          NWGFXDrawString(iX, iY, TEXT_LEFT, "File Server", WHITE);
          iY++;
     }
     if (pNode->lFlags & COMPONENT_BRIDGE)
     {
          NWGFXDrawString(iX, iY, TEXT_LEFT, "Bridge", WHITE);
          iY++;
     }
     if (pNode->lFlags & COMPONENT_SHELLDRIVER)
     {
          NWGFXDrawString(iX, iY, TEXT_LEFT, "Shell Driver", WHITE);
          iY++;
     }
     if (pNode->lFlags & COMPONENT_BRIDGEDRIVER)
     {
          NWGFXDrawString(iX, iY, TEXT_LEFT, "Bridge Driver", WHITE);
          iY++;
     }
     if (pNode->lFlags & COMPONENT_IPXSPX)
     {
          NWGFXDrawString(iX, iY, TEXT_LEFT, "IPX/SPX", WHITE);
          iY++;
     }
     if (pNode->lFlags & COMPONENT_IPXSPX_NONDED)
     {
          NWGFXDrawString(iX, iY, TEXT_LEFT, "Non-dedicated IPX/SPX", WHITE);
          iY++;
     }
     if (pNode->lFlags & COMPONENT_IPXONLY)
     {
          NWGFXDrawString(iX, iY, TEXT_LEFT, "IPX only", WHITE);
          iY++;
     }
     if (pNode->lFlags & COMPONENT_VAPSHELL)
     {
          NWGFXDrawString(iX, iY, TEXT_LEFT, "VAP Shell", WHITE);
          iY++;
     }
}

/******************************************************************************
** Draw network nodes.
*/
void DrawNetwork(void)
{
     PNETNODE  pNode;              /* Pointer to node. */
     int       iCurNode, iLoop;    /* Currently displayed node. */
     NWGPOS    iX, iY;             /* Node drawing position. */
     
     /* Draw the frame left scrollbar nudge. */
     if (iFirstNode != 0)
          NWGFXPlotChar(NetFrame.iSX+1, NetFrame.iSY+ICON_HEIGHT+3, LEFTNUDGE, WHITE);
     else
          NWGFXPlotChar(NetFrame.iSX+1, NetFrame.iSY+ICON_HEIGHT+3, ' ', WHITE);
          
     /* Draw the frame right scrollbar nudge. */
     if (iLastNode != (iNumNodes-1))
          NWGFXPlotChar(NetFrame.iDX-1, NetFrame.iSY+ICON_HEIGHT+3, RIGHTNUDGE, WHITE);
     else
          NWGFXPlotChar(NetFrame.iDX-1, NetFrame.iSY+ICON_HEIGHT+3, ' ', WHITE);

     /* Find first node. */
     pNode = Network;
     for (iLoop=0; iLoop < iFirstNode; iLoop++)
          pNode = pNode->Next;

     /* Setup for draw. */
     iCurNode = iFirstNode;
     iX = NetFrame.iSX + 1;
     iY = NetFrame.iSY + 2;
     
     /* Draw the network nodes. */
     do
     {
          /* Draw the nodes' name. */
          NWGFXDrawString(iX+3, iY-1, TEXT_CENTRE, pNode->szName, WHITE);
          
          /* Draw the nodes icon. */
          DrawIcon(Icons[pNode->iType], iX, iY);     

          /* Draw the 'cable'. */
          if (iCurNode == 0)
          {
               /* Is first node? */
               NWGFXDrawHorizLine(iX+4, iX+7, iY+ICON_HEIGHT, D_HORZBAR, WHITE);
               NWGFXPlotChar(iX+3, iY+ICON_HEIGHT, D_BOTLEFT, WHITE);
          }
          else if (iCurNode == iNumNodes-1)
          {
               /* Is last node? */
               NWGFXDrawHorizLine(iX, iX+3, iY+ICON_HEIGHT, D_HORZBAR, WHITE);
               NWGFXPlotChar(iX+3, iY+ICON_HEIGHT, D_BOTRIGHT, WHITE);
          }
          else
          {
               /* Somewhere in the middle. */
               NWGFXDrawHorizLine(iX, iX+7, iY+ICON_HEIGHT, D_HORZBAR, WHITE);
               NWGFXPlotChar(iX+3, iY+ICON_HEIGHT, D_BOTMID, WHITE);
          } 

          /* Move on. */
          iX += NODE_WIDTH;
          pNode = pNode->Next;
          iCurNode++;
     }
     while ( (pNode != NONE) && (iCurNode <= iLastNode) );
}

/******************************************************************************
** Draw an icon.
*/
void DrawIcon(NWGSTR *pIcon, NWGPOS iX, NWGPOS iY)
{
     int  iLoop;    /* Line counter. */
     
     /* Draw each line. */
     for (iLoop = 0; iLoop < ICON_HEIGHT; iLoop++)
          NWGFXDrawString(iX, iY + iLoop, TEXT_LEFT, pIcon[iLoop], WHITE);
}

/******************************************************************************
** Highlight an Icon.
*/
void DrawIconHighlight(int iIcon, NWGBOOL bHighlight)
{
     int       iLoop;    /* Line counter. */
     NWGPOS    iX, iY;   /* Position. */

     /* Calculate square origin. */
     iX = (NODE_WIDTH * (iIcon - iFirstNode)) + NetFrame.iSX + 1;
     iY = NetFrame.iSY + 2;
     
     /* Highlight each line. */
     for (iLoop = 0; iLoop < ICON_HEIGHT; iLoop++)
          NWGFXHiliteField(iX, iX+NODE_WIDTH-1, iY + iLoop, FALSE, bHighlight);
}
