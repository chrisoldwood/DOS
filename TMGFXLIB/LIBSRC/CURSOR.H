/******************************************************************************
** (C) Chris Wood 1997
**
** MODULE:		CURSOR.H
** COMPONENT:	Text Mode Graphics Library.
** DESCRIPTION:	Cursor functions.
**
*******************************************************************************
*/

/* Check for previous inclusion */
#ifndef CURSOR_H
#define CURSOR_H

/******************************************************************************
**
** Function prototypes.
**
*******************************************************************************
*/

void SetCursorPos(DC* pDC, int iX, int iY);
void GetCursorPos(DC* pDC, int* pX, int *pY);
void HideCursor(DC* pDC);

#endif /* CURSOR_H */
