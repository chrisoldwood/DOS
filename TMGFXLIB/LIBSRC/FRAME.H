/******************************************************************************
** (C) Chris Wood 1997
**
** MODULE:		FRAME.H
** COMPONENT:	Text Mode Graphics Library.
** DESCRIPTION:	Frame drawing functions.
**
*******************************************************************************
*/

/* Check for previous inclusion */
#ifndef FRAME_H
#define FRAME_H

/******************************************************************************
**
** Types.
**
*******************************************************************************
*/

/******************************************************************************
**
** Frame style.
**
** Frames can be drawn using either the single or double pixel line characters.
*/

typedef enum tagFrameStyle
{
	SingleLine = 0,
	DoubleLine = 1
} FrameStyle;

/******************************************************************************
**
** Function prototypes.
**
*******************************************************************************
*/

void Frame(DC* pDC, Rect* pRect, FrameStyle eStyle, Colour eClr);
void HorzFrameLine(DC* pDC, int iX, int iY, uint iLen, FrameStyle eStyle, Colour eClr);
void VertFrameLine(DC* pDC, int iX, int iY, uint iLen, FrameStyle eStyle, Colour eClr);

#endif /* FRAME_H */
