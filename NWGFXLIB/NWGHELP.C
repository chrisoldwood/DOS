/******************************************************************************
** (C) Chris Wood 1994/95.
**
** NWGHELP.C - On-Line help handler.
**
******************************************************************************* 
*/

#include <stdio.h>
#include "nwgfx.h"

/**** Global Variables ********************************************************/
NWGNUM    iHelpLevel=0;            /* Current Help Level. */
NWGNUM    iHelpSection=0;          /* The current help section. */
NWGHELP   *hAppHelp;               /* Pointer to help text. */

extern NWGDIM    iScnWidth;        /* The screen width in chars. */
extern NWGDIM    iScnHeight;       /* The screen height in chars. */
extern NWGTYPE   iScnType;         /* The screen type, colour or mono. */
extern NWGDIM    iTitleHt;         /* The title bar height. */    
extern NWGBOOL   bAllowHelp;       /* Allow the help to be shown. */

/**** Internal Functions. ****************************************************/
VOID NWGFXCallUpHelp(VOID);
VOID NWGFXListKeys(VOID);
VOID NWGFXShowHelpSection(VOID);

/******************************************************************************
** Set the current help text section.
*/
VOID NWGFXSetHelpSection(NWGNUM iSection)
{
     iHelpSection = iSection;
}

/******************************************************************************
** Get the current help text section.
*/
NWGNUM NWGFXGetHelpSection(VOID)
{
     return iHelpSection;
}

/******************************************************************************
** Call up the help text handler. If the current help level is 0, then call up
** the current section of the applications help. If the applications help is 
** already being displayed, then show the key assignments. If no help text is
** given then display an error.
*/
VOID NWGFXCallUpHelp(VOID)
{
     /* Check the current help level. */
     if (iHelpLevel == 0)
     {
          /* Check to see if we have help for the application. */
          if (!hAppHelp)
          {
               /* Inc. help level. */
               iHelpLevel++;
               
               /* Show "No Help" warning. */
               NWGFXHelpMessage("No help is available\rPress F1 again for a list of keys");

               /* Dec. help level. */
               iHelpLevel--;
          }
          else
          {
               /* Inc. help level. */
               iHelpLevel++;

               /* Show application help. */
               NWGFXShowHelpSection();
               
               /* Dec. help level. */
               iHelpLevel--;
          }
     }
     else if (iHelpLevel == 1)
     {
          /* Inc. help level. */
          iHelpLevel++;

          /* Show keyboard assignments. */
          NWGFXListKeys();
          
          /* Dec. help level. */
          iHelpLevel--;
     }
     else /* iHelpLevel == 2 */
     {
          /* No more help. */
     }
}

