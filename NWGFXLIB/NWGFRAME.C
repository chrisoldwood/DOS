/******************************************************************************
** (C) Chris Wood 1994/95.
**
** NWGFRAME.C - Code for drawing and calculating window frames.
**
******************************************************************************* 
*/

#include "nwgfx.h"

#ifdef DEBUG
#include <stdio.h>
#include <stdlib.h>
#endif

#define MAX_FRAMES  10             /* Maximum number of frames in stack - 1. */

/**** Global Variables ********************************************************/
NWGFRAMEPTR    Frames[MAX_FRAMES]; /* Stack of active frames. */
NWGNUM         iCurrFrame=1;       /* Currently viewed frame. */

extern NWGDIM     iScnWidth;       /* The screen width in chars. */
extern NWGDIM     iScnHeight;      /* The screen height in chars. */
extern NWGTYPE    iScnType;        /* The screen type, colour or mono. */
extern NWGDIM     iTitleHt;        /* The title bar height. */    

/******************************************************************************
** Draw a window frame from (sx,sy) to (dx,dy). The window may or may not
** require a title bar. The style determines if the border is to use single or
** double line characters. The foreground is the frame colour.
*/
VOID NWGFXDrawFrame(NWGPOS iSX, NWGPOS iSY, NWGPOS iDX, NWGPOS iDY, NWGBOOL bTitle, 
                         NWGTYPE wStyle, NWGCOLOUR cFGColour, NWGCOLOUR cBGColour)
{
     /* Draw window background. */
     NWGFXFillRect(iSX, iSY, iDX, iDY, ' ', BLACK, cBGColour);

     /* Check frame style. */
     if (wStyle == FRAME_SINGLE)
     {
          /* Draw edges. */
          NWGFXDrawHorizLine(iSX, iDX, iSY, S_HORZBAR, cFGColour);
          NWGFXDrawHorizLine(iSX, iDX, iDY, S_HORZBAR, cFGColour);
          NWGFXDrawVertLine(iSX, iSY, iDY, S_VERTBAR, cFGColour);
          NWGFXDrawVertLine(iDX, iSY, iDY, S_VERTBAR, cFGColour);

          /* Draw corners. */
          NWGFXPlotChar(iSX, iSY, S_TOPLEFT, cFGColour);
          NWGFXPlotChar(iDX, iSY, S_TOPRIGHT, cFGColour);
          NWGFXPlotChar(iDX, iDY, S_BOTRIGHT, cFGColour);
          NWGFXPlotChar(iSX, iDY, S_BOTLEFT, cFGColour);

          /* Check for title. */
          if (bTitle)
          {
               /* Draw title separator. */
               NWGFXDrawHorizLine(iSX, iDX, iSY+2, S_HORZBAR, cFGColour);
               NWGFXPlotChar(iSX, iSY+2, S_SIDELEFT, cFGColour);
               NWGFXPlotChar(iDX, iSY+2, S_SIDERIGHT, cFGColour);
          }
     }
     else /* uStyle == FRAME_DOUBLE */
     {
          /* Draw edges. */
          NWGFXDrawHorizLine(iSX, iDX, iSY, D_HORZBAR, cFGColour);
          NWGFXDrawHorizLine(iSX, iDX, iDY, D_HORZBAR, cFGColour);
          NWGFXDrawVertLine(iSX, iSY, iDY, D_VERTBAR, cFGColour);
          NWGFXDrawVertLine(iDX, iSY, iDY, D_VERTBAR, cFGColour);

          /* Draw corners. */
          NWGFXPlotChar(iSX, iSY, D_TOPLEFT, cFGColour);
          NWGFXPlotChar(iDX, iSY, D_TOPRIGHT, cFGColour);
          NWGFXPlotChar(iDX, iDY, D_BOTRIGHT, cFGColour);
          NWGFXPlotChar(iSX, iDY, D_BOTLEFT, cFGColour);

          /* Check for title. */
          if (bTitle)
          {
               /* Draw title separator. */
               NWGFXDrawHorizLine(iSX, iDX, iSY+2, D_HORZBAR, cFGColour);
               NWGFXPlotChar(iSX, iSY+2, D_SIDELEFT, cFGColour);
               NWGFXPlotChar(iDX, iSY+2, D_SIDERIGHT, cFGColour);
          }
     }
}

