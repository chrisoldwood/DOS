/******************************************************************************
** (C) Chris Wood 1994/95.
**
** NWGMBOX.C - Message boxes handler.
**
******************************************************************************* 
*/

#include <string.h>
#include <malloc.h>
#include "nwgfx.h"

#ifdef DEBUG
#include <stdio.h>
#include <stdlib.h>
#endif

/**** Global Variables ********************************************************/
extern NWGDIM    iScnWidth;          /* The screen width in chars. */
extern NWGDIM    iScnHeight;         /* The screen height in chars. */
extern NWGTYPE   iScnType;           /* The screen type, colour or mono. */
extern NWGDIM    iTitleHt;           /* The title bar height. */    

/******************************************************************************
** Display the message in a box using the colours specified. The dimensions of
** the box are calculated from the text given.
** The text is copied to another buffer because we have to change the CR's to
** null string terminators.
*/
VOID NWGFXMessageBox(NWGSTR pText, NWGCOLOUR cTextColour, NWGCOLOUR cBGColour)
{
     NWGFARBUF lpMsgBuf;           /* Screen save buffer. */
     NWGPOS    iSX, iSY;           /* Window origin. */
     NWGPOS    iDX, iDY;           /* Window lower right corner. */
     NWGDIM    iWidth, iHeight;    /* Window width & height. */
     NWGSTR    pCopy;              /* Text copy. */
     NWGSTR    pTmp1;              /* Temporary line pointer. */
     NWGSTR    pTmp2;              /* Temporary character pointers. */
     NWGNUM    iMaxStrLen;         /* Maximum size of given string. */
     NWGNUM    iCurStrLen;         /* Size of current string. */
     NWGNUM    iNumLines;          /* Number of lines of text. */
     NWGNUM    iLoop;              /* Line output counter. */

#ifdef DEBUG
     /* Check we have a valid string address. */
     if (!pText)
     {
          NWGFXSetCursorPos(0, 0);
          printf("NWGFXMessageBox(): Null string address.\n");
          return;
     }
#endif

     /* Get string length and copy to temporay buffer. */
     pCopy = malloc(strlen(pText)+1);
     strcpy(pCopy, pText);
     
     /* Setup default text metrics. */
     pTmp1 = pTmp2 = pCopy;
     iMaxStrLen = 23;
     iNumLines = 0;

     /* Run along the text. */
     while(*pTmp1)
     {
          /* Find the end of a line of text. */
          while( (*pTmp2 != '\r') && (*pTmp2 != '\0') )
               pTmp2++;
          iNumLines++;
          
          /* Check if largest to date.*/
          iCurStrLen = pTmp2 - pTmp1;
          if (iCurStrLen > iMaxStrLen)
               iMaxStrLen = iCurStrLen;
          
          /* Change string terminator to null char. */
          if (*pTmp2 == '\r')
          {
               *pTmp2 = '\0';
               pTmp2++;
          }
          
          /* Move to next line. */
          pTmp1 = pTmp2;
     }
     
     /* Calculate window origin. */
     iWidth = iMaxStrLen + 4;
     iHeight = iNumLines + 5;
     NWGFXCalcCentredFrame(iWidth, iHeight, &iSX, &iSY, &iDX, &iDY);
     
     /* Save the screen area under the message box. */
     lpMsgBuf = NWGFXSaveRect(iSX, iSY, iDX, iDY);

     /* Draw message box. */
     NWGFXDrawFrame(iSX, iSY, iDX, iDY, FALSE, FRAME_SINGLE, cTextColour, cBGColour);

     /* Set output to start of text. */
     pTmp1 = pCopy;
     
     /* Show message text. */
     for(iLoop=0; iLoop<iNumLines; iLoop++)
     {
          /* Output line. */
          NWGFXDrawString(iSX+2, iSY+iLoop+2, TEXT_LEFT, pTmp1, cTextColour);
     
          /* Move to line end. */
          while(*pTmp1)
               pTmp1++;
          
          /* Skip terminator. */
          pTmp1++;
     }
     NWGFXDrawString(iSX+2, iDY-1, TEXT_LEFT, (NWGSTR) "Press [ESC] to continue", cTextColour);

     /* Free text copy memory. */
     free(pCopy);
     
     /* Disable the auto-exit. */
     NWGFXAllowExit(FALSE);

     /* Wait for escape key. */
     while(NWGFXWaitForKeyPress() != KEY_ESCAPE);
     
     /* Re-enable the auto-exit. */
     NWGFXAllowExit(TRUE);

     /* Replace the screen area under the message box. */
     NWGFXRestoreRect(iSX, iSY, iDX, iDY, lpMsgBuf);
}

