/******************************************************************************
** (C) Chris Wood 1995.
**
** MSG.C - Functions to get/send a message.
**
******************************************************************************* 
*/

#include <string.h>
#include <nwcalls.h>
#include <nwgfx.h>
#include "helpids.h"

/******************************************************************************
** Get the users name by index.
*/
NWGBOOL GetMessage(char* pBuffer)
{
     NWGFRAME MsgFrame;            /* The message frame. */
     NWGBOOL  bAccept;             /* Message valid. */
     NWGBOOL  bDone=FALSE;         /* Finished. */
     NWGPOS   iSX, iSY;            /* Window edge. */
     int      iCharPos=0;          /* Cursor position. */
     NWGKEY   wKey;                /* Last key pressed. */
     int      iOldHelpID;          /* Old help section ID. */
     
     /* Setup frame. */
     MsgFrame.iPosType = FP_ABSOLUTE;
     MsgFrame.iSX = 14;
     MsgFrame.iSY = 10;
     MsgFrame.iDX = 14 + 50 + 1;
     MsgFrame.iDY = 10 + 4;
     MsgFrame.fFlags = FF_DOUBLE | FF_TITLE;
     MsgFrame.pTitle = "Message Text";
     
     /* Setup the help section. */
     iOldHelpID = NWGFXGetHelpSection();
     NWGFXSetHelpSection(IDH_MESSAGE);
     
     /* Create window frame. */
     NWGFXCreateWindow(&MsgFrame);
  
     /* Ensure cursor appears in white. */
     NWGFXDrawHorizLine(MsgFrame.iSX + 1, MsgFrame.iDX - 1, MsgFrame.iDY - 1, ' ', WHITE);
     
     /* Set cursor to window start. */
     iSX = MsgFrame.iSX + 1;
     iSY = MsgFrame.iSY + 3;
     NWGFXSetCursorPos(iSX, iSY);
     
     /* Reset message text. */
     pBuffer[0] = '\0';
     
     /* Get message text. */
     while(!bDone)
     {
          /* Get a key. */
          wKey = NWGFXWaitForKeyPress();
          
          /* Decode keycode. */
          switch(wKey)
          {
               /* Accept text. */
               case KEY_RETURN:
                    bAccept = TRUE;
                    bDone = TRUE;
                    break;
               
               /* Cancel message. */
               case KEY_ESCAPE:
                    /* Accept message. */
                    bAccept = FALSE;
                    bDone = TRUE;
                    break;
               
               /* Delete a character. */
               case KEY_BACKSPACE:
                    /* Check we are not at start. */
                    if (iCharPos)
                    {
                         /* Move back one char. */
                         iCharPos--;
                         NWGFXSetCursorPos(iSX + iCharPos, iSY);
                         
                         /* Delete current char. */
                         pBuffer[iCharPos] = '\0';
                         
                         /* Wipe char from window. */
                         NWGFXPlotChar(iSX + iCharPos, iSY, ' ', WHITE);
                    }
                    
               /* Safe. */
               default:
                    /* Check characters' range. */
                    if ( (wKey < 256) && (wKey > 31) )
                    {
                         /* Check we are not at end. */
                         if (iCharPos < 50)
                         {
                              /* Save character. */
                              pBuffer[iCharPos]   = (char) wKey;
                              pBuffer[iCharPos+1] = '\0';

                              /* Show char in window. */
                              NWGFXPlotChar(iSX + iCharPos, iSY, (char) wKey, WHITE);
                         
                              /* Move cursor on one char. */
                              iCharPos++;
                              
                              /* Hide if at end. */
                              if (iCharPos < 50)
                                   NWGFXSetCursorPos(iSX + iCharPos, iSY);
                              else
                                   NWGFXHideCursor();
                         }
                    }
                    break;
          }
     }
     
     /* Hide cursor. */
     NWGFXHideCursor();
     
     /* Destroy window. */
     NWGFXDestroyWindow();

     /* Restore the old help. */
     NWGFXSetHelpSection(iOldHelpID);
     
     return bAccept;
}

/******************************************************************************
** Send a message to a user.
*/
void SendMessage(NWCONN_HANDLE hServer, char* pUser, char* pMsg)
{
     NWNUMBER       wConns;             /* The number of connections. */
     NWCONN_NUM     wUs;                /* Our connection number. */
     NWCONN_NUM     ConnList[20];       /* The list of connections. */
     NWFLAGS        ResList[20];        /* The results. */
     char           UserName[50];       /* Our user name. */
     char           FullMsg[100];       /* The message + user name. */

     /* Get our connection number. */
     NWGetConnectionNumber(hServer, &wUs);
     
     /* Get our user name.*/
     NWGetConnectionInformation(hServer, wUs, (char NWFAR*) UserName, NULL, NULL, NULL);
     
     /* Add on the message. */
     strcpy(FullMsg, "Msg From ");
     strcat(FullMsg, UserName);
     strcat(FullMsg, ": ");
     strcat(FullMsg, pMsg);
     
     /* Get a list of connections for the user. */
     NWGetObjectConnectionNumbers(hServer, (char NWFAR*) pUser, OT_USER,
          (NWNUMBER NWFAR*) &wConns, (NWCONN_NUM NWFAR*) ConnList, 20);
     
     /* Send the message to all stations they are connected to. */
     NWSendBroadcastMessage(hServer, (char NWFAR*) FullMsg, wConns, 
          (NWCONN_NUM NWFAR*) ConnList, (NWFLAGS NWFAR*) ResList);
}