/******************************************************************************
** Calculate the corners of a frame of given width & height so that it will
** be centered on the screen.
*/
VOID NWGFXCalcCentredFrame(NWGDIM iWidth, NWGDIM iHeight, NWGPOS * pSX, 
                              NWGPOS * pSY, NWGPOS * pDX, NWGPOS * pDY)
{
#ifdef DEBUG
     if ( (!pSX) || (!pSY) || (!pDX) || (!pDY) )
     {
          NWGFXSetCursorPos(0, 0);
          printf("NWGFXCalcCentredFrame(): NULL position pointer.");
          return;
     }
#endif

     *pSX = (iScnWidth - iWidth) / 2;
     *pSY = ((iScnHeight - iTitleHt - iHeight ) / 2 ) + iTitleHt;
     *pDX = *pSX + (iWidth - 1);
     *pDY = *pSY + (iHeight - 1);
}

/******************************************************************************
** Calculate the corners of a frame in absolute co-ordinates given a set of
** co-ordinates in a "different" convention.
** This is only used internally.
*/
VOID NWGFXCalcAbsoluteFrame(NWGTYPE iType, NWGPOS iSX, NWGPOS iSY, NWGPOS iDX, 
                              NWGPOS iDY, NWGPOS * pSX, NWGPOS * pSY, 
                              NWGPOS * pDX, NWGPOS * pDY)
{
#ifdef DEBUG
     if ( (!pSX) || (!pSY) || (!pDX) || (!pDY) )
     {
          NWGFXSetCursorPos(0, 0);
          printf("NWGFXCalcAbsoluteFrame(): NULL position pointer.");
          return;
     }
#endif

     /* Calculate frame position in absolute co-ordinates. */
     switch(iType)
     {
          case FP_ABSOLUTE:
               /* Co-ordinates are (sx,sy):(dx,dy). */
               *pSX = iSX;
               *pSY = iSY;
               *pDX = iDX;
               *pDY = iDY;
               break;
               
          case FP_RELATIVE:
               /* Origin is (+-sx,+-sy); if -ve, relative to right/bottom. */
               if (iSX < 0)
                    *pSX = iScnWidth + iSX - 1;
               else
                    *pSX = iSX;
               
               if (iSY < 0)
                    *pSY = iScnHeight + iSY - 1;
               else
                    *pSY = iSY;

               /* (dx,dy) is (width,height). */
               *pDX = *pSX + iDX - 1;
               *pDY = *pSY + iDY - 1;
               break;

          case FP_CENTRED:
               /* (sx,sy) is (width,height). */
               NWGFXCalcCentredFrame(iSX, iSY, pSX, pSY, pDX, pDY);
               break;

          default:
               /* Ignore! */
               *pSX = 0;
               *pSY = 0;
               *pDX = 0;
               *pDY = 0;
               return;
     }
}

/******************************************************************************
** Create and draw a new window frame. The frame is added to the stack and
** drawn as active on the screen. If there is no room for the new frame it is
** silently ignored.
*/
VOID NWGFXCreateWindow(NWGFRAMEPTR pFrame)
{
     NWGPOS    iSX, iSY, iDX, iDY;      /* Frame position.*/
     NWGBOOL   bTitle;                  /* Frame has title. */
     NWGTYPE   iBorder;                 /* Frame border style. */
     NWGCOLOUR cBG;                     /* Background colour. */
     NWGCOLOUR cBorder;                 /* Border colour. */
     
#ifdef DEBUG
     if (!pFrame)
     {
          NWGFXSetCursorPos(0, 0);
          printf("NWGFXCreateFrame(): NULL frame pointer.");
          return;
     }
     else if (iCurrFrame == MAX_FRAMES)
     {
          NWGFXSetCursorPos(0, 0);
          printf("NWGFXCreateFrame(): No room on frame stack.");
          return;
     }
#endif

     /* Check for room on the stack. */
     if (iCurrFrame == MAX_FRAMES)
          return;
     else
          iCurrFrame++;

     /* Save frame pointer. */
     Frames[iCurrFrame] = pFrame;
     
     /* Convert frame position to absolute co-ordinates. */
     NWGFXCalcAbsoluteFrame(pFrame->iPosType, pFrame->iSX, pFrame->iSY, pFrame->iDX, 
                              pFrame->iDY, &iSX, &iSY, &iDX, &iDY);

     /* Save the screen area under the frame. */
     pFrame->lpFrameBuf = NWGFXSaveRect(iSX, iSY, iDX, iDY);

     /* Get the display colours. */
     if (iScnType == MONO_SCREEN)
     {
          cBG     = DARK;
          cBorder = BRIGHT;
     }
     else /* iScnType == COLOUR_SCREEN */
     {
          cBG     = BLUE;
          cBorder = YELLOW;
     }

     /* Check border style. */
     if (pFrame->fFlags & FF_DOUBLE)
          iBorder = FRAME_DOUBLE;
     else
          iBorder = FRAME_SINGLE;
          
     /* Check for title. */
     if (pFrame->fFlags & FF_TITLE)
          bTitle = TRUE;
     else
          bTitle = FALSE;

     /* Draw the frame. */
     NWGFXDrawFrame(iSX, iSY, iDX, iDY, bTitle, iBorder, cBorder, cBG);

     /* Draw title if required. */
     if (bTitle)
          NWGFXDrawString((iDX-iSX)/2+iSX+1, iSY+1, TEXT_CENTRE, pFrame->pTitle, cBorder);
}

