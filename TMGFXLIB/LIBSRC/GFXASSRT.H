/******************************************************************************
** (C) Chris Wood 1997
**
** MODULE:		GFXASSRT.H
** COMPONENT:	Text Mode Graphics Library.
** DESCRIPTION:	Debugging macros.
**
*******************************************************************************
*/

/* Check for previous inclusion */
#ifndef GFXASSRT_H
#define GFXASSRT_H

/* Debug version? */
#ifdef _DEBUG

/******************************************************************************
**
**	These are the implmentations for a debug version.
**
*******************************************************************************
*/

/* Prototype for the actual ASSERT function. */
void TMGfxAssert(const char* pszExpression, const char* pszFile, uint iLine);

/* Assert macro. */
#define ASSERT(x)		if (x) {} else TMGfxAssert(#x, __FILE__, __LINE__)

#else /* NDEBUG */

/******************************************************************************
**
**	These are the implmentations for a release version.
**
*******************************************************************************
*/

#define ASSERT(x)

#endif /* _DEBUG */

#endif /* GFXASSRT_H */
