/******************************************************************************
** (C) Chris Wood 1997
**
** MODULE:		PIXEL.C
** COMPONENT:	Text Mode Graphics Library.
** DESCRIPTION:	Single pixel functions.
**
*******************************************************************************
*/

#include "tmgfx.h"

/******************************************************************************
** Function:	SetPixel()
**
** Description:	Sets a pixel on a screen to the specified character and colour.
**
** Parameters:	pDC			The display context.
**				iX, iY		The pixel position.
**				cChar		The pixel character.
**				eClr		The pixel colour.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void SetPixel(DC* pDC, int iX, int iY, byte cChar, Colour eClr)
{
	Pixel _far*	pBuffer;

	ASSERT(pDC);
    
    /* Adjust position for origin. */
    iX += pDC->ptOrigin.x;
    iY += pDC->ptOrigin.y;
    
    /* Check clipping. */
    if (!IsPointInRect(iX, iY, &pDC->rcClip))
    	return;
    
	/* Get pixel address. */
	pBuffer = GetPixelAddress(pDC, iX, iY);
	
	/* Set character and colour. */
	pBuffer->cChar = cChar;
	pBuffer->iFgColour = (byte) eClr;
	
	/* Set background? */
	if (pDC->eBgMode == Opaque)
		pBuffer->iBgColour = (byte) pDC->eBgColour;
}

/******************************************************************************
** Function:	SetPixelChar()
**
** Description:	Sets the pixels' character at the specified point.
**
** Parameters:	pDC			The display context.
**				iX, iY		The pixel position.
**				cChar		The character.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void SetPixelChar(DC* pDC, int iX, int iY, byte cChar)
{
	Pixel _far*	pBuffer;

	ASSERT(pDC);
    
    /* Adjust position for origin. */
    iX += pDC->ptOrigin.x;
    iY += pDC->ptOrigin.y;
    
    /* Check clipping. */
    if (!IsPointInRect(iX, iY, &pDC->rcClip))
    	return;
    
	/* Get pixel address. */
	pBuffer = GetPixelAddress(pDC, iX, iY);
	
	/* Set character and colour. */
	pBuffer->cChar = cChar;
}

/******************************************************************************
** Function:	GetPixelChar()
**
** Description:	Gets the pixels' character from the specified point.
**
** Parameters:	pDC			The display context.
**				iX, iY		The pixel position.
**
** Returns:		The character.
**
*******************************************************************************
*/

byte GetPixelChar(DC* pDC, int iX, int iY)
{
	Pixel _far*	pBuffer;

	ASSERT(pDC);
    
    /* Adjust position for origin. */
    iX += pDC->ptOrigin.x;
    iY += pDC->ptOrigin.y;
    
	/* Get pixel address. */
	pBuffer = GetPixelAddress(pDC, iX, iY);
	
	/* Set character and colour. */
	return pBuffer->cChar;
}

/******************************************************************************
** Function:	SetPixelFgColour()
**
** Description:	Sets the pixels' foreground colour at the specified point.
**
** Parameters:	pDC			The display context.
**				iX, iY		The pixel position.
**				eClr		The colour.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void SetPixelFgColour(DC* pDC, int iX, int iY, Colour eClr)
{
	Pixel _far*	pBuffer;

	ASSERT(pDC);
    
    /* Adjust position for origin. */
    iX += pDC->ptOrigin.x;
    iY += pDC->ptOrigin.y;
    
    /* Check clipping. */
    if (!IsPointInRect(iX, iY, &pDC->rcClip))
    	return;
    
	/* Get pixel address. */
	pBuffer = GetPixelAddress(pDC, iX, iY);
	
	/* Set character and colour. */
	pBuffer->iFgColour = (byte) eClr;
}

/******************************************************************************
** Function:	GetPixelFgColour()
**
** Description:	Gets the pixels' foreground colour from the specified point.
**
** Parameters:	pDC			The display context.
**				iX, iY		The pixel position.
**
** Returns:		The colour.
**
*******************************************************************************
*/

Colour GetPixelFgColour(DC* pDC, int iX, int iY)
{
	Pixel _far*	pBuffer;

	ASSERT(pDC);
    
    /* Adjust position for origin. */
    iX += pDC->ptOrigin.x;
    iY += pDC->ptOrigin.y;
    
	/* Get pixel address. */
	pBuffer = GetPixelAddress(pDC, iX, iY);
	
	/* Set character and colour. */
	return pBuffer->iFgColour;
}

/******************************************************************************
** Function:	SetPixelBgColour()
**
** Description:	Sets the pixels' background colour at the specified point.
**
** Parameters:	pDC			The display context.
**				iX, iY		The pixel position.
**				eClr		The colour.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void SetPixelBgColour(DC* pDC, int iX, int iY, Colour eClr)
{
	Pixel _far*	pBuffer;

	ASSERT(pDC);
    
    /* Adjust position for origin. */
    iX += pDC->ptOrigin.x;
    iY += pDC->ptOrigin.y;
    
    /* Check clipping. */
    if (!IsPointInRect(iX, iY, &pDC->rcClip))
    	return;
    
	/* Get pixel address. */
	pBuffer = GetPixelAddress(pDC, iX, iY);
	
	/* Set character and colour. */
	pBuffer->iBgColour = (byte) eClr;
}

/******************************************************************************
** Function:	GetPixelBgColour()
**
** Description:	Gets the pixels' background colour from the specified point.
**
** Parameters:	pDC			The display context.
**				iX, iY		The pixel position.
**
** Returns:		The colour.
**
*******************************************************************************
*/

Colour GetPixelBgColour(DC* pDC, int iX, int iY)
{
	Pixel _far*	pBuffer;

	ASSERT(pDC);
    
    /* Adjust position for origin. */
    iX += pDC->ptOrigin.x;
    iY += pDC->ptOrigin.y;
    
	/* Get pixel address. */
	pBuffer = GetPixelAddress(pDC, iX, iY);
	
	/* Set character and colour. */
	return pBuffer->iBgColour;
}

/******************************************************************************
** Function:	SetPixelBlink()
**
** Description:	Sets the pixels' blink style at the specified point.
**
** Parameters:	pDC			The display context.
**				iX, iY		The pixel position.
**				bBlink		The blink style.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void SetPixelBlink(DC* pDC, int iX, int iY, int bBlink)
{
	Pixel _far*	pBuffer;

	ASSERT(pDC);
    
    /* Adjust position for origin. */
    iX += pDC->ptOrigin.x;
    iY += pDC->ptOrigin.y;
    
    /* Check clipping. */
    if (!IsPointInRect(iX, iY, &pDC->rcClip))
    	return;
    
	/* Get pixel address. */
	pBuffer = GetPixelAddress(pDC, iX, iY);
	
	/* Set character and colour. */
	pBuffer->bBlink = (byte) bBlink;
}
