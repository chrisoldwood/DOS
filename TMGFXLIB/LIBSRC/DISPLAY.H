/******************************************************************************
** (C) Chris Wood 1997
**
** MODULE:		DISPLAY.H
** COMPONENT:	Text Mode Graphics Library.
** DESCRIPTION:	Video mode functions.
**
*******************************************************************************
*/

/* Check for previous inclusion */
#ifndef DISPLAY_H
#define DISPLAY_H

/******************************************************************************
**
** Types.
**
*******************************************************************************
*/

/******************************************************************************
**
** These are the standard text video modes.
*/

#define BW40X25		0
#define COL40X25	1
#define BW80X25		2
#define COL80X25	3
#define MONO80X25	7

/******************************************************************************
**
** A display pixel.
**
** A text mode pixel is made up of two bytes. The ASCII character and an
** attribute byte. The attribute byte is split into bitfields that make up
** the foregound and background colours and a bit to signal whether the
** foreground text should blink.
**
** This is put in a structure so that we can let the compiler worry about
** masking and shifting bits in the attribute byte.
*/

/* Must be byte aligned. */
#pragma pack(1)

typedef struct tagPixel
{
	byte	cChar;				/* The pixels' character     */
	byte	iFgColour : 4,		/* Foreground colour         */
			iBgColour : 3,		/* Background colour         */
			bBlink    : 1;		/* Foreground text blinking? */
} Pixel;

#pragma pack()

/******************************************************************************
**
** The background mode.
**
** This defines how the background colour affects drawing operations.
*/

typedef enum tagBgMode
{
	Transparent,
	Opaque
} BgMode;

/******************************************************************************
**
** A display context.
**
** This structure holds all the necessary information about a display on
** which you can draw. All of the drawing functions require one of these because
** it allows us to virualise video pages and adapters. We can also then
** virtualise viewports and clipping rectangles, which the Windows' library
** will need. A display can also be a memory buffer.
**
** Note: The clipping rectangle is always in absolute coordinates, i.e. it is
**       not affected by the viewport origin.
*/

typedef struct tagDC
{
	Pixel _far*	pBuffer;		/* The address of the screen buffer */
	uint		iPage;			/* The video page                   */
	uint		iWidth;			/* The buffer width in 'pixels'     */
	uint		iHeight;		/* The buffer height in 'pixels'    */
	BgMode		eBgMode;		/* Background mode                  */
	Colour		eBgColour;		/* Background colour                */
	Point		ptOrigin;		/* The viewport origin              */
	Rect		rcClip;			/* The clipping rectangle           */
} DC;

/******************************************************************************
**
** Function prototypes.
**
*******************************************************************************
*/

uint GetDisplayMode(void);
bool SetDisplayMode(uint iMode);
Pixel _far* GetDisplayAddress(void);
uint GetDisplayPage(void);
uint GetDisplayWidth(void);
uint GetDisplayHeight(void);
void GetDisplayRect(Rect* pRect);
void GetDisplayDC(DC* pDC);
void ClearDisplay(DC* pDC);
void OpenDisplay(DC* pDC);
void CloseDisplay(void);

/******************************************************************************
**
** Inline functions.
**
*******************************************************************************
*/

/******************************************************************************
** Get the address of the pixel at iX,iY.
*/

__inline Pixel _far* GetPixelAddress(DC* pDC, int iX, int iY)
{
	ASSERT(pDC);
	
	return pDC->pBuffer + (iY * pDC->iWidth) + iX;
}

#endif /* DISPLAY_H */
