/******************************************************************************
** (C) Chris Wood 1997
**
** MODULE:		LINE.C
** COMPONENT:	Text Mode Graphics Library.
** DESCRIPTION:	Line drawing functions.
**
*******************************************************************************
*/

#include "tmgfx.h"

/******************************************************************************
** Function:	HorzLine()
**
** Description:	Draws a horizontal line from left to right on a screen using
**				the specified character and colour.
**
** Parameters:	pDC			The display context.
**				iX, iY		The line start position.
**				iLen		The line length.
**				cChar		The line character.
**				eClr		The line colour.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void HorzLine(DC* pDC, int iX, int iY, uint iLen, byte cChar, Colour eClr)
{
	Pixel _far*	pBuffer;
	int			iX2;

	ASSERT(pDC);

    /* Adjust position for origin. */
    iX += pDC->ptOrigin.x;
    iY += pDC->ptOrigin.y;
    
    /* Calculate line end. */
    iX2 = iX + (int)iLen - 1;
    
    /* Check Y clipping for visibility. */
    if ( (iY < pDC->rcClip.y1) || (iY > pDC->rcClip.y2) )
    	return;

	/* Check X clipping for visibility. */
	if ( (iX2 < pDC->rcClip.x1) || (iX > pDC->rcClip.x2) )
		return;
    
    /* Check line start for clipping. */
    if (iX < pDC->rcClip.x1)
    {
    	iLen -= (pDC->rcClip.x1 - iX);
    	iX    = pDC->rcClip.x1;
    }

    /* Check line end for clipping. */
    if (iX2 > pDC->rcClip.x2)
    {
    	iLen -= (iX2 - pDC->rcClip.x2);
    }
    
	/* Get line start address. */
	pBuffer = GetPixelAddress(pDC, iX, iY);
	
	/* Draw line. */
	while(iLen--)
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

/******************************************************************************
** Function:	VertLine()
**
** Description:	Draws a vertical line from top to bottom on a screen using the
**				specified character and colour.
**
** Parameters:	pDC			The display context.
**				iX, iY		The line start position.
**				iLen		The line length.
**				cChar		The line character.
**				eClr		The line colour.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void VertLine(DC* pDC, int iX, int iY, uint iLen, byte cChar, Colour eClr)
{
	Pixel _far*	pBuffer;
	int			iY2;

	ASSERT(pDC);

    /* Adjust position for origin. */
    iX += pDC->ptOrigin.x;
    iY += pDC->ptOrigin.y;

    /* Calculate line end. */
    iY2 = iY + (int)iLen - 1;
    
    /* Check Y clipping for visibility. */
    if ( (iX < pDC->rcClip.x1) || (iX > pDC->rcClip.x2) )
    	return;

	/* Check X clipping for visibility. */
	if ( (iY2 < pDC->rcClip.y1) || (iY > pDC->rcClip.y2) )
		return;
    
    /* Check line start for clipping. */
    if (iY < pDC->rcClip.y1)
    {
    	iLen -= (pDC->rcClip.y1 - iY);
    	iY    = pDC->rcClip.y1;
    }

    /* Check line end for clipping. */
    if (iY2 > pDC->rcClip.y2)
    {
    	iLen -= (iY2 - pDC->rcClip.y2);
    }

	/* Get line start address. */
	pBuffer = GetPixelAddress(pDC, iX, iY);
	
	/* Draw line. */
	while(iLen--)
	{
		/* Set character and colour. */
		pBuffer->cChar = cChar;
		pBuffer->iFgColour = (byte) eClr;
		
		/* Set background? */
		if (pDC->eBgMode == Opaque)
			pBuffer->iBgColour = (byte) pDC->eBgColour;
			
		pBuffer += pDC->iWidth;
	}
}
