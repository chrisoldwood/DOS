/******************************************************************************
** (C) Chris Wood 1997
**
** MODULE:		CLIPRECT.H
** COMPONENT:	Text Mode Graphics Library.
** DESCRIPTION:	Clipping rectangle functions.
**
*******************************************************************************
*/

/* Check for previous inclusion */
#ifndef CLIPRECT_H
#define CLIPRECT_H

/******************************************************************************
**
** Structures.
**
*******************************************************************************
*/

/******************************************************************************
**
** A rectangle.
**
** It is assumed that the rectangle extends from x1,y1 to x2,y2 and includes
** the bottom right point x2,y2.
*/

typedef struct tagRect
{
	int	x1;
	int	y1;
	int	x2;
	int	y2;
} Rect;

/******************************************************************************
**
** Functions prototypes.
**
*******************************************************************************
*/

bool IsPointInRect(int iX, int iY, Rect* pRect);
bool DoRectsIntersect(Rect* pRect1, Rect* pRect2);
void GetIntersectRect(Rect* pRect1, Rect* pRect2, Rect* pDstRect);

/******************************************************************************
**
** Inline functions.
**
*******************************************************************************
*/

/******************************************************************************
** Set the members of a rectangle.
*/

__inline void SetRect(Rect* pRect, int x1, int y1, int x2, int y2)
{
	ASSERT(pRect);
	ASSERT(x2 >= x1);
	ASSERT(y2 >= y1);

	pRect->x1 = x1;
	pRect->y1 = y1;
	pRect->x2 = x2;
	pRect->y2 = y2;
}

/******************************************************************************
** Get the width of a rectangle.
*/

__inline uint GetRectWidth(Rect* pRect)
{
	ASSERT(pRect);
	ASSERT(pRect->x2 >= pRect->x1);

	return (pRect->x2 - pRect->x1 + 1);
}

/******************************************************************************
** Get the height of a rectangle.
*/

__inline uint GetRectHeight(Rect* pRect)
{
	ASSERT(pRect);
	ASSERT(pRect->y2 >= pRect->y1);

	return (pRect->y2 - pRect->y1 + 1);
}

#endif /* CLIPRECT_H */