/******************************************************************************
** Show a section of the application help text.
*/
VOID NWGFXShowHelpSection(VOID)
{
     NWGFARBUF lpHelpBuf;          /* Screen save buffer. */
     NWGCOLOUR cTextColour;        /* Colour of help text. */
     NWGCOLOUR cBGColour;          /* Colour of help background. */
     NWGNUM    iFirst, iLast;      /* First and last lines of help displayed. */
     NWGNUM    iVisible;           /* Height of visible 'window'. */
     NWGNUM    iCurPage;           /* Currently shown page. */
     NWGNUM    iMaxPage;           /* Max number of pages. */
     NWGNUM    iLoop;              /* Display counter. */
     NWGPOS    iSX, iSY;           /* Window origin. */
     NWGPOS    iDX, iDY;           /* Window lower right corner. */
     NWGKEY    wKey;               /* Last key pressed. */
     NWGCHAR   szPage[20];         /* Page number string. */
     NWGPOS    iOldX, iOldY;            /* Old cursor position. */

     /* Get old cursor position and hide. */
     NWGFXGetCursorPos(&iOldX, &iOldY);
     NWGFXHideCursor();

     /* Calculate window position. */
     NWGFXCalcCentredFrame(hAppHelp[iHelpSection].iWidth, hAppHelp[iHelpSection].iHeight,
                              &iSX, &iSY, &iDX, &iDY);
     
     /* Save the screen area under the help box. */
     lpHelpBuf = NWGFXSaveRect(iSX, iSY, iDX, iDY);

     /* Determine screen colours. */
     if (iScnType == MONO_SCREEN)
     {         
          cTextColour = DARK;
          cBGColour = NORMAL;
     }
     else /* iScnType == COLOUR_SCREEN */
     {
          cTextColour = BLACK;
          cBGColour = GREEN;
     }

     /* Draw help window. */
     NWGFXDrawFrame(iSX, iSY, iDX, iDY, TRUE, FRAME_DOUBLE, cTextColour, cBGColour);

     /* Init display 'window'. */
     iFirst = iCurPage = 0;
     iVisible = iDY - iSY - 3;
     iMaxPage = (hAppHelp[iHelpSection].iNumLines / iVisible);
     if ((hAppHelp[iHelpSection].iNumLines % iVisible) == 0)
          iMaxPage--;
     if (iMaxPage)
          iLast = iVisible - 1;
     else
          iLast = hAppHelp[iHelpSection].iNumLines - 1;

     /* Draw heading. */
     NWGFXDrawString(iSX+2, iSY+1, TEXT_LEFT, hAppHelp[iHelpSection].pTitle, cTextColour);
     sprintf(szPage, "Help %d of %d", iCurPage+1, iMaxPage+1);
     NWGFXDrawString(iDX-2, iSY+1, TEXT_RIGHT, szPage, cTextColour);
     
     /* Display the first page. */
     for(iLoop=0; iLoop <= iLast; iLoop++)
          NWGFXDrawString(iSX+2, iSY+iLoop+3, TEXT_LEFT, hAppHelp[iHelpSection].pText[iLoop].pString, cTextColour);
          
     /* Wait for escape key. */
     while((wKey = NWGFXWaitForKeyPress()) != KEY_ESCAPE)
     {
          if( (wKey == KEY_PAGEUP) && (iCurPage > 0) )
          {
               /* Update 'visible' window. */
               iFirst = iFirst - iVisible;
               iLast = iFirst + (iVisible-1);
               iCurPage--;

               /* Draw previous page. */
               NWGFXDrawFrame(iSX, iSY, iDX, iDY, TRUE, FRAME_DOUBLE, cTextColour, cBGColour);
               NWGFXDrawString(iSX+2, iSY+1, TEXT_LEFT, hAppHelp[iHelpSection].pTitle, cTextColour);
               sprintf(szPage, "Help %d of %d", iCurPage+1, iMaxPage+1);
               NWGFXDrawString(iDX-2, iSY+1, TEXT_RIGHT, szPage, cTextColour);
               for(iLoop=iFirst; iLoop <= iLast; iLoop++)
                    NWGFXDrawString(iSX+2, iSY+iLoop+3-iFirst, TEXT_LEFT, hAppHelp[iHelpSection].pText[iLoop].pString, cTextColour);
          }
          else if ( (wKey == KEY_PAGEDOWN) && (iCurPage < iMaxPage) )
          {
               /* Update 'visible' window. */
               iFirst = iLast + 1;
               iLast = iFirst + (iVisible-1);
               if (iLast > (hAppHelp[iHelpSection].iNumLines-1))
                    iLast = hAppHelp[iHelpSection].iNumLines - 1;
               iCurPage++;

               /* Draw succesive page. */
               NWGFXDrawFrame(iSX, iSY, iDX, iDY, TRUE, FRAME_DOUBLE, cTextColour, cBGColour);
               NWGFXDrawString(iSX+2, iSY+1, TEXT_LEFT, hAppHelp[iHelpSection].pTitle, cTextColour);
               sprintf(szPage, "Help %d of %d", iCurPage+1, iMaxPage+1);
               NWGFXDrawString(iDX-2, iSY+1, TEXT_RIGHT, szPage, cTextColour);
               for(iLoop=iFirst; iLoop <= iLast; iLoop++)
                    NWGFXDrawString(iSX+2, iSY+iLoop+3-iFirst, TEXT_LEFT, hAppHelp[iHelpSection].pText[iLoop].pString, cTextColour);
          }
     }
     
     /* Restore cursor position. */
     NWGFXSetCursorPos(iOldX, iOldY);

     /* Replace the screen area under the help box. */
     NWGFXRestoreRect(iSX, iSY, iDX, iDY, lpHelpBuf);
}

/* Number of strings in key-list array. */
#define KEYLISTSIZE 28

/* The key assignments, shown after two successive F1 keypresses. */
NWGSTR pKeyList[KEYLISTSIZE] = { 
"ESCAPE   Esc    Go back one level",
"EXIT     AltF10 Exit application",
"CANCEL   F7     Cancel operation",
"ACCEPT   Return Accept the info",
"",
"INSERT   Ins    Insert a new item",
"DELETE   Del    Delete an item",
"MODIFY   F3     Modify an item",
"MARK     F5     Toggle the item",
"SELECT   Return Select the item",
"",
"HELP     F1     Ask for help",
"CYCLE    TAB    Cycle through items",
"MODE     F9     Change modes",
"",
"UP       \030      Move up one line",
"DOWN     \031      Move down one line",
"LEFT     \033      Move left one place",
"RIGHT    \032      Move right one place",
"PAGEUP   PgUp   Move up one page",
"PAGEDOWN PgDn   Move down one page",
"",
"FIRST    ^PgUp  Move to beginning",
"LAST     ^PgDn  Move to end",
"START    Home   Move to line start",
"END      End    Move to line end",
"NEXT     ^\032     Move to next field",
"PREV     ^\033     Move to prev field",
};

