/******************************************************************************
** (C) Chris Wood 1997
**
** MODULE:		TEXT.C
** COMPONENT:	Text Mode Graphics Library.
** DESCRIPTION:	Text output functions.
**
*******************************************************************************
*/

#include "tmgfx.h"
#include <string.h>

/******************************************************************************
** Function:	LeftText()
**
** Description:	Output a null terminated string, left justified at the positon
**				given.
**
** Parameters:	pDC			The display context.
**				iX, iY		The line start position.
**				pText		A null terminated string.
**				eClr		The text colour.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void LeftText(DC* pDC, int iX, int iY, char* pText, Colour eClr)
{
	Pixel _far*	pBuffer;
	int			iX2;
	int			iLen;

	ASSERT(pDC);
    ASSERT(pText);
    
    /* Check for empty string. */
    iLen = strlen((char*)pText);
    if (!iLen)
    	return;
    	
    /* Adjust position for origin. */
    iX += pDC->ptOrigin.x;
    iY += pDC->ptOrigin.y;
    
    /* Calculate text end. */
    iX2 = iX + (int)iLen - 1;
    
    /* Check Y clipping for visibility. */
    if ( (iY < pDC->rcClip.y1) || (iY > pDC->rcClip.y2) )
    	return;

	/* Check X clipping for visibility. */
	if ( (iX2 < pDC->rcClip.x1) || (iX > pDC->rcClip.x2) )
		return;
    
    /* Check text start for clipping. */
    if (iX < pDC->rcClip.x1)
    {
    	/* Adjust text starting point. */
    	pText += (pDC->rcClip.x1 - iX);
    	iLen  -= (pDC->rcClip.x1 - iX);
    	iX     = pDC->rcClip.x1;
    }

    /* Check text end for clipping. */
    if (iX2 > pDC->rcClip.x2)
    {
    	/* Adjust text length. */
    	iLen -= (iX2 - pDC->rcClip.x2);
    }
    
	/* Get text start address. */
	pBuffer = GetPixelAddress(pDC, iX, iY);
	
	/* Draw text. */
	while(iLen--)
	{
		/* Set character and colour. */
		pBuffer->cChar     = *pText++;
		pBuffer->iFgColour = (byte) eClr;
		
		/* Set background? */
		if (pDC->eBgMode == Opaque)
			pBuffer->iBgColour = (byte) pDC->eBgColour;
			
		pBuffer++;
	}
}

/******************************************************************************
** Function:	RightText()
**
** Description:	Output a null terminated string, right justified at the positon
**				given.
**
** Parameters:	pDC			The display context.
**				iX, iY		The line start position.
**				pText		A null terminated string.
**				eClr		The text colour.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void RightText(DC* pDC, int iX, int iY, char* pText, Colour eClr)
{
	ASSERT(pDC);
    ASSERT(pText);
    
    /* Adjust starting position to make it left justified. */
    iX -= strlen((char*)pText) - 1;
    
	LeftText(pDC, iX, iY, pText, eClr);
}

/******************************************************************************
** Function:	CentreText()
**
** Description:	Output a null terminated string, centred in the rectangle
**				given. The rectangle width must be >= the length of the text.
**
** Parameters:	pDC			The display context.
**				pRect		The rectangle to centre in.
**				pText		A null terminated string.
**				eClr		The text colour.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CentreText(DC* pDC, Rect* pRect, char* pText, Colour eClr)
{
	int iX, iY;
	
	ASSERT(pDC);
    ASSERT(pText);
    ASSERT(((int)(GetRectWidth(pRect) - strlen(pText))) >= 0);
    
    /* Calculate left justified position. */
    iX = pRect->x1 + ((GetRectWidth(pRect) - strlen(pText)) / 2);
    iY = pRect->y1 + (GetRectHeight(pRect) / 2);
    
	LeftText(pDC, iX, iY, pText, eClr);
}
