/******************************************************************************
** (C) Chris Wood 1997
**
** MODULE:		TEXT.H
** COMPONENT:	Text Mode Graphics Library.
** DESCRIPTION:	Text output functions.
**
*******************************************************************************
*/

/* Check for previous inclusion */
#ifndef TEXT_H
#define TEXT_H

/******************************************************************************
**
** Function prototypes.
**
*******************************************************************************
*/

void LeftText(DC* pDC, int iX, int iY, char* pText, Colour eClr);
void RightText(DC* pDC, int iX, int iY, char* pText, Colour eClr);
void CentreText(DC* pDC, Rect* pRect, char* pText, Colour eClr);

#endif /* TEXT_H */
