/******************************************************************************
** (C) Chris Wood 1997
**
** MODULE:		TMGFX.H
** COMPONENT:	Text Mode Graphics Library.
** DESCRIPTION:	Wrapper to include all the core library headers.
**
*******************************************************************************
*/

/* Check for previous inclusion */
#ifndef TMGFX_H
#define TMGFX_H

/******************************************************************************
**
** Standard headers.
**
*******************************************************************************
*/

#include <stdio.h>
#include <malloc.h>

/******************************************************************************
**
** Library specific headers.
**
*******************************************************************************
*/

#include "pragmas.h"		/* Disable certain warnings    */
#include "gfxtypes.h"		/* Basic data types            */
#include "gfxassrt.h"		/* Debugging macros            */
#include "point.h"			/* Point structure             */
#include "cliprect.h"		/* Clipping rect functions     */
#include "display.h"		/* Display functions           */
#include "cursor.h"			/* Cursor functions            */
#include "pixel.h"			/* Single pixel functions      */
#include "line.h"			/* Line drawing functions      */
#include "rect.h"			/* Rectangle drawing functions */
#include "frame.h"			/* Frame drawing functions     */
#include "text.h"			/* Text output functions       */

#endif /* TMGFX_H */
