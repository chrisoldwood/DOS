/******************************************************************************
** (C) Chris Wood 1997
**
** MODULE:		FRAME.C
** COMPONENT:	Text Mode Graphics Library.
** DESCRIPTION:	Frame drawing functions.
**
*******************************************************************************
*/

#include "tmgfx.h"

/******************************************************************************
**
** Local variables.
**
*******************************************************************************
*/

/******************************************************************************
**
** This array stores the characters needed to draw a frame. The frame style is
** used as an index into this array so that we don't have to use alot of if's
** to draw the frame.
*/

byte FrameChars[2][6] = 
{
	{ S_TOPLEFT, S_TOPRIGHT, S_BOTLEFT, S_BOTRIGHT, S_HORZBAR, S_VERTBAR },
	{ D_TOPLEFT, D_TOPRIGHT, D_BOTLEFT, D_BOTRIGHT, D_HORZBAR, D_VERTBAR }
};

/******************************************************************************
**
** This enum is used to reflect the position at which a line joins another.
*/

typedef enum tagJoinPos
{
	JoinAtTop,
	JoinAtBottom,
	JoinOnLeft,
	JoinOnRight
} JoinPos;

/******************************************************************************
** Function:	Frame()
**
** Description:	Draw a frame on a screen using either single or double pixel
**				line characters in the colour specified.
**
** Parameters:	pDC			The display context.
**				pRect		The frame position.
**				eStyle		The frame style.
**				eClr		The colour.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void Frame(DC* pDC, Rect* pRect, FrameStyle eStyle, Colour eClr)
{
	uint iWidth, iHeight;
	
	ASSERT(pDC);
	ASSERT(pRect);

    iWidth  = GetRectWidth(pRect);
    iHeight = GetRectHeight(pRect);

	ASSERT(iWidth >= 2);
	ASSERT(iHeight >= 2);
	
    /* First draw the 4 corners. */
    SetPixel(pDC, pRect->x1, pRect->y1, FrameChars[eStyle][0], eClr);
    SetPixel(pDC, pRect->x2, pRect->y1, FrameChars[eStyle][1], eClr);
    SetPixel(pDC, pRect->x1, pRect->y2, FrameChars[eStyle][2], eClr);
    SetPixel(pDC, pRect->x2, pRect->y2, FrameChars[eStyle][3], eClr);
    
    /* Draw the joining lines if necessary. */
    if (iWidth > 2)
    {
	    HorzLine(pDC, pRect->x1+1, pRect->y1, iWidth-2, FrameChars[eStyle][4], eClr);
	    HorzLine(pDC, pRect->x1+1, pRect->y2, iWidth-2, FrameChars[eStyle][4], eClr);
    }
    
    if (iHeight > 2)
    {
	    VertLine(pDC, pRect->x1, pRect->y1+1, iHeight-2, FrameChars[eStyle][5], eClr);
	    VertLine(pDC, pRect->x2, pRect->y1+1, iHeight-2, FrameChars[eStyle][5], eClr);
	}
}

/******************************************************************************
** Function:	GetFrameJoinChar()
**
** Description:	Get the character which makes the frame line join seamlessly
**				with another frame line.
**
** Parameters:	cChar	The exisiting line character.
**				eJoin	The place where the join is to be.
**
** Returns:		The new character if it can create a join.
**				The same character if no join is possible.
**
*******************************************************************************
*/

static byte GetFrameJoinChar(byte cChar, JoinPos eJoin)
{
	switch(cChar)
	{
		/* Single line characters. */
		
		case S_HORZBAR:
			if (eJoin == JoinAtTop)
				return S_TOPMID;
			else
				return S_BOTMID;
			
		case S_VERTBAR:
			if (eJoin == JoinOnLeft)
				return S_SIDELEFT;
			else
				return S_SIDERIGHT;
			
		case S_TOPMID:
		case S_BOTMID:
		case S_SIDELEFT:
		case S_SIDERIGHT:
			return S_CROSS;

		/* Double line characters. */
		
		case D_HORZBAR:
			if (eJoin == JoinAtTop)
				return D_TOPMID;
			else
				return D_BOTMID;
			
		case D_VERTBAR:
			if (eJoin == JoinOnLeft)
				return D_SIDELEFT;
			else
				return D_SIDERIGHT;
			
		case D_TOPMID:
		case D_BOTMID:
		case D_SIDELEFT:
		case D_SIDERIGHT:
			return D_CROSS;
	
		/* No join possible. */

		default:
			return cChar;
	}
}

/******************************************************************************
** Function:	HorzFrameLine()
**
** Description:	Draw a frame style line in the specified colour. It also
**				matches the end points to make the joins seamless if other
**				frame lines are at the ends.
**
** Parameters:	pDC			The display context.
**				iX, iY		The line start position.
**				iLen		The line length.
**				eStyle		The line style.
**				eClr		The colour.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void HorzFrameLine(DC* pDC, int iX, int iY, uint iLen, FrameStyle eStyle, Colour eClr)
{
	byte cStartChar, cEndChar;
	
	/* Get the line start character. */
	cStartChar = GetPixelChar(pDC, iX, iY);
	
	/* Get the line end character. */
	cEndChar = GetPixelChar(pDC, iX+iLen-1, iY);
	
	HorzLine(pDC, iX, iY, iLen, FrameChars[eStyle][4], eClr);
	
	SetPixelChar(pDC, iX, iY, GetFrameJoinChar(cStartChar, JoinOnLeft));
	SetPixelChar(pDC, iX+iLen-1, iY, GetFrameJoinChar(cEndChar, JoinOnRight));
}

/******************************************************************************
** Function:	VertFrameLine()
**
** Description:	Draw a frame style line in the specified colour. It also
**				matches the end points to make the joins seamless if other
**				frame lines are at the ends.
**
** Parameters:	pDC			The display context.
**				iX, iY		The line start position.
**				iLen		The line length.
**				eStyle		The line style.
**				eClr		The colour.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void VertFrameLine(DC* pDC, int iX, int iY, uint iLen, FrameStyle eStyle, Colour eClr)
{
	byte cStartChar, cEndChar;
	
	/* Get the line start character. */
	cStartChar = GetPixelChar(pDC, iX, iY);
	
	/* Get the line end character. */
	cEndChar = GetPixelChar(pDC, iX, iY+iLen-1);
	
	VertLine(pDC, iX, iY, iLen, FrameChars[eStyle][5], eClr);
	
	SetPixelChar(pDC, iX, iY, GetFrameJoinChar(cStartChar, JoinAtTop));
	SetPixelChar(pDC, iX, iY+iLen-1, GetFrameJoinChar(cEndChar, JoinAtBottom));
}
