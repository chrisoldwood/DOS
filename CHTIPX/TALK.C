/******************************************************************************
** (C) Chris Wood 1995.
**
** TALK.C - Do the talking.
**
******************************************************************************* 
*/

#include <string.h>
#include <nwcalls.h>
#include <nwgfx.h>
#include "ipxcode.h"

/**** Global Vars. ***********************************************************/
char Buffer[80];                        /* Typing buffer. */
char RecBuf[80];                        /* Recieved data buffer. */
NWGPOS iTopSY, iTopDY;                  /* Top window position. */
NWGPOS iBotSY, iBotDY;                  /* Bottom window position. */
NWGPOS iTopCurY;                        /* Top window cursor position. */
NWGPOS iBotCurY;                        /* Bottom window cursor position. */

extern NWGDIM  wDispWidth;              /* The display width in chars. */
extern NWGDIM  wDispHeight;             /* The display height in chars. */

/**** Function Prototypes. ***************************************************/
void DrawDivider(NWGPOS iY);
void DrawRecipientMsg(char *pBuffer);
void DrawSenderMsg(char *pBuffer);

/******************************************************************************
** Create the responses window and do some chatting.
*/
void Talk(char *UserName)
{
     NWGFRAME  TalkFrame;          /* The chatting window. */
     NWGBOOL   bDone=FALSE;        /* Finished talking. */
     NWGKEY    wKey;               /* Last key-pressed. */
     NWGDIM    iTopWndSize;        /* Recpients window size. */
     NWGDIM    iBotWndSize;        /* Senders window size. */
     int       iCharPos=0;         /* Buffer position. */
     NWGPOS    iCurX, iCurY;       /* Message cursor position. */
     
     /* Set up message frame. */
     TalkFrame.iPosType = FP_ABSOLUTE;
     TalkFrame.iSX      = 0;
     TalkFrame.iSY      = 3;
     TalkFrame.iDX      = wDispWidth - 1;
     TalkFrame.iDY      = wDispHeight - 1;
     TalkFrame.fFlags   = FF_DOUBLE;
     
     /* Create window frame. */
     NWGFXCreateWindow(&TalkFrame);
     
     /* Calculate the size of the windows. */
     iTopWndSize = (wDispHeight - 8) / 2;
     iBotWndSize = wDispHeight - 8 - iTopWndSize;
     
     /* Calculate their positions. */
     iTopSY = 4;
     iTopDY = iTopSY + iTopWndSize - 1;
     iBotSY = iTopWndSize + 5;
     iBotDY = iBotSY + iBotWndSize - 1;
     
     /* Draw the dividers. */
     DrawDivider(iTopDY + 1);
     DrawDivider(iBotDY + 1);
     
     /* Draw the window labels. */
     NWGFXDrawString(wDispWidth/2, 3, TEXT_CENTRE, UserName, WHITE);
     NWGFXDrawString(wDispWidth/2, iTopDY + 1, TEXT_CENTRE, "YOU", WHITE);
     NWGFXDrawString(wDispWidth/2, iBotDY + 1, TEXT_CENTRE, "MESSAGE", WHITE);
     
     /* Ensure cursor appears in white. */
     NWGFXDrawHorizLine(1, wDispWidth-2, wDispHeight-2, ' ', WHITE);

     /* Calculate the cursor position. */
     iCurX = 1;
     iCurY = wDispHeight - 2;
     NWGFXSetCursorPos(iCurX, iCurY);

     /* Reset message text. */
     Buffer[0] = '\0';
     
     /* Reset window cursor positions. */
     iTopCurY = iBotCurY = 0;
     
     /* Loop until cancelled. */
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
                    /* Send text. */
                    case KEY_RETURN:
                         /* Blank text line. */
                         NWGFXDrawHorizLine(1, wDispWidth-2, wDispHeight-2, ' ', WHITE);

                         /* Reset cursor position. */
                         iCurX = 1;
                         iCurY = wDispHeight - 2;
                         NWGFXSetCursorPos(iCurX, iCurY);

                         /* Echo locally. */
                         DrawSenderMsg(Buffer);
                         
                         /* Send. */
                         SendData(Buffer);
                         
                         /* Reset message text. */
                         iCharPos = 0;
                         Buffer[0] = '\0';
                         break;

                    /* Cancel connection. */
                    case KEY_ESCAPE:
                         /* Check. */
                         bDone = NWGFXQueryMenu("End Chat", TRUE);
                         
                         /* Send terminate packet. */
                         if (bDone)
                              SendData("ENDCHAT");
                         break;
               
                    /* Delete a character. */
                    case KEY_BACKSPACE:
                         /* Check we are not at start. */
                         if (iCharPos)
                         {
                              /* Move back one char. */
                              iCharPos--;
                              NWGFXSetCursorPos(iCurX + iCharPos, iCurY);
                         
                              /* Delete current char. */
                              Buffer[iCharPos] = '\0';
                         
                              /* Wipe char from window. */
                              NWGFXPlotChar(iCurX + iCharPos, iCurY, ' ', WHITE);
                         }
                         break;
                    
                    /* Safe. */
                    default:
                         /* Check characters' range. */
                         if ( (wKey < 256) && (wKey > 31) )
                         {
                              /* Check we are not at end. */
                              if (iCharPos < wDispWidth-2)
                              {
                                   /* Save character. */
                                   Buffer[iCharPos]   = (char) wKey;
                                   Buffer[iCharPos+1] = '\0';

                                   /* Show char in window. */
                                   NWGFXPlotChar(iCurX + iCharPos, iCurY, (char) wKey, WHITE);
                         
                                   /* Move cursor on one char. */
                                   iCharPos++;
                              
                                   /* Hide if at end. */
                                   if (iCharPos < 50)
                                        NWGFXSetCursorPos(iCurX + iCharPos, iCurY);
                                   else
                                        NWGFXHideCursor();
                              }
                         }
                         break;
               }
          }
          
          /* Check for recipients message. */
          if (ReceivedData())
          {
               /* Get data. */
               ReceiveData(RecBuf);
               
               /* Check for termination. */
               if (strcmp(RecBuf, "ENDCHAT") == 0)
               {
                    /* Notify user. */
                    NWGFXInfoMessage("Recipient Has Disconnected.");
                    bDone = TRUE;
               }
               else
               {
                    /* Display message. */
                    DrawRecipientMsg(RecBuf);
               }
          }
          
          /* Yield. */
          Yield();
     }
     
     /* Hide cursor. */
     NWGFXHideCursor();
     
     /* Destroy window. */
     NWGFXDestroyWindow();
}

