/******************************************************************************
** (C) Chris Wood 1994/95.
**
** NWGCORE.C - Low-level functions for accessing the screen buffer.
**
******************************************************************************* 
*/

#include <dos.h>
#include <malloc.h>
#include <string.h>
#include "nwgfx.h"
#include "nwgpriv.h"

#ifdef DEBUG
#include <stdio.h>
#include <stdlib.h>
#endif

/**** Portability. ***********************************************************/
#ifdef _MSC_VER
/* For Microsoft C. */
#define CPUREGS     _REGS
#define INT86       _int86
#else
/* For everyone else. */
#define CPUREGS     REGS
#define INT86       int86
#endif

/**** Global Variables. ******************************************************/
extern NWGVIDPTR lpVideoData;           /* Pointer to BIOS video data. */
extern NWGSCNPTR lpScnOrigin;           /* The screen buffer address. */
extern NWGDIM    iScnWidth;             /* The screen width in chars. */
extern NWGDIM    iScnHeight;            /* The screen height in chars. */
extern NWGTYPE   iScnType;              /* The screen type, colour or mono. */
extern NWGNUM    iScnPage;              /* Active screen page. */

/*******************************************************************************
** Set the position of the cursor to (x,y).
*/
VOID NWGFXSetCursorPos(NWGPOS iX, NWGPOS iY)
{
     union CPUREGS inregs, outregs;

#ifdef DEBUG
     /* Check we have a valid screen address. */
     if (!lpScnOrigin)
     {
          printf("NWGFXSetCursorPos(): Screen uninitialised.\n");
          exit(1);
     }
#endif

     /* Set up registers. */
     inregs.h.ah = 2;                        /* Video function 2. */
     inregs.h.bh = (NWGBYTE) iScnPage;       /* Active page. */
     inregs.h.dl = (NWGBYTE) iX;             /* Position column. */
     inregs.h.dh = (NWGBYTE) iY;             /* Position row. */
     
     /* Call BIOS SetCursorPos function. */
     INT86(0x10, &inregs, &outregs);
}

/*******************************************************************************
** Get the position of the cursor and return in (x,y).
*/
VOID NWGFXGetCursorPos(NWGPOS *iX, NWGPOS *iY)
{
#ifdef DEBUG
     /* Check we have a valid video data pointer. */
     if (!lpVideoData)
     {
          printf("NWGFXGetCursorPos(): Screen uninitialised.\n");
          exit(1);
     }

     /* Check for valid position pointers. */
     if ( (!iX) || (!iY) )
     {
          NWGFXSetCursorPos(0, 0);
          printf("NWGFXGetCursorPos(): NULL position pointer.");
          return;
     }
#endif

     *iX = lpVideoData->CursorPos[iScnPage].uX;
     *iY = lpVideoData->CursorPos[iScnPage].uY;
}

/*******************************************************************************
** Hide the cursor by positioning it off the screen.
*/
VOID NWGFXHideCursor(VOID)
{
     union CPUREGS inregs, outregs;

#ifdef DEBUG
     /* Check we have a valid screen address. */
     if (!lpScnOrigin)
     {
          printf("NWGFXHideCursor(): Screen uninitialised.\n");
          exit(1);
     }
#endif

     /* Set up registers. */
     inregs.h.ah = 2;                        /* Video function 2. */
     inregs.h.bh = (NWGBYTE) iScnPage;       /* Active page. */

#ifdef DEBUG
     /* 
     ** In a debug release we hide the cursor in the top left
     ** so that any other messages (e.g. Runtime errors) will
     ** still be visible.
     */
     inregs.h.dl = (NWGBYTE) 0;              /* Position column. */
     inregs.h.dh = (NWGBYTE) 0;              /* Position row. */
#else
     /* 
     ** In a non-debug release we don't expect any other erros (!)
     ** so the cursor is hidden properly off the bottom.
     */
     inregs.h.dl = (NWGBYTE) 0;              /* Position column. */
     inregs.h.dh = (NWGBYTE) iScnHeight;     /* Position row. */
#endif

     /* Call BIOS SetCursorPos function. */
     INT86(0x10, &inregs, &outregs);
}

