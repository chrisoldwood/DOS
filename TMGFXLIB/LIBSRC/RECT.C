/******************************************************************************
** (C) Chris Wood 1997
**
** MODULE:		RECT.C
** COMPONENT:	Text Mode Graphics Library.
** DESCRIPTION:	Rectangle drawing functions.
**
*******************************************************************************
*/

#include "tmgfx.h"

/******************************************************************************
** Function:	RectBorder()
**
** Description:	Draw a rectangle on a display using the specified character and
**				colour.
**
** Parameters:	pDC			The display context.
**				pRect		The rectangle position.
**				cChar		The character.
**				eClr		The colour.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void RectBorder(DC* pDC, Rect* pRect, byte cChar, Colour eClr)
{
	uint iWidth, iHeight;
	
	ASSERT(pDC);
	ASSERT(pRect);

    iWidth  = GetRectWidth(pRect);
    iHeight = GetRectHeight(pRect);

    HorzLine(pDC, pRect->x1, pRect->y1, iWidth,  cChar, eClr);
    HorzLine(pDC, pRect->x1, pRect->y2, iWidth,  cChar, eClr);
    VertLine(pDC, pRect->x1, pRect->y1, iHeight, cChar, eClr);
    VertLine(pDC, pRect->x2, pRect->y1, iHeight, cChar, eClr);
}

/******************************************************************************
** Function:	FillRect()
**
** Description:	Fill a rectangle on a display using the specified character and
**				colour.
**
** Parameters:	pDC			The display context.
**				pRect		The rectangle position.
**				cChar		The character.
**				eClr		The colour.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void FillRect(DC* pDC, Rect* pRect, byte cChar, Colour eClr)
{
	Rect 		rcRect;
	Pixel _far* pBuffer;
	uint 		iRectWidth, iRectHeight;

	ASSERT(pDC);
	ASSERT(pRect);
	
	/* Copy rect and adjust for origin. */
	rcRect = *pRect;
	rcRect.x1 += pDC->ptOrigin.x;
	rcRect.y1 += pDC->ptOrigin.y;
	rcRect.x2 += pDC->ptOrigin.x;
	rcRect.y2 += pDC->ptOrigin.y;
	
	/* Check for clipping. */
	if (!DoRectsIntersect(&rcRect, &pDC->rcClip))
		return;
	
	/* Get clipped rectangle. */
	GetIntersectRect(&rcRect, &pDC->rcClip, &rcRect);
	
	/* For each line. */
	iRectHeight = GetRectHeight(&rcRect);

	while(iRectHeight--)
	{
		/* Get line start address. */
		pBuffer = GetPixelAddress(pDC, rcRect.x1, rcRect.y1+iRectHeight);
		
		/* Calculate the number of pixels to clear. */
		iRectWidth = GetRectWidth(&rcRect);
		
		/* For all pixels in the line. */
		while(iRectWidth--)
		{
			/* Set character and colour. */
			pBuffer->cChar = cChar;
			pBuffer->iFgColour = (byte) eClr;
				
			/* Set background? */
			if (pDC->eBgMode == Opaque)
				pBuffer->iBgColour = (byte) pDC->eBgColour;
			
			pBuffer++;
		}
	}
}

/******************************************************************************
** Function:	BlitRect()
**
** Description:	Blit a rectangle from one display context to another. The
**				contexts can refer to the same physical display and the areas
**				can overlap.
**
** Parameters:	pDstDC		The destination display context.
**				pRect		The destination rectangle.
**				pSrcDc		The source display context.
**				iX, iY		The source rectangle origin.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void BlitRect(DC* pDstDC, Rect* pRect, DC* pSrcDC, int iX, int iY)
{
	Rect		rcRect, rcDstRect;
	uint		iNumLines, iNumPixels;
	int			iDx, iDy;
	uint		iXOffset, iYOffset;
	Pixel _far* pSrcBuffer;
	Pixel _far* pDstBuffer;
	
	ASSERT(pDstDC);
	ASSERT(pSrcDC);
	ASSERT(pRect);
	
    /* Adjust src position for origin. */
    iX += pSrcDC->ptOrigin.x;
    iY += pSrcDC->ptOrigin.y;
	
	/* Copy dst rect and adjust for origin. */
	rcRect = *pRect;
	rcRect.x1 += pDstDC->ptOrigin.x;
	rcRect.y1 += pDstDC->ptOrigin.y;
	rcRect.x2 += pDstDC->ptOrigin.x;
	rcRect.y2 += pDstDC->ptOrigin.y;
	
	/* Check for clipping. */
	if (!DoRectsIntersect(&rcRect, &pDstDC->rcClip))
		return;
	
	/* Get clipped rectangle. */
	GetIntersectRect(&rcRect, &pDstDC->rcClip, &rcDstRect);
	
	/* Adjust src position for clipping. */
	iX += rcDstRect.x1 - rcRect.x1;
	iY += rcDstRect.y1 - rcRect.y1;
	
	/* Copy left to right or right to left? */
	if (rcDstRect.x1 <= iX)
	{
		iDx      = +1;
		iXOffset = 0;
	}
	else
	{
		iDx      = -1;
		iXOffset = GetRectWidth(&rcDstRect)-1;
	}

	/* Copy top to bottom or bottom to top? */
	if (rcDstRect.y1 <= iY)
	{
		iDy      = +1;
		iYOffset = 0;
	}
	else
	{
		iDy      = -1;
		iYOffset = GetRectHeight(&rcDstRect)-1;
	}
	
	/* For each line. */
	iNumLines = GetRectHeight(&rcDstRect);

	while(iNumLines--)
	{
		/* Get line start addresses. */
		pSrcBuffer = GetPixelAddress(pSrcDC, iX+iXOffset, iY+iYOffset);
		pDstBuffer = GetPixelAddress(pDstDC, rcDstRect.x1+iXOffset, rcDstRect.y1+iYOffset);
		
		/* Calculate the number of pixels to copy. */
		iNumPixels = GetRectWidth(&rcDstRect);
		
		/* For all pixels in the line. */
		while(iNumPixels--)
		{
			/* Copy and move on. */
			*pDstBuffer = *pSrcBuffer;
			pDstBuffer += iDx;
			pSrcBuffer += iDx;
		}
		
		/* Move on a line. */
		iYOffset += iDy;
	}
}

/******************************************************************************
** Function:	ScrollRect()
**
** Description:	Scroll a rectangle in any direction. The offsets can be +ve or
**				-ve. A +ve value scrolls to the right/down and a -ve value
**				scrolls left/up.
**
** Parameters:	pDC			The display context.
**				pRect		The rectangle position.
**				cChar		The character.
**				eClr		The colour.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void ScrollRect(DC* pDC, Rect* pRect, int iXOffset, int iYOffset)
{
	ASSERT();

}
