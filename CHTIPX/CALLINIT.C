/******************************************************************************
** (C) Chris Wood 1995.
**
** CALLINIT.C - Initialise a new call, or accept one.
**
******************************************************************************* 
*/

#include <string.h>
#include <nwcalls.h>
#include <nwgfx.h>
#include <time.h>
#include "ipxcode.h"

/**** Global Vars. ***********************************************************/
char RecBuf[80];                        /* Recieved data buffer. */

/**** Function Prototypes. ***************************************************/
NWGBOOL GetUserConnection(char* pUser, NWCONN_NUM * pConn);
void SendChatRequest(NWCONN_NUM Conn);

extern void Talk(char *UserName);

/******************************************************************************
** Initiate a new connection to the user specified.
*/
void MakeCall(char *UserName)
{
     NWGFRAME  MsgFrame;           /* The message frame. */
     NWGBOOL   bDone=FALSE;        /* Finished waiting. */
     NWGBOOL   bCancel=TRUE;       /* Users' cancelled. */
     NWGPOS    iTextX, iTextY;     /* Text position. */
     NWGKEY    wKey;               /* The last key pressed. */
     NWCONN_NUM wConn;             /* The recipients connection. */
     time_t    CurrTime;           /* The current time. */
     time_t    MsgTime;            /* Next time for message. */
     
     /* Set up message frame. */
     MsgFrame.iPosType = FP_ABSOLUTE;
     MsgFrame.fFlags   = FF_BASIC;

     /* Calculate frame position. */
     NWGFXCalcCentredFrame(31, 7, &MsgFrame.iSX, &MsgFrame.iSY,
                                  &MsgFrame.iDX, &MsgFrame.iDY);
     
     /* Calculate text position. */
     iTextX = MsgFrame.iSX + 3;
     iTextY = MsgFrame.iSY + 2;

     /* Get the users' connection number. */
     if (!GetUserConnection(UserName, &wConn))
     {
          /* Notify user. */
          NWGFXInfoMessage("User is not logged in.");
          return;
     }

     /* Setup IPX connection. */
     if (!IPXSetup(wConn))
     {
          /* Notify user. */
          NWGFXInfoMessage("Could not make connection.");
          return;
     }

     /* Create window frame. */
     NWGFXCreateWindow(&MsgFrame);
     
     /* Show status. */
     NWGFXDrawString(iTextX, iTextY, TEXT_LEFT,   "Waiting For Connection...", WHITE);
     NWGFXDrawString(iTextX, iTextY+2, TEXT_LEFT, "     [ESC] To Cancel", WHITE);
     
     /* Get current time. */
     time(&CurrTime);
     MsgTime = CurrTime;
     
     /* Wait for connection. */
     while(!bDone)
     {
          /* Get time. */
          time(&CurrTime);
          
          /* Check against message time. */
          if (CurrTime >= MsgTime)
          {
               /* Send a message to the user indicating the request. */
               SendChatRequest(wConn);
               
               /* Update next message time. */
               MsgTime = CurrTime + 20;
          }
          
          /* Check for keypress. */
          if (NWGFXIsKeyPressed())
          {
               /* Get key pressed. */
               wKey = NWGFXGetKeyPressed();
          
               /* Decode keycode. */
               switch(wKey)
               {
                    /* Cancel connection. */
                    case KEY_ESCAPE:
                         bCancel = TRUE;
                         bDone = TRUE;
                         break;
                    
                    /* Safe. */
                    default:
                         break;
               }
          }
          
          /* Check for data. */
          if (ReceivedData())
          {
               /* Get data. */
               ReceiveData(RecBuf);
               
               /* Check for connection. */
               if (strcmp(RecBuf, "CONNECT") == 0)
               {
                    /* Send acknlowedge. */
                    SendData("ACK");
                    
                    /* Ready to go. */
                    bDone = TRUE;
                    bCancel = FALSE;
               }
          }
          
          /* Yield. */
          Yield();
     }
     
     /* Destroy window. */
     NWGFXDestroyWindow();

     /* Check for cancel. */
     if (!bCancel)
          Talk(UserName);

     /* Destroy connection. */
     IPXShutDown();
}

/******************************************************************************
** Get the connection number of the recipient. Returns FALSE if not logged in.
*/
NWGBOOL GetUserConnection(char* pUser, NWCONN_NUM * pConn)
{
     NWCONN_HANDLE  hServer;            /* Default server. */
     NWNUMBER       wConns;             /* The number of connections. */
     NWCONN_NUM     ConnList[20];       /* The list of connections. */

     /* Get the default server. */
     NWGetPrimaryConnectionID(&hServer);
     
     /* Get a list of connections for the user. */
     NWGetObjectConnectionNumbers(hServer, (char NWFAR*) pUser, OT_USER,
          (NWNUMBER NWFAR*) &wConns, (NWCONN_NUM NWFAR*) ConnList, 20);

     /* Check they are logged in. */
     if (!wConns)
          return FALSE;

     /* Get the first connection. */
     *pConn = ConnList[0];
     
     /* User logged in. */
     return TRUE;
}