/******************************************************************************
** Draw a message box with the string given. The colours used are for a help
** related message.
*/
VOID NWGFXHelpMessage(NWGSTR pMessage)
{
#ifdef DEBUG
     /* Check we have a valid string address. */
     if (!pMessage)
     {
          NWGFXSetCursorPos(0, 0);
          printf("NWGFXHelpMessage(): Null string address.\n");
          return;
     }
#endif

     if (iScnType == MONO_SCREEN)
          NWGFXMessageBox(pMessage, DARK, NORMAL);
     else /* iScnType == COLOUR_SCREEN */
          NWGFXMessageBox(pMessage, BLACK, GREEN);
}

/******************************************************************************
** Draw a message box with the string given. The colours used are for any 
** information messages.
*/
VOID NWGFXInfoMessage(NWGSTR pMessage)
{
#ifdef DEBUG
     /* Check we have a valid string address. */
     if (!pMessage)
     {
          NWGFXSetCursorPos(0, 0);
          printf("NWGFXInfoMessage(): Null string address.\n");
          return;
     }
#endif

     if (iScnType == MONO_SCREEN)
          NWGFXMessageBox(pMessage, DARK, NORMAL);
     else /* iScnType == COLOUR_SCREEN */
          NWGFXMessageBox(pMessage, WHITE, MAGENTA);
}

/******************************************************************************
** Draw a message box with the string given. The colours used are for a message
** concerned with internal errors.
*/
VOID NWGFXErrorMessage(NWGSTR pMessage)
{
#ifdef DEBUG
     /* Check we have a valid string address. */
     if (!pMessage)
     {
          NWGFXSetCursorPos(0, 0);
          printf("NWGFXErrorMessage(): Null string address.\n");
          return;
     }
#endif

     if (iScnType == MONO_SCREEN)
          NWGFXMessageBox(pMessage, DARK, NORMAL);
     else /* iScnType == COLOUR_SCREEN */
          NWGFXMessageBox(pMessage, YELLOW, RED);
}

/******************************************************************************
** Draw a message informing the user to wait. This must be called twice, once
** to show the message and once to clear it. 
*/
VOID NWGFXWaitMessage(NWGBOOL bShow)
{
static NWGFARBUF    lpWaitBuf;          /* Screen save buffer. */

     NWGPOS    iSX, iSY;                /* Window origin. */
     NWGPOS    iDX, iDY;                /* Window lower right corner. */
     NWGCOLOUR cTextColour;             /* The text colour. */
     NWGCOLOUR cBGColour;               /* The background colour. */
     NWGCOLOUR cFlashColour;            /* Bg colour for flashing text. */

#ifdef DEBUG
     /* Check address to validate call. */
     if (bShow && lpWaitBuf)
     {
          NWGFXSetCursorPos(0, 0);
          printf("NWGFXWaitMessage(): Wait message already displayed.\n");
          return;
     }
     else if (!bShow && !lpWaitBuf)
     {
          NWGFXSetCursorPos(0, 0);
          printf("NWGFXWaitMessage(): Wait message not displayed.\n");
          return;
     }
#endif

     /* Calculate window position. */
     NWGFXCalcCentredFrame(15, 5, &iSX, &iSY, &iDX, &iDY);

     /* Check to see if we are hiding or showing. */
     if (!bShow)
     {
          /* Replace the screen area under the message box. */
          NWGFXRestoreRect(iSX, iSY, iDX, iDY, lpWaitBuf);

          /* Clear pointer. */
          lpWaitBuf = (NWGFARBUF) NULL;
     }
     else /* bShow == TRUE */
     {
          /* Save the screen area under the message box. */
          lpWaitBuf = NWGFXSaveRect(iSX, iSY, iDX, iDY);

          /* Get message box colours. */
          if (iScnType == MONO_SCREEN)
          {         
               cTextColour  = BRIGHT;
               cBGColour    = DARK;
               cFlashColour = DARK;
          }
          else /* iScnType == COLOUR_SCREEN */
          {
               cTextColour  = YELLOW;
               cBGColour    = BLUE;
               cFlashColour = LTBLUE;
          }
          
          /* Draw the frame. */
          NWGFXDrawFrame(iSX, iSY, iDX, iDY, FALSE, FRAME_SINGLE, cTextColour, cBGColour);
          NWGFXFillRect(iSX+1, iSY+1, iDX-1, iDY-1, ' ', cTextColour, cFlashColour);
          
          /* Write the message. */
          NWGFXDrawString(iSX+2, iSY+2, TEXT_LEFT, (NWGSTR) "Please Wait", cTextColour);
     }
}
