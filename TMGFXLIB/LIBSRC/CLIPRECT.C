/******************************************************************************
** (C) Chris Wood 1997
**
** MODULE:		CLIPRECT.C
** COMPONENT:	Text Mode Graphics Library.
** DESCRIPTION:	Clipping rectangle functions.
**
*******************************************************************************
*/

#include "tmgfx.h"

/******************************************************************************
** Function:	IsPointInRect()
**
** Description:	Is the specified point inside the specified rectangle. It also
**				includes the border of the rectangle.
**
** Parameters:	iX,iY		The point.
**				pRect		The rectangle.
**
** Returns:		TRUE		If it is.
**				FALSE		If not.
**
*******************************************************************************
*/

bool IsPointInRect(int iX, int iY, Rect* pRect)
{
	ASSERT(pRect);
	ASSERT(pRect->x2 >= pRect->x1);
	ASSERT(pRect->y2 >= pRect->y1);
	
	if ( (iX < pRect->x1) || (iX > pRect->x2) )
		return FALSE;

	if ( (iY < pRect->y1) || (iY > pRect->y2) )
		return FALSE;
	
	return TRUE;
}

/******************************************************************************
** Function:	DoRectsIntersect()
**
** Description:	Do the two rectangles intersect anywhere?
**
** Parameters:	pRect1		The 1st rectangle.
**				pRect2		The 2nd rectangle.
**
** Returns:		TRUE		If they do.
**				FALSE		If not.
**
*******************************************************************************
*/

bool DoRectsIntersect(Rect* pRect1, Rect* pRect2)
{
	ASSERT(pRect1->x2 >= pRect1->x1);
	ASSERT(pRect1->y2 >= pRect1->y1);
	ASSERT(pRect2->x2 >= pRect2->x1);
	ASSERT(pRect2->y2 >= pRect2->y1);

	if (pRect1->x1 < pRect2->x1)
	{
		if (pRect1->x2 < pRect2->x1)
			return FALSE;
	}
	else
	{
		if (pRect1->x1 > pRect2->x2)
			return FALSE;
	}

	if (pRect1->y1 < pRect2->y1)
	{
		if (pRect1->y2 < pRect2->y1)
			return FALSE;
	}
	else
	{
		if (pRect1->y1 > pRect2->y2)
			return FALSE;
	}
	
	return TRUE;
}

/******************************************************************************
** Function:	GetIntersectRect()
**
** Description:	Find the intersection of 2 rectangles. This assumes that both
**				rectangles are normalised and they do intersect. The returned
**				rect can also be either rect 1 or 2.
**
** Parameters:	pRect1		The 1st rectangle.
**				pRect2		The 2nd rectangle.
**				pDstRect	The destination rectangle.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void GetIntersectRect(Rect* pRect1, Rect* pRect2, Rect* pDstRect)
{
	ASSERT(pRect1->x2 >= pRect1->x1);
	ASSERT(pRect1->y2 >= pRect1->y1);
	ASSERT(pRect2->x2 >= pRect2->x1);
	ASSERT(pRect2->y2 >= pRect2->y1);

	if (pRect1->x1 > pRect2->x1)
		pDstRect->x1 = pRect1->x1;
	else
		pDstRect->x1 = pRect2->x1;
		
	if (pRect1->x2 < pRect2->x2)
		pDstRect->x2 = pRect1->x2;
	else
		pDstRect->x2 = pRect2->x2;

	if (pRect1->y1 > pRect2->y1)
		pDstRect->y1 = pRect1->y1;
	else
		pDstRect->y1 = pRect2->y1;
		
	if (pRect1->y2 < pRect2->y2)
		pDstRect->y2 = pRect1->y2;
	else
		pDstRect->y2 = pRect2->y2;
}