/******************************************************************************
** Plot a character at the location (x,y) in the colour specified. The colour 
** only determines the forground colour of the character.
*/
VOID NWGFXPlotChar(NWGPOS iX, NWGPOS iY, NWGCHAR cChar, NWGCOLOUR cColour)
{
     NWGSCNPTR     lpScreen;      /* Pointer to screen. */

#ifdef DEBUG
     /* Check we have a valid screen address. */
     if (!lpScnOrigin)
     {
          printf("NWGFXPlotChar(): Screen uninitialised.\n");
          exit(1);
     }
     
     /* Validate character position. */
     if ( (iX < 0) || (iX >= iScnWidth) || (iY < 0) || (iY >= iScnHeight) )
     {
          NWGFXSetCursorPos(0, 0);
          printf("NWGFXPlotChar(): Attempt to plot outside of the screen.");
          return;
     }
#endif

     /* Calculate char position. */
     lpScreen = lpScnOrigin + ( (iScnWidth * 2 * iY) + (iX * 2) );

     /* Store character and attribute. */
     *lpScreen++ = cChar;
     *lpScreen   = (*lpScreen & 0xF0) | cColour;
}

/******************************************************************************
** Draw a horizontal line using one character from (sx,y) to (dx,y) in the 
** colour specified. The colour only determines the forground colour of the 
** characters. The line must be defined as left to right.
*/
VOID NWGFXDrawHorizLine(NWGPOS iSX, NWGPOS iDX, NWGPOS iY, NWGCHAR cChar, 
                         NWGCOLOUR cColour)
{
     NWGSCNPTR     lpScreen;      /* Pointer to screen. */
     NWGNUM        iLength;       /* Length of line in characters. */

#ifdef DEBUG
     /* Check we have a valid screen address. */
     if (!lpScnOrigin)
     {
          printf("NWGFXDrawHorizLine(): Screen uninitialised.\n");
          exit(1);
     }
     
     /* Validate line ends. */
     if ( (iSX < 0) || (iSX >= iScnWidth) || (iDX < 0) || (iDX >= iScnWidth) ||
          (iY < 0) || (iY >= iScnHeight) )
     {
          NWGFXSetCursorPos(0, 0);
          printf("NWGFXDrawHorizLine(): Attempt to draw outside of the screen.");
          return;
     }
#endif

     /* Calculate line start and length. */
     lpScreen = lpScnOrigin + ( (iScnWidth * 2 * iY) + (iSX * 2) );
     iLength = iDX - iSX + 1;

     /* Plot line. */
     while(iLength--)
     {
          *lpScreen++ = cChar;
          *lpScreen++ = (*lpScreen & 0xF0) | cColour;
     }
}

/******************************************************************************
** Draw a vertical line using one character from (x,sy) to (x,dy) in the colour
** specified. The colour only determines the forground colour of the characters.
*/
VOID NWGFXDrawVertLine(NWGPOS iX, NWGPOS iSY, NWGPOS iDY, NWGCHAR cChar, 
                         NWGCOLOUR cColour)
{
     NWGSCNPTR     lpScreen;       /* Pointer to screen. */
     NWGNUM         iLength;       /* Length of line in characters. */
     NWGNUM         iOffset;       /* Next raster line offset. */

#ifdef DEBUG
     /* Check we have a valid screen address. */
     if (!lpScnOrigin)
     {
          printf("NWGFXDrawVertLine(): Screen uninitialised.\n");
          exit(1);
     }
     
     /* Validate line ends. */
     if ( (iX < 0) || (iX >= iScnWidth) || (iSY < 0) || (iSY >= iScnHeight) ||
          (iDY < 0) || (iDY >= iScnHeight) )
     {
          NWGFXSetCursorPos(0, 0);
          printf("NWGFXDrawVertLine(): Attempt to draw outside of the screen.");
          return;
     }
#endif

     /* Calculate line start, length and raster line offset. */
     lpScreen = lpScnOrigin + ( (iScnWidth * 2 * iSY) + (iX * 2) );
     iLength = iDY - iSY + 1;
     iOffset = (iScnWidth * 2) - 1;

     /* Plot line. */
     while(iLength--)
     {
          *lpScreen++ = cChar;
          *lpScreen   = (*lpScreen & 0xF0) | cColour;
          lpScreen   += iOffset;
     }
}

