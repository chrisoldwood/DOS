/******************************************************************************
** (C) Chris Wood 1994/95.
**
** NWGINIT.C - Screen and application initialisation and cleanup code.
**
******************************************************************************* 
*/

#include <dos.h>
#include <string.h>
#include "nwgfx.h"
#include "nwgpriv.h"

/**** Portability. ***********************************************************/
#ifndef MK_FP
/* Not defined by Microsoft C7. */
#define MK_FP(seg, offset)    (void FAR *)(((unsigned long)seg << 16) + \
                              (unsigned long)(unsigned)offset)
#endif

/**** Global Variables. ******************************************************/
NWGVIDPTR   lpVideoData;           /* Pointer to BIOS video data. */
NWGSCNPTR   lpScnOrigin;           /* The screen buffer address. */
NWGDIM      iScnWidth;             /* The screen width in chars. */
NWGDIM      iScnHeight;            /* The screen height in chars. */
NWGTYPE     iScnType;              /* The screen type, colour or mono. */
NWGNUM      iScnPage;              /* Active screen page. */
NWGPOS      iOldCurX, iOldCurY;    /* The old cursor position. */
NWGFARBUF   lpOldScreen;           /* The old screen conents. */    
NWGDIM      iTitleHt;              /* The title bar height. */

extern NWGHELP      *hAppHelp;     /* Pointer to help text. */
extern NWGCHAR      szAppName[20]; /* The application name. */
extern NWGABORTPROC fnExit;        /* The exit callback function. */

/******************************************************************************
** Initialise the screen type and dimensions global variables. First find out
** what screen mode we're using, determine the width, height and screen address,
** and save the cursor position. Check if the screen mode is supported and exit
** if not. Finally save the screen contents for restoration later and draw the
** startup background.
** NB: This must be called before any other library drawing routine.
*/
NWCCODE NWGFXInitScreen(VOID)
{
     NWGNUM    iVideoMode;         /* The current video mode. */
     NWGWORD   wScnOffset;         /* The screen address offset. */

     /* Setup pointer to video data. */
     lpVideoData = (NWGVIDPTR) MK_FP(0x0040,0x0049);
     
     /* Get basic data. */
     iVideoMode = lpVideoData->uMode;
     iScnWidth  = lpVideoData->wNumColumns;
     iScnHeight = lpVideoData->uNumRows + 1;
     wScnOffset = lpVideoData->wBufferOffset;
     iScnPage   = lpVideoData->uActivePage;

     /* Save current cursor pos. */
     iOldCurX   = lpVideoData->CursorPos[iScnPage].uX;
     iOldCurY   = lpVideoData->CursorPos[iScnPage].uY;
     
     /* Determine screen type. */
     if (lpVideoData->wCRTAddress == (NWGWORD) 0x03B4)
          iScnType = MONO_SCREEN;
     else
          iScnType = COLOUR_SCREEN;
          
     /* Decode video mode, and calculate screen addreess. */
     if (iVideoMode < 4)
     {
          /* Colour:- Basic screen address is at 0xB800. */
          lpScnOrigin = (NWGSCNPTR) MK_FP(0xB800,wScnOffset);
     }
     else if (iVideoMode == 7)
     {
          /* Probably Mono:- Basic screen address is at 0xB000. */
          lpScnOrigin = (NWGSCNPTR) MK_FP(0xB000,wScnOffset);
     }
     else
     {
          /* We only support TEXT video modes up yo 7. */
          return (NWCCODE) NWGERR_VIDEO;
     }

     /* Save the current screen contents.*/
     lpOldScreen = NWGFXSaveRect(0, 0, iScnWidth-1, iScnHeight-1);
     if (!lpOldScreen)
          return (NWCCODE) NWGERR_ALLOC;

     /* Hide the cursor. */
     NWGFXHideCursor();

     /* Fill the background with the init colour. */
     if (iScnType == MONO_SCREEN)
          NWGFXFillRect(0, 0, iScnWidth-1, iScnHeight-1, ' ', BRIGHT, DARK);
     else /* iScnType == COLOUR_SCREEN */
          NWGFXFillRect(0, 0, iScnWidth-1, iScnHeight-1, ' ', CYAN, BLUE);
     
     /* Everything went as planned. */
     return (NWCCODE) NWGERR_NONE;
}

/******************************************************************************
** Restore the screen to it's original state. This involves restoring both the
** screen contents and the cursor position. The memory used to hold the old
** screen contents is also free'd up.
*/
VOID NWGFXCleanUp(VOID)
{
     /* Restore the screen contents. */
     NWGFXRestoreRect(0, 0, iScnWidth-1, iScnHeight-1, lpOldScreen);

     /* Restore cursor position. */
     NWGFXSetCursorPos(iOldCurX, iOldCurY);
}

/******************************************************************************
** Draw a string in the colours used during application startup.
*/
VOID NWGFXDrawInitString(NWGPOS iX, NWGPOS iY, NWGTYPE iStyle, NWGSTR pString)
{
     NWGCOLOUR cColour;       /* The text colour. */

     /* Determine the text colour from the screen type. */
     if (iScnType == MONO_SCREEN)
          cColour = BRIGHT;
     else /* iScnType == COLOUR_SCREEN */
          cColour = LTCYAN;

     /* Use the standard string routine. */
     NWGFXDrawString(iX, iY, iStyle, pString, cColour);
}

/******************************************************************************
** Setup various application pointers and draw the main application title bar
** and background.
*/
VOID NWGFXInitApplication(NWGDIM iHeight, NWGSTR pAppName, NWGABORTPROC fnAbort,
                              NWGHELP* hHelp)
{
     /* Draw title bar. */
     if (iScnType == MONO_SCREEN)
          NWGFXDrawFrame(0, 0, iScnWidth-1, iHeight-1, FALSE, FRAME_SINGLE, DARK, NORMAL);
     else /* iScnType == COLOUR_SCREEN */
          NWGFXDrawFrame(0, 0, iScnWidth-1, iHeight-1, FALSE, FRAME_SINGLE, BLUE, CYAN);

     /* Record title bar height. */
     iTitleHt = iHeight;

     /* Draw background. */
     if (iScnType == MONO_SCREEN)
          NWGFXFillRect(0, iTitleHt, iScnWidth-1, iScnHeight-1, SPARSEBLOCK, NORMAL, DARK);
     else /* iScnType == COLOUR_SCREEN */
          NWGFXFillRect(0, iTitleHt, iScnWidth-1, iScnHeight-1, SPARSEBLOCK, CYAN, BLUE);

     /* Save application name. */
     strcpy(szAppName, pAppName);
     
     /* Save abort function. */
     fnExit = fnAbort;
     
     /* Save pointer to application help text. */
     hAppHelp = hHelp;
}

/******************************************************************************
** Draw a string in the colours used for the application title bar.
*/
VOID NWGFXDrawTitleString(NWGPOS iX, NWGPOS iY, NWGTYPE iStyle, NWGSTR pString)
{
     NWGCOLOUR cColour;       /* The text colour. */

     /* Determine the text colour from the screen type. */
     if (iScnType == MONO_SCREEN)
          cColour = DARK;
     else /* iScnType == COLOUR_SCREEN */
          cColour = BLUE;

     /* Use the standard string routine. */
     NWGFXDrawString(iX, iY, iStyle, pString, cColour);
}
