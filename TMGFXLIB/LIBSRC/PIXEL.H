/******************************************************************************
** (C) Chris Wood 1997
**
** MODULE:		PIXEL.H
** COMPONENT:	Text Mode Graphics Library.
** DESCRIPTION:	Single pixel functions.
**
*******************************************************************************
*/

/* Check for previous inclusion */
#ifndef PIXEL_H
#define PIXEL_H

/******************************************************************************
**
** Function prototypes.
**
*******************************************************************************
*/

void SetPixel(DC* pDC, int iX, int iY, byte cChar, Colour eClr);
void SetPixelChar(DC* pDC, int iX, int iY, byte cChar);
byte GetPixelChar(DC* pDC, int iX, int iY);
void SetPixelFgColour(DC* pDC, int iX, int iY, Colour eClr);
Colour GetPixelFgColour(DC* pDC, int iX, int iY);
void SetPixelBgColour(DC* pDC, int iX, int iY, Colour eClr);
Colour GetPixelBgColour(DC* pDC, int iX, int iY);
void SetPixelBlink(DC* pDC, int iX, int iY, int bBlink);

#endif /* PIXEL_H */