/******************************************************************************
** Draw a string to the screen. The style determines how the string is to be
** displayed:- left justified, right justified or centred, with (x,y) defining
** the leftmost, rightmost or central string point. The colour only determines
** the foreground colour of the characters.
*/
VOID NWGFXDrawString(NWGPOS iX, NWGPOS iY, NWGTYPE iStyle, NWGSTR pString, 
                         NWGCOLOUR cColour)
{
     NWGSCNPTR      lpScreen;      /* Pointer to screen. */
     NWGNUM         iLength;       /* Length of string. */

#ifdef DEBUG
     /* Check we have a valid screen address. */
     if (!lpScnOrigin)
     {
          printf("NWGFXDrawString(): Screen uninitialised.\n");
          exit(1);
     }
     
     /* Check we have a valid string address. */
     if (!pString)
     {
          NWGFXSetCursorPos(0, 0);
          printf("NWGFXDrawString(): Null string address.\n");
          return;
     }
#endif

     /* Calculate string length, and ignore if 0. */
     iLength = strlen(pString);
     if (!iLength)
          return;

     /* Decode text style and adjust the x co-ordinate. */
     switch(iStyle)
     {
          case TEXT_LEFT:
               /* No need to adjust. */
               break;
          case TEXT_RIGHT:
               iX -= iLength - 1;
               break;
               
          case TEXT_CENTRE:
               iX -= iLength / 2;
               break;

          default:
               /* Ignore! */
               return;
     }
     
#ifdef DEBUG
     /* Validate string ends. */
     if ( (iX < 0) || ((iX + iLength - 1) >= iScnWidth) || (iY < 0) || (iY >= iScnHeight) )
     {
          NWGFXSetCursorPos(0, 0);
          printf("NWGFXDrawString(): Attempt to draw outside of the screen.");
          return;
     }
#endif

     /* Calculate line start, length and raster line offset. */
     lpScreen = lpScnOrigin + ( (iScnWidth * 2 * iY) + (iX * 2) );

     /* Plot string. */
     while(iLength--)
     {
          *lpScreen++ = *pString++;
          *lpScreen++ = (*lpScreen & 0xF0) | cColour;
     }
}

/******************************************************************************
** Save a screen rectangle defined by (sx,sy):(dx,dy) to a memory buffer. The
** memory buffer is allocated here and returned to the caller.
*/
NWGFARBUF NWGFXSaveRect(NWGPOS iSX, NWGPOS iSY, NWGPOS iDX, NWGPOS iDY)
{
     NWGSCNPTR      lpScreen;           /* Pointer to screen. */
     NWGFARBUF      lpBuffer, lpSave;   /* Pointer the memory save buffer. */
     NWGNUM         iWidth, iHeight;    /* Blit dimensions. */
     NWGNUM         iOffset;            /* Next raster line offset. */
     NWGNUM         iTmpWidth;          /* Temporary width counter. */

#ifdef DEBUG
     /* Check we have a valid screen address. */
     if (!lpScnOrigin)
     {
          printf("NWGFXSaveRect(): Screen uninitialised.\n");
          exit(1);
     }

     /* Validate rectangle corners. */
     if ( (iSX < 0) || (iSX >= iScnWidth)  || (iDX < 0) || (iDX >= iScnWidth) || 
          (iSY < 0) || (iSY >= iScnHeight) || (iDY < 0) || (iDY >= iScnHeight) )
     {
          NWGFXSetCursorPos(0, 0);
          printf("NWGFXSaveRect(): Attempt to blit an area outside of the screen.");
          return (NWGFARBUF) NULL;
     }
#endif
     
     /* Allocate far memory buffer. */
     lpBuffer = lpSave = _fmalloc( ( (iDX - iSX + 1) * (iDY - iSY + 1) ) * 2 );
     if (!lpBuffer)
          return (NWGFARBUF) NULL;
          
     /* Calculate blit origin, dimensions and next raster line offset. */
     lpScreen = lpScnOrigin + ( (iScnWidth * 2 * iSY) + (iSX * 2) );
     iWidth   = (iDX - iSX + 1) * 2;
     iHeight  = iDY - iSY + 1;
     iOffset  = (iScnWidth * 2) - iWidth;

     /* Perfom the blit. */
     while(iHeight--)
     {
          /* Fetch the line width in bytes. */
          iTmpWidth = iWidth;

          /* Blit a line. */
          while(iTmpWidth--)
               *lpBuffer++ = *lpScreen++;

          /* Move to next line. */
          lpScreen += iOffset;
     }
     
     /* Return the buffer address. */
     return lpSave;
}