/******************************************************************************
** Destroy the last window frame on the stack. This just restores the screen 
** and removes one from the stack counter.
*/
VOID NWGFXDestroyWindow(VOID)
{
     NWGPOS    iSX, iSY, iDX, iDY;      /* Frame position.*/
     NWGFRAMEPTR    pFrame;             /* Current frame. */
     
#ifdef DEBUG
     if (!iCurrFrame)
     {
          NWGFXSetCursorPos(0, 0);
          printf("NWGFXDestroyFrame(): No frame on stack.");
          return;
     }
#endif

     /* Check for frame on the stack. */
     if (!iCurrFrame)
          return;

     /* Get current frame pointer. */
     pFrame = Frames[iCurrFrame];

     /* Convert frame position to absolute co-ordinates. */
     NWGFXCalcAbsoluteFrame(pFrame->iPosType, pFrame->iSX, pFrame->iSY, pFrame->iDX, 
                              pFrame->iDY, &iSX, &iSY, &iDX, &iDY);

     /* Replace the screen area under the frame. */
     NWGFXRestoreRect(iSX, iSY, iDX, iDY, pFrame->lpFrameBuf);

     /* Decrement the stack pointer. */
     iCurrFrame--;
}

/******************************************************************************
** Re-draw the window frames border in a new state. The state can either be
** ACTIVE or INACTIVE.
*/
VOID NWGFXDrawWindow(NWGTYPE iState)
{
     NWGPOS    iSX, iSY, iDX, iDY;      /* Frame position.*/
     NWGBOOL   bTitle;                  /* Frame has title. */
     NWGTYPE   iBorder;                 /* Frame border style. */
     NWGCOLOUR cBG;                     /* Background colour. */
     NWGCOLOUR cBorder;                 /* Border colour. */
     NWGFRAMEPTR    pFrame;             /* Current frame. */

#ifdef DEBUG
     if (!iCurrFrame)
     {
          NWGFXSetCursorPos(0, 0);
          printf("NWGFXDrawFrame(): No frame to draw.");
          return;
     }
#endif

     /* Check for frame on the stack. */
     if (!iCurrFrame)
          return;

     /* Get current frame pointer. */
     pFrame = Frames[iCurrFrame];

     /* Convert frame position to absolute co-ordinates. */
     NWGFXCalcAbsoluteFrame(pFrame->iPosType, pFrame->iSX, pFrame->iSY, pFrame->iDX, 
                              pFrame->iDY, &iSX, &iSY, &iDX, &iDY);

     /* Get the display colours. */
     if (iScnType == MONO_SCREEN)
     {
          cBG = DARK;
          if (iState == ACTIVE)
               cBorder = BRIGHT;
          else
               cBorder = NORMAL;
     }
     else /* iScnType == COLOUR_SCREEN */
     {
          cBG = BLUE;
          if (iState == ACTIVE)
               cBorder = YELLOW;
          else
               cBorder = WHITE;
     }

     /* Check border style. */
     if (pFrame->fFlags & FF_DOUBLE)
          iBorder = FRAME_DOUBLE;
     else
          iBorder = FRAME_SINGLE;
          
     /* Check for title. */
     if (pFrame->fFlags & FF_TITLE)
          bTitle = TRUE;
     else
          bTitle = FALSE;

     /* Draw the frame. */
     NWGFXDrawFrame(iSX, iSY, iDX, iDY, bTitle, iBorder, cBorder, cBG);

     /* Draw title if required. */
     if (bTitle)
          NWGFXDrawString((iDX-iSX)/2+iSX+1, iSY+1, TEXT_CENTRE, pFrame->pTitle, cBorder);

}

