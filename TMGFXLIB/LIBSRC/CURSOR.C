/******************************************************************************
** (C) Chris Wood 1997
**
** MODULE:		CURSOR.C
** COMPONENT:	Text Mode Graphics Library.
** DESCRIPTION:	Cursor functions.
**
*******************************************************************************
*/

#include "tmgfx.h"

/******************************************************************************
** Function:	SetCursorPos()
**
** Description:	Sets the position of the cursor for a screen.
**
** Parameters:	pDC		The display context.
**				iX, iY	The new cursor position.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void SetCursorPos(DC* pDC, int iX, int iY)
{
	byte uX, uY, uPage;
	
	ASSERT(pDC);

    /* Adjust position for origin. */
    iX += pDC->ptOrigin.x;
    iY += pDC->ptOrigin.y;
    
    uX    = (byte) iX;
    uY    = (byte) iY;
	uPage = (byte) pDC->iPage;
	
	/* Set the new position. */
	__asm
	{
		mov	dl,uX
		mov	dh,uY
		mov bh,uPage
		mov ah,2
		int 10h
	}
}

/******************************************************************************
** Function:	GetCursorPos()
**
** Description:	Gets the position of the cursor for a display. This returns the
**				position as an absolute position, i.e. it is not affected by
**				the viewport origin.
**
** Parameters:	pDC		The display context.
**				pX, pY	The returned cursor position.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void GetCursorPos(DC* pDC, int* pX, int *pY)
{
	byte uX, uY, uPage;
	
	ASSERT(pDC);

	uPage = (byte) pDC->iPage;

	/* Get the position. */
	__asm
	{
		mov	bh,uPage
		mov ah,3
		int 10h
		mov	uX,dl
		mov	uY,dh
	}

    *pX = uX;
    *pY = uY;
}

/******************************************************************************
** Function:	HideCursor()
**
** Description:	Hide the cursor. For debug builds this places it at the top of
**				the screen so that asserts and run-time errors are still
**				visible. For release builds it is placed off the bottom.
**
** Parameters:	pDC		The display context.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void HideCursor(DC* pDC)
{
	byte uX, uY, uPage;

	ASSERT(pDC);

	uPage = (byte) pDC->iPage;
	
#ifdef _DEBUG
	/* Set at top-left corner. */
	uX = 0;
	uY = 0;
#else
	/* Set just off the bottom. */
	uX = 0;
	uY = pDC->iHeight;
#endif

	/* Set the new position. */
	__asm
	{
		mov	dl,uX
		mov	dh,uY
		mov bh,uPage
		mov ah,2
		int 10h
	}
}
