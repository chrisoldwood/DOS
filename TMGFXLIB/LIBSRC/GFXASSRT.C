/******************************************************************************
** (C) Chris Wood 1997
**
** MODULE:		GFXASSRT.C
** COMPONENT:	Text Mode Graphics Library.
** DESCRIPTION:	Debugging output functions.
**
*******************************************************************************
*/

#include "tmgfx.h"
#include <conio.h>

/******************************************************************************
** Function:	TMGfxAssert()
**
** Description:	This is called when an ASSERT fails. It displays a message with
**				details about why and where the assert failed. It also asks
**				whether you want to break.
**
** Parameters:	pszExpression	A string representation of the ASSERT.
**				pszFile			The file in which it occurs.
**				iLine			The line where it occurs.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void TMGfxAssert(const char* pszExpression, const char* pszFile, uint iLine)
{
	/* Flush the keyboard buffer. */
	while (_kbhit())
		_getch();

	/* Display the assert. */
	printf("ASSERT Failed!\nExpr:\t%s\nFile:\t%s\nLine:\t%d\n\nDebug (y/n)?", pszExpression, pszFile, iLine);
	
	/* Wait for keypress. */
	while (_kbhit());
	
	/* Break? */
	if (_getch() == 'y')
	{
		__asm { int 3 }
	}
}
