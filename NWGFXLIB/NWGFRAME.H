/******************************************************************************
** (C) Chris Wood 1995.
**
** NWGFRAME.H - Structures and definitions for window frames.
**
******************************************************************************* 
*/

/* First check for previous inclusion. */
#ifndef _NWGFRAME_H
#define _NWGFRAME_H

/* Check for C++ compiler. */
#ifdef __cplusplus
extern "C" {
#endif

/**** Structure. ************************************************************/
typedef struct tagFrame {          /* A window frame. */
     NWGTYPE        iPosType;      /* Frame position specifier. */
     NWGPOS         iSX,iSY;       /* Top-Left corner or Width/Height. */
     NWGPOS         iDX,iDY;       /* Bottom-Right corner or Width/Height. */
     NWGFLAGS       fFlags;        /* Frame style flags. */
     NWGSTR         pTitle;        /* The frame title. */
     NWGFARBUF      lpFrameBuf;    /* Screen contents buffer (Used internally). */
} NWGFRAME, *NWGFRAMEPTR;

/**** Menu position specifiers. **********************************************/
#define FP_ABSOLUTE      0         /* Frame corners as absolute co-ordinates. */
#define FP_RELATIVE      1         /* Frame origin is relative to screen edges, of width & height. */
#define FP_CENTRED       2         /* Position is centre of the screen, of width & height */

/**** Menu style flags. ******************************************************/
#define FF_BASIC         0x00      /* Basic frame style, single border no title. */
#define FF_DOUBLE        0x01      /* Double line border. */
#define FF_TITLE         0x02      /* Frame has title. */

/* Check for C++ compiler. */
#ifdef __cplusplus
}
#endif

/* _NWGFRAME_H */
#endif