/******************************************************************************
** Restore a screen rectangle defined by (sx,sy):(dx,dy) from a memory buffer.
** The memory buffer is free'd here.
*/
VOID NWGFXRestoreRect(NWGPOS iSX, NWGPOS iSY, NWGPOS iDX, NWGPOS iDY, NWGFARBUF lpBuffer)
{
     NWGSCNPTR      lpScreen;           /* Pointer to screen. */
     NWGNUM         iWidth, iHeight;    /* Blit dimensions. */
     NWGNUM         iOffset;            /* Next raster line offset. */
     NWGNUM         iTmpWidth;          /* Temporary width counter. */

#ifdef DEBUG
     /* Check we have a valid screen address. */
     if (!lpScnOrigin)
     {
          printf("NWGFXRestoreRect(): Screen uninitialised.\n");
          exit(1);
     }

     /* Validate rectangle corners. */
     if ( (iSX < 0) || (iSX >= iScnWidth)  || (iDX < 0) || (iDX >= iScnWidth) || 
          (iSY < 0) || (iSY >= iScnHeight) || (iDY < 0) || (iDY >= iScnHeight) )
     {
          NWGFXSetCursorPos(0, 0);
          printf("NWGFXRestoreRect(): Attempt to blit an area outside of the screen.");
          return;
     }
#endif

     /* Calculate blit origin, dimensions and next raster line offset. */
     lpScreen = lpScnOrigin + ( (iScnWidth * 2 * iSY) + (iSX * 2) );
     iWidth   = (iDX - iSX + 1) * 2;
     iHeight  = iDY - iSY + 1;
     iOffset  = (iScnWidth * 2) - iWidth;

     /* Perfom the blit. */
     while(iHeight--)
     {
          /* Fetch the line width in bytes. */
          iTmpWidth = iWidth;

          /* Blit a line. */
          while(iTmpWidth--)
               *lpScreen++ = *lpBuffer++;

          /* Move to next line. */
          lpScreen += iOffset;
     }

     /* Free far memory buffer. */
     _ffree(lpBuffer);
}

/******************************************************************************
** Fill a rectangle defined by (sx,sy):(dx,dy) with the specified character,
** fore and background colours.
*/
VOID NWGFXFillRect(NWGPOS iSX, NWGPOS iSY, NWGPOS iDX, NWGPOS iDY, NWGCHAR cChar,
                   NWGCOLOUR cFGColour, NWGCOLOUR cBGColour)
{
     NWGSCNPTR      lpScreen;           /* Pointer to screen. */
     NWGNUM         iWidth, iHeight;    /* Rectangle dimensions. */
     NWGNUM         iOffset;            /* Next raster line offset. */
     NWGNUM         iTmpWidth;          /* Temporary width counter. */
     NWGCOLOUR      cColours;           /* Colours combined as attribute. */

#ifdef DEBUG
     /* Check we have a valid screen address. */
     if (!lpScnOrigin)
     {
          printf("NWGFXFillRect(): Screen uninitialised.\n");
          exit(1);
     }

     /* Validate rectangle corners. */
     if ( (iSX < 0) || (iSX >= iScnWidth)  || (iDX < 0) || (iDX >= iScnWidth) || 
          (iSY < 0) || (iSY >= iScnHeight) || (iDY < 0) || (iDY >= iScnHeight) )
     {
          NWGFXSetCursorPos(0, 0);
          printf("NWGFXFillRect(): Attempt to fill an area outside of the screen.");
          return;
     }
#endif

     /* Calculate fill origin, dimensions and next raster line offset. */
     lpScreen = lpScnOrigin + ( (iScnWidth * 2 * iSY) + (iSX * 2) );
     iWidth   = iDX - iSX + 1;
     iHeight  = iDY - iSY + 1;
     iOffset  = (iScnWidth * 2) - (iWidth * 2);

     /* Combine colours to create attribute. */
     cColours = (cBGColour << 4) | cFGColour;
     
     /* Perfom the fill. */
     while(iHeight--)
     {
          /* Fetch the line width in bytes. */
          iTmpWidth = iWidth;

          /* Fill a line. */
          while(iTmpWidth--)
          {
               *lpScreen++ = cChar;
               *lpScreen++ = cColours;
          }
          
          /* Move to next line. */
          lpScreen += iOffset;
     }
}