/******************************************************************************
** List the keyboard assignments, until Escape is pressed. PageUp & PageDown
** flick through the list a page at a time.
*/
VOID NWGFXListKeys(VOID)
{
     NWGFARBUF lpHelpBuf;          /* Screen save buffer. */
     NWGCOLOUR cTextColour;        /* Colour of help text. */
     NWGCOLOUR cBGColour;          /* Colour of help background. */
     NWGNUM    iFirst, iLast;      /* First and last lines of help displayed. */
     NWGNUM    iVisible;           /* Height of visible 'window'. */
     NWGNUM    iCurPage;           /* Currently shown page. */
     NWGNUM    iMaxPage;           /* Max number of pages. */
     NWGNUM    iLoop;              /* Display counter. */
     NWGPOS    iSX, iSY;           /* Window origin. */
     NWGPOS    iDX, iDY;           /* Window lower right corner. */
     NWGKEY    wKey;               /* Last key pressed. */

     /* Calculate window position. */
     iSX = 0;
     iSY = iTitleHt;
     iDX = iScnWidth - 1;
     iDY = iScnHeight - 1;
     iVisible = iScnHeight - iTitleHt - 4; 
     
     /* Save the screen area under the window. */
     lpHelpBuf = NWGFXSaveRect(iSX, iSY, iDX, iDY);

     /* Determine screen colours. */
     if (iScnType == MONO_SCREEN)
     {         
          cTextColour = DARK;
          cBGColour = NORMAL;
     }
     else /* iScnType == COLOUR_SCREEN */
     {
          cTextColour = BLACK;
          cBGColour = GREEN;
     }

     /* Draw keylist box. */
     NWGFXDrawFrame(iSX, iSY, iDX, iDY, FALSE, FRAME_DOUBLE, cTextColour, cBGColour);

     /* Init display 'window'. */
     iFirst = iCurPage = 0;
     iMaxPage = (KEYLISTSIZE / iVisible);
     if ((KEYLISTSIZE % iVisible) == 0)
          iMaxPage--;
     if (iMaxPage)
          iLast = iVisible - 1;
     else
          iLast = KEYLISTSIZE - 1;

     /* Display first page. */
     NWGFXDrawString(iSX+2, iTitleHt+1, TEXT_LEFT, (NWGSTR) "The key assignments are:", cTextColour);
     for(iLoop=0; iLoop <= iLast; iLoop++)
          NWGFXDrawString(iSX+2, iTitleHt+iLoop+3, TEXT_LEFT, pKeyList[iLoop], cTextColour);
          
     /* Wait for escape key. */
     while((wKey = NWGFXWaitForKeyPress()) != KEY_ESCAPE)
     {
          if( (wKey == KEY_PAGEUP) && (iCurPage > 0) )
          {
               /* Update 'visible' window. */
               iFirst = iFirst - iVisible;
               iLast = iFirst + (iVisible-1);
               iCurPage--;

               /* Draw previous page. */
               NWGFXFillRect(iSX+1, iSY+2, iDX-1, iDY-1, ' ', cTextColour, cBGColour);
               for(iLoop=iFirst; iLoop <= iLast; iLoop++)
                    NWGFXDrawString(iSX+2, iTitleHt+iLoop-iFirst+3, TEXT_LEFT, pKeyList[iLoop], cTextColour);
          }
          else if ( (wKey == KEY_PAGEDOWN) && (iCurPage < iMaxPage) )
          {
               /* Update 'visible' window. */
               iFirst = iLast + 1;
               iLast = iFirst + (iVisible-1);
               if (iLast > (KEYLISTSIZE-1))
                    iLast = KEYLISTSIZE - 1;
               iCurPage++;

               /* Draw succesive page. */
               NWGFXFillRect(iSX+1, iSY+2, iDX-1, iDY-1, ' ', cTextColour, cBGColour);
               for(iLoop=iFirst; iLoop <= iLast; iLoop++)
                    NWGFXDrawString(iSX+2, iTitleHt+iLoop-iFirst+3, TEXT_LEFT, pKeyList[iLoop], cTextColour);
          }
     }
     
     /* Replace the screen area under the window. */
     NWGFXRestoreRect(iSX, iSY, iDX, iDY, lpHelpBuf);
}

/*******************************************************************************
** Allow or disallow help to be shown. This is used to disable the auto-help 
** function.
*/
VOID NWGFXAllowHelp(NWGBOOL bShowHelp)
{
     /* Set internal flag. */
     bAllowHelp = bShowHelp;
}