/******************************************************************************
** Get a metric for the screen.
*/
NWGNUM NWGFXGetScreenMetric(NWGNUM iMetric)
{
     /* Decode desired metric. */
     switch(iMetric)
     {
          /* Width of screen in characters. */
          case SCREEN_WIDTH:
               return iScnWidth;
               
          /* Height of screen in characters. */
          case SCREEN_HEIGHT:
               return iScnHeight;
     
          /* Display type, mono or colour. */
          case SCREEN_TYPE:
               return iScnType;
               
          /* Window background colour. */
          case COLOUR_WINDOWBG:
               if (iScnType == MONO_SCREEN)
                    return (NWGNUM) DARK;
               else
                    return (NWGNUM) BLUE;

          /* Active window border colour. */
          case COLOUR_ACTIVE:
               if (iScnType == MONO_SCREEN)
                    return (NWGNUM) BRIGHT;
               else
                    return (NWGNUM) YELLOW;

          /* Inactive window border colour. */
          case COLOUR_INACTIVE:
               if (iScnType == MONO_SCREEN)
                    return (NWGNUM) NORMAL;
               else
                    return (NWGNUM) WHITE;

          /* Title bar background colour. */
          case COLOUR_TITLEBG:
               if (iScnType == MONO_SCREEN)
                    return (NWGNUM) NORMAL;
               else
                    return (NWGNUM) CYAN;

          /* Title bar text colour. */
          case COLOUR_TITLETEXT:
               if (iScnType == MONO_SCREEN)
                    return (NWGNUM) DARK;
               else
                    return (NWGNUM) BLUE;

          /* Error message window background colour. */
          case COLOUR_ERRORBG:
               if (iScnType == MONO_SCREEN)
                    return (NWGNUM) NORMAL;
               else
                    return (NWGNUM) RED;

          /* Error message window text colour. */
          case COLOUR_ERRORTEXT:
               if (iScnType == MONO_SCREEN)
                    return (NWGNUM) DARK;
               else
                    return (NWGNUM) YELLOW;

          /* Info window background colour. */
          case COLOUR_INFOBG:
               if (iScnType == MONO_SCREEN)
                    return (NWGNUM) NORMAL;
               else
                    return (NWGNUM) MAGENTA;

          /* Info window text colour. */
          case COLOUR_INFOTEXT:
               if (iScnType == MONO_SCREEN)
                    return (NWGNUM) DARK;
               else
                    return (NWGNUM) WHITE;

          /* Help window background colour. */
          case COLOUR_HELPBG:
               if (iScnType == MONO_SCREEN)
                    return (NWGNUM) NORMAL;
               else
                    return (NWGNUM) GREEN;

          /* Help window text colour. */
          case COLOUR_HELPTEXT:
               if (iScnType == MONO_SCREEN)
                    return (NWGNUM) DARK;
               else
                    return (NWGNUM) BLACK;

          /* App init bg colour. */
          case COLOUR_INITBG:
               if (iScnType == MONO_SCREEN)
                    return (NWGNUM) DARK;
               else
                    return (NWGNUM) BLUE;

          /* App init text colour. */
          case COLOUR_INITTEXT:
               if (iScnType == MONO_SCREEN)
                    return (NWGNUM) BRIGHT;
               else
                    return (NWGNUM) CYAN;

          /* Unknown metric. */
          default:
#ifdef DEBUG
               NWGFXSetCursorPos(0, 0);
               printf("NWGFXGetScreenMetric(): Unknown metric.");
#endif
               return 0;
     }
}