/******************************************************************************
** Send a message to the user to chat. It does not check that the user got the
** message.
*/
void SendChatRequest(NWCONN_NUM Conn)
{
     NWCONN_HANDLE  hServer;            /* Default server. */
     NWCONN_NUM     wUs;                /* Our connection number. */
     NWFLAGS        ResList;            /* The results. */
     char           UserName[50];       /* Our user name. */
     char           FullMsg[100];       /* The message + user name. */

     /* Get the default server. */
     NWGetPrimaryConnectionID(&hServer);
     
     /* Get our connection number. */
     NWGetConnectionNumber(hServer, &wUs);
     
     /* Get our user name.*/
     NWGetConnectionInformation(hServer, wUs, (char NWFAR*) UserName, NULL, NULL, NULL);
     
     /* Add on the message. */
     strcpy(FullMsg, "Chat request from: ");
     strcat(FullMsg, UserName);
     
     /* Send the message to the station they are connected to. */
     NWSendBroadcastMessage(hServer, (char NWFAR*) FullMsg, 1, 
          (NWCONN_NUM NWFAR*) &Conn, (NWFLAGS NWFAR*) &ResList);

     return;
}

/******************************************************************************
** Complete a connection from another user.
*/
void AcceptCall(char *UserName)
{
     NWGFRAME  MsgFrame;           /* The message frame. */
     NWGBOOL   bDone=FALSE;        /* Finished waiting. */
     NWGBOOL   bCancel=TRUE;       /* Users' cancelled. */
     NWGPOS    iTextX, iTextY;     /* Text position. */
     NWGKEY    wKey;               /* The last key pressed. */
     NWCONN_NUM wConn;             /* The recipients connection. */
     
     /* Set up message frame. */
     MsgFrame.iPosType = FP_ABSOLUTE;
     MsgFrame.fFlags   = FF_BASIC;

     /* Calculate frame position. */
     NWGFXCalcCentredFrame(31, 7, &MsgFrame.iSX, &MsgFrame.iSY,
                                  &MsgFrame.iDX, &MsgFrame.iDY);
     
     /* Calculate text position. */
     iTextX = MsgFrame.iSX + 3;
     iTextY = MsgFrame.iSY + 2;

     /* Get the users' connection number. */
     if (!GetUserConnection(UserName, &wConn))
     {
          /* Notify user. */
          NWGFXInfoMessage("User is not logged in.");
          return;
     }

     /* Setup IPX connection. */
     if (!IPXSetup(wConn))
     {
          /* Notify user. */
          NWGFXInfoMessage("Could not make connection.");
          return;
     }

     /* Send connect acknowledge. */
     SendData("CONNECT");

     /* Create window frame. */
     NWGFXCreateWindow(&MsgFrame);
     
     /* Show status. */
     NWGFXDrawString(iTextX, iTextY, TEXT_LEFT,   "Establishing Connection...", WHITE);
     NWGFXDrawString(iTextX, iTextY+2, TEXT_LEFT, "     [ESC] To Cancel", WHITE);
     
     /* Wait for connection. */
     while(!bDone)
     {
          /* Check for keypress. */
          if (NWGFXIsKeyPressed())
          {
               /* Get key pressed. */
               wKey = NWGFXGetKeyPressed();
          
               /* Decode keycode. */
               switch(wKey)
               {
                    /* Check for answer. */
                    case KEY_TIMEOUT:
                         break;
                    
                    /* Cancel connection. */
                    case KEY_ESCAPE:
                         bCancel = TRUE;
                         bDone = TRUE;
                         break;
                    
                    /* Safe. */
                    default:
                         break;
               }
          }
          
          /* Check for data. */
          if (ReceivedData())
          {
               /* Get data. */
               ReceiveData(RecBuf);
               
               /* Check for connection. */
               if (strcmp(RecBuf, "ACK") == 0)
               {
                    /* Ready to go. */
                    bDone = TRUE;
                    bCancel = FALSE;
               }
          }
          
          /* Yield. */
          Yield();
     }
     
     /* Destroy window. */
     NWGFXDestroyWindow();

     /* Check for cancel. */
     if (!bCancel)
          Talk(UserName);

     /* Destroy connection. */
     IPXShutDown();
}
