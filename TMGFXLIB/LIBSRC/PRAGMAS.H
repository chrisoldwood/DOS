/******************************************************************************
** (C) Chris Wood 1997
**
** MODULE:		PRAGMAS.H
** COMPONENT:	Text Mode Graphics Library.
** DESCRIPTION:	Disable annoying Level 4 warnings.
**
*******************************************************************************
*/

/* Check for previous inclusion */
#ifndef PRAGMAS_H
#define PRAGMAS_H

/* Creating/using precompiled header. */
#pragma warning ( disable : 4699 )

/* Unreferenced (Inline) function removed. */
#pragma warning ( disable : 4505 )

/* Conditional expression is constant. */
#pragma warning ( disable : 4127 )

/* Bitfields other than 'int'. */
#pragma warning ( disable : 4214 )

#endif /* PRAGMAS_H */