/******************************************************************************
** Clear a rectangle defined by (sx,sy):(dx,dy). The character and foreground
** colour attribute is set to zero.
*/
VOID NWGFXClearRect(NWGPOS iSX, NWGPOS iSY, NWGPOS iDX, NWGPOS iDY)
{
     NWGSCNPTR      lpScreen;           /* Pointer to screen. */
     NWGNUM         iWidth, iHeight;    /* Rectangle dimensions. */
     NWGNUM         iOffset;            /* Next raster line offset. */
     NWGNUM         iTmpWidth;          /* Temporary width counter. */

#ifdef DEBUG
     /* Check we have a valid screen address. */
     if (!lpScnOrigin)
     {
          printf("NWGFXClearRect(): Screen uninitialised.\n");
          exit(1);
     }

     /* Validate rectangle corners. */
     if ( (iSX < 0) || (iSX >= iScnWidth)  || (iDX < 0) || (iDX >= iScnWidth) || 
          (iSY < 0) || (iSY >= iScnHeight) || (iDY < 0) || (iDY >= iScnHeight) )
     {
          NWGFXSetCursorPos(0, 0);
          printf("NWGFXClearRect(): Attempt to clear an area outside of the screen.");
          return;
     }
#endif

     /* Calculate clear origin, dimensions and next raster line offset. */
     lpScreen = lpScnOrigin + ( (iScnWidth * 2 * iSY) + (iSX * 2) );
     iWidth   = iDX - iSX + 1;
     iHeight  = iDY - iSY + 1;
     iOffset  = (iScnWidth * 2) - (iWidth * 2);

     /* Perfom the clear. */
     while(iHeight--)
     {
          /* Fetch the line width in bytes. */
          iTmpWidth = iWidth;

          /* Clear a line. */
          while(iTmpWidth--)
          {
               *lpScreen++  = (NWGCHAR) 0;
               *lpScreen++ &= (NWGCOLOUR) 0xF0;
          }
          
          /* Move to next line. */
          lpScreen += iOffset;
     }
}

/******************************************************************************
** Scroll the area defined by (sx,sy):(dx,dy) up one line. The bottom line is 
** then cleared.
*/
VOID NWGFXScrollRectUp(NWGPOS iSX, NWGPOS iSY, NWGPOS iDX, NWGPOS iDY)
{
     NWGSCNPTR      lpSrc, lpDst;       /* Pointers to screen source & destination. */
     NWGNUM         iWidth, iHeight;    /* Rectangle dimensions. */
     NWGNUM         iOffset;            /* Next raster line offset. */
     NWGNUM         iTmpWidth;          /* Temporary width counter. */

#ifdef DEBUG
     /* Check we have a valid screen address. */
     if (!lpScnOrigin)
     {
          printf("NWGFXScrollRectUp(): Screen uninitialised.\n");
          exit(1);
     }

     /* Validate rectangle corners. */
     if ( (iSX < 0) || (iSX >= iScnWidth)  || (iDX < 0) || (iDX >= iScnWidth) || 
          (iSY < 0) || (iSY >= iScnHeight) || (iDY < 0) || (iDY >= iScnHeight) )
     {
          NWGFXSetCursorPos(0, 0);
          printf("NWGFXScrollRectUp(): Attempt to scroll an area outside of the screen.");
          return;
     }
#endif

     /* Calculate scroll origin, dimensions and next raster line offset. */
     lpDst   = lpScnOrigin + ( (iScnWidth * 2 * iSY) + (iSX * 2) );
     lpSrc   = lpDst + (iScnWidth * 2);
     iWidth  = (iDX - iSX + 1) * 2;
     iHeight = iDY - iSY;
     iOffset = (iScnWidth * 2) - iWidth;

     /* Scroll the region up. */
     while(iHeight--)
     {
          /* Fetch the line width in bytes. */
          iTmpWidth = iWidth;

          /* Copy a line. */
          while(iTmpWidth--)
               *lpDst++  = *lpSrc++;
          
          /* Move to next line. */
          lpDst += iOffset;
          lpSrc += iOffset;
     }

     /* Convert width to character based. */
     iWidth /= 2;

     /* Clear the bottom line. */
     while(iWidth--)
     {
          *lpDst++  = (NWGCHAR) 0;
          *lpDst++ &= (NWGCOLOUR) 0xF0;
     }
}

