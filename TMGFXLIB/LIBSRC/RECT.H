/******************************************************************************
** (C) Chris Wood 1997
**
** MODULE:		RECT.H
** COMPONENT:	Text Mode Graphics Library.
** DESCRIPTION:	Rectangle drawing functions.
**
*******************************************************************************
*/

/* Check for previous inclusion */
#ifndef RECT_H
#define RECT_H

/******************************************************************************
**
** Function prototypes.
**
*******************************************************************************
*/

void RectBorder(DC* pDC, Rect* pRect, byte cChar, Colour eClr);
void FillRect(DC* pDC, Rect* pRect, byte cChar, Colour eClr);
void BlitRect(DC* pDstDC, Rect* pRect, DC* pSrcDC, int iX, int iY);
void ScrollRect(DC* pDC, Rect* pRect, int iXOffset, int iYOffset);

#endif /* RECT_H */
