/******************************************************************************
** (C) Chris Wood 1997
**
** MODULE:		LINE.H
** COMPONENT:	Text Mode Graphics Library.
** DESCRIPTION:	Line drawing functions.
**
*******************************************************************************
*/

/* Check for previous inclusion */
#ifndef LINE_H
#define LINE_H

/******************************************************************************
**
** Function prototypes.
**
*******************************************************************************
*/

void HorzLine(DC* pDC, int iX, int iY, uint iLen, byte cChar, Colour eClr);
void VertLine(DC* pDC, int iX, int iY, uint iLen, byte cChar, Colour eClr);

#endif /* LINE_H */