/******************************************************************************
** Scroll the area defined by (sx,sy):(dx,dy) down one line. The top line is 
** then cleared.
*/
VOID NWGFXScrollRectDown(NWGPOS iSX, NWGPOS iSY, NWGPOS iDX, NWGPOS iDY)
{
     NWGSCNPTR      lpSrc, lpDst;       /* Pointers to screen source & destination. */
     NWGNUM         iWidth, iHeight;    /* Rectangle dimensions. */
     NWGNUM         iOffset;            /* Next raster line offset. */
     NWGNUM         iTmpWidth;          /* Temporary width counter. */

#ifdef DEBUG
     /* Check we have a valid screen address. */
     if (!lpScnOrigin)
     {
          printf("NWGFXScrollRectDown(): Screen uninitialised.\n");
          exit(1);
     }

     /* Validate rectangle corners. */
     if ( (iSX < 0) || (iSX >= iScnWidth)  || (iDX < 0) || (iDX >= iScnWidth) || 
          (iSY < 0) || (iSY >= iScnHeight) || (iDY < 0) || (iDY >= iScnHeight) )
     {
          NWGFXSetCursorPos(0, 0);
          printf("NWGFXScrollRectDown(): Attempt to scroll an area outside of the screen.");
          return;
     }
#endif

     /* Calculate scroll origin, dimensions and next raster line offset. */
     lpDst   = lpScnOrigin + ( (iScnWidth * 2 * iDY) + (iSX * 2) );
     lpSrc   = lpDst - (iScnWidth * 2);
     iWidth  = (iDX - iSX + 1) * 2;
     iHeight = iDY - iSY;
     iOffset = (iScnWidth * 2) + iWidth;

     /* Scroll the region down. */
     while(iHeight--)
     {
          /* Fetch the line width in bytes. */
          iTmpWidth = iWidth;

          /* Copy a line. */
          while(iTmpWidth--)
               *lpDst++  = *lpSrc++;
          
          /* Move to next line. */
          lpDst -= iOffset;
          lpSrc -= iOffset;
     }

     /* Convert width to character based. */
     iWidth /= 2;

     /* Clear the bottom line. */
     while(iWidth--)
     {
          *lpDst++  = (NWGCHAR) 0;
          *lpDst++ &= (NWGCOLOUR) 0xF0;
     }
}

/*******************************************************************************
** Show the line given by (sx,y):(dx,y) as highlighted by changing the text and
** background colours. The colours also depend on whether the item is selected
** or not.
*/
VOID NWGFXHiliteField(NWGPOS iSX, NWGPOS iDX, NWGPOS iY, NWGBOOL bSelected, 
                         NWGBOOL bHiLite)
{
     NWGSCNPTR      lpScreen;           /* Pointer to screen. */
     NWGNUM         iWidth;             /* Line width in characters. */
     NWGCOLOUR      cTextColour;        /* Colour used for text. */
     NWGCOLOUR      cLineColour;        /* Colour used for background. */
     NWGCOLOUR      cColours;           /* Colours combined as attribute. */
     
#ifdef DEBUG
     /* Check we have a valid screen address. */
     if (!lpScnOrigin)
     {
          printf("NWGFXHiliteField(): Screen uninitialised.\n");
          exit(1);
     }

     /* Validate line ends. */
     if ( (iSX < 0) || (iSX >= iScnWidth)  || (iDX < 0) || (iDX >= iScnWidth) || 
          (iY < 0)  || (iY >= iScnHeight) )
     {
          NWGFXSetCursorPos(0, 0);
          printf("NWGFXHiliteField(): Attempt to hilite outside of the screen.");
          return;
     }
#endif

     /* Get colours for text and background. */
     if (iScnType == MONO_SCREEN)
     {
          if (bHiLite)
          {
               if(bSelected)
               {
                    cTextColour = DARK;
                    cLineColour = BRIGHT;
               }
               else
               {
                    cTextColour = DARK;
                    cLineColour = NORMAL;
               }
          }
          else
          {
               if(bSelected)
               {
                    cTextColour = BRIGHT;
                    cLineColour = DARK;
               }
               else
               {
                    cTextColour = NORMAL;
                    cLineColour = DARK;
               }
          }
     }
     else /* iScnType == COLOUR_SCREEN */
     {
          if (bHiLite)
          {
               if(bSelected)
               {
                    cTextColour = BLUE;
                    cLineColour = WHITE;
               }
               else
               {
                    cTextColour = BLUE;
                    cLineColour = LTGREY;
               }
          }
          else
          {
               if(bSelected)
               {
                    cTextColour = YELLOW;
                    cLineColour = BLUE;
               }
               else
               {
                    cTextColour = WHITE;
                    cLineColour = BLUE;
               }
          }
     }

     /* Calculate line origin and dimensions. */
     lpScreen = lpScnOrigin + ( (iScnWidth * 2 * iY) + (iSX * 2) + 1 );
     iWidth   = iDX - iSX + 1;

     /* Combine colours to create attribute. */
     cColours = (cLineColour << 4) | cTextColour;
     
     /* Adjust the attributes. */
     while(iWidth--)
     {
          *lpScreen = cColours;
          lpScreen += 2;
     }
}
