/******************************************************************************
** (C) Chris Wood 1997
**
** MODULE:		DISPLAY.C
** COMPONENT:	Text Mode Graphics Library.
** DESCRIPTION:	Video display functions.
**
*******************************************************************************
*/

#include "tmgfx.h"
#include <dos.h>

/******************************************************************************
** Function:	GetDisplayMode()
**
** Description:	Gets the current video mode.
**
** Parameters:	None.
**
** Returns:		The video mode.
**
*******************************************************************************
*/

uint GetDisplayMode(void)
{
	byte iMode;

	/* Get mode. */
	__asm
	{
		mov	ah,15
		int 10h
		mov iMode,al
	}
	
	return iMode;
}

/******************************************************************************
** Function:	SetDisplayMode()
**
** Description:	Sets the current video mode.
**
** Parameters:	iMode	The new text video mode.
**
** Returns:		TRUE	On success.
**				FASLE	On failure.
**
*******************************************************************************
*/

bool SetDisplayMode(uint iMode)
{
	byte iNewMode = (byte) iMode;
	
	/* Try and change mode. */
	__asm
	{
		mov ah,0
		mov al,iNewMode
		int 10h
	}
	
	/* Video mode changed? */
	if (GetDisplayMode() == iMode)
		return TRUE;
		
	return FALSE;
}

/******************************************************************************
** Function:	GetDisplayAddress()
**
** Description:	Gets the video buffer address.
**
** Parameters:	None.
**
** Returns:		The buffer address for known text video modes.
**				NULL for unknown or graphics video modes.
**
*******************************************************************************
*/

Pixel _far* GetDisplayAddress(void)
{
	word wSegment, wOffset;

    /* Get buffer segment. */
    switch(GetDisplayMode())
    {
		case BW40X25:
		case COL40X25:
		case BW80X25:
		case COL80X25:
			wSegment = 0xB800;
			break;

		case MONO80X25:
			wSegment = 0xB000;
			break;
    
    	default:
    		ASSERT(FALSE);
    		return NULL;
    }
	
	/* Get video buffer offset. */
	wOffset = *((word _far*) MK_FP(0x0040, 0x004E));
	
	/* Convert to far pointer. */
	return ((Pixel _far*) MK_FP(wSegment, wOffset));
}

/******************************************************************************
** Function:	GetDisplayPage()
**
** Description:	Gets the current dispay page.
**
** Parameters:	None.
**
** Returns:		The video page.
**
*******************************************************************************
*/

uint GetDisplayPage(void)
{
	byte iPage;

	/* Get mode. */
	__asm
	{
		mov	ah,15
		int 10h
		mov iPage,bh
	}
	
	return iPage;
}

/******************************************************************************
** Function:	GetDisplayWidth()
**
** Description:	Gets the current display width in 'pixels'.
**
** Parameters:	None.
**
** Returns:		The width.
**
*******************************************************************************
*/

uint GetDisplayWidth(void)
{
	return (*((word _far*) MK_FP(0x0040, 0x004A)));
}

/******************************************************************************
** Function:	GetDisplayHeight()
**
** Description:	Gets the current display height in 'pixels'.
**
** Parameters:	None.
**
** Returns:		The height.
**
*******************************************************************************
*/

uint GetDisplayHeight(void)
{
	return (*((byte _far*) MK_FP(0x0040, 0x0084))) + 1;
}

/******************************************************************************
** Function:	GetDisplayRect()
**
** Description:	Gets the current screen dimensions as a rect.
**
** Parameters:	pRect		The return rect.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void GetDisplayRect(Rect* pRect)
{
	ASSERT(pRect);
	
	pRect->x1 = 0;
	pRect->y1 = 0;
	pRect->x2 = GetDisplayWidth()  - 1;
	pRect->y2 = GetDisplayHeight() - 1;
}

/******************************************************************************
** Function:	GetDisplayDC()
**
** Description:	Gets a display context for the current display.
**
** Parameters:	pDC		The display context to fill in.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void GetDisplayDC(DC* pDC)
{
	Rect rcScreen;
	
	ASSERT(pDC);
	
	/* Video settings. */
	pDC->pBuffer = GetDisplayAddress();
	pDC->iPage   = GetDisplayPage();
	pDC->iWidth  = GetDisplayWidth();
	pDC->iHeight = GetDisplayHeight();
	
	/* Background mode and colour */
	pDC->eBgMode   = Transparent;
	pDC->eBgColour = Black;
	
	/* Set origin to top-left. */
	SetPoint(&pDC->ptOrigin, 0, 0);

	/* Set clipping rectangle to the whole screen. */
	GetDisplayRect(&rcScreen);
	pDC->rcClip = rcScreen;
}

/******************************************************************************
** Function:	ClearDisplay()
**
** Description:	Clears a display to black.
**
** Parameters:	pDC		The display context to clear.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void ClearDisplay(DC* pDC)
{
	Pixel _far*	pBuffer;
	uint 		iNumPixels;

	ASSERT(pDC);

	/* Get pixel address. */
	pBuffer = GetPixelAddress(pDC, 0, 0);
	
	/* Calculate the number of pixels to clear. */
	iNumPixels = pDC->iWidth * pDC->iHeight;
	
	/* For all pixels. */
	while(iNumPixels--)
	{
		/* Clear pixel. */
		pBuffer->cChar     = 0;
		pBuffer->iFgColour = Black;
		pBuffer->iBgColour = Black;
		pBuffer->bBlink    = FALSE;
		
		pBuffer++;
	}
}

/******************************************************************************
**
** These are used by Open/CloseDisplay() to save the current display settings
** and contents.
*/

static uint 	iOldMode;
static DC		dcOldSetup;
static DC		dcOldContents;
static Pixel*	pOldContents = NULL;
static int		iOldCurX, iOldCurY;

/******************************************************************************
** Function:	OpenDisplay()
**
** Description:	Opens the current display for drawing. This call saves the
**				current display settings and contents so that they can be
**				restored later.
**
** Parameters:	pDC		The returned display context for the current display.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void OpenDisplay(DC* pDC)
{
	
	ASSERT(pDC);
	
	/* Get current settings. */
	iOldMode = GetDisplayMode();
	GetDisplayDC(&dcOldSetup);
	GetCursorPos(&dcOldSetup, &iOldCurX, &iOldCurY);
	
	/* Allocate storage for previous contents. */
	pOldContents = malloc(sizeof(Pixel) * dcOldSetup.iWidth * dcOldSetup.iHeight);
	ASSERT(pOldContents);

	if (pOldContents)
	{
		/* Setup up old contents DC. */
		dcOldContents.pBuffer = pOldContents;
		dcOldContents.iWidth  = dcOldSetup.iWidth;
		dcOldContents.iHeight = dcOldSetup.iHeight;
		SetPoint(&dcOldContents.ptOrigin, 0, 0);
		dcOldContents.rcClip = dcOldSetup.rcClip;

		/* Save contents. */
		BlitRect(&dcOldContents, &dcOldContents.rcClip, &dcOldSetup, 0, 0);
	}
	
	/* Fill return buffer. */
	GetDisplayDC(pDC);
}

/******************************************************************************
** Function:	CloseDisplay()
**
** Description:	Closes the display and restores it to its former state.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CloseDisplay(void)
{
	/* Restore video settings. */
	SetDisplayMode(iOldMode);
	SetCursorPos(&dcOldSetup, iOldCurX, iOldCurY);
	
	if (pOldContents)
	{
		/* Restore contents. */
		BlitRect(&dcOldSetup, &dcOldSetup.rcClip, &dcOldContents, 0, 0);
		
		free(pOldContents);
	}
}