/******************************************************************************
** Draw a window divider.
*/
void DrawDivider(NWGPOS iY)
{
     /* Draw the main divider. */
     NWGFXDrawHorizLine(1, wDispWidth-2, iY, D_HORZBAR, YELLOW);
     
     /* Draw the ends. */
     NWGFXPlotChar(0, iY, D_SIDELEFT, YELLOW);
     NWGFXPlotChar(wDispWidth-1, iY, D_SIDERIGHT, YELLOW);
}

/******************************************************************************
** Display new recipient message.
*/
void DrawRecipientMsg(char *pBuffer)
{
     /* Check for scroll. */
     if ((iTopSY + iTopCurY) > iTopDY)
     {
          /* Scroll window up one line. */
          NWGFXScrollRectUp(1, iTopSY, wDispWidth-2, iTopDY);
          
          /* Decrement cursor position. */
          iTopCurY--;
     }
      
     /* Display message. */
     NWGFXDrawString(1, iTopSY + iTopCurY, TEXT_LEFT, pBuffer, WHITE);

     /* Move down a line. */
     iTopCurY++;
}

/******************************************************************************
** Display new sender message.
*/
void DrawSenderMsg(char *pBuffer)
{
     /* Check for scroll. */
     if ((iBotSY + iBotCurY) > iBotDY)
     {
          /* Scroll window up one line. */
          NWGFXScrollRectUp(1, iBotSY, wDispWidth-2, iBotDY);

          /* Decrement cursor position. */
          iBotCurY--;
     }

     /* Display message. */
     NWGFXDrawString(1, iBotSY + iBotCurY, TEXT_LEFT, pBuffer, WHITE);

     /* Move down a line. */
     iBotCurY++;
}
