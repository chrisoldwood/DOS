/******************************************************************************
** (C) Chris Wood 1995.
**
** DEBUG.H - Debugging utils header.
**
*******************************************************************************
*/

/* Check for previous inclusion. */
#ifndef _DEBUG_H_
#define _DEBUG_H_

/* Check the build type. */
#ifdef _DEBUG

/* My assert functions' prototypes. */
extern void MyAssert(PSTR, int, BOOL);

#define ASSERT(bExpr)              MyAssert(__FILE__, __LINE__, (bExpr))

#define IsValidPtr(pMem)			(((pMem) > NULL) ? TRUE : FALSE)

#else /* Is NDEBUG. */

#define ASSERT(bExpr)
#define TRACE(szMsg)

#define IsValidPtr(pMem)

#endif /* _DEBUG. */

#endif /* _DEBUG_H_ */
