/******************************************************************************
** (C) Chris Wood 1994/95.
**
** NWGTYPES.H - Global data types and definitions.
**
******************************************************************************* 
*/

/* First check for previous inclusion. */
#ifndef _NWGTYPES_H
#define _NWGTYPES_H

/**** NetWare data types used. ***********************************************/
#ifndef NWCCODE
#define NWCCODE     unsigned short
#endif

/**** Portability. ***********************************************************/
#if defined(__TURBOC__) && !defined(__BORLANDC__)
/* For Turbo C but not Borland C++. */
#define FAR    far
#else
/* For everyone else. */
#define FAR    _far
#endif

/**** Data type aliases. *****************************************************/
typedef unsigned char         NWGBYTE;       /* 8-bit byte. */
typedef char                  NWGCHAR;       /* Character. */
typedef unsigned short        NWGWORD;       /* 16-bit word. */
typedef unsigned long         NWGLONG;       /* 32-bit long. */
typedef int                   NWGBOOL;       /* Boolean. */
typedef char *                NWGSTR;        /* Pointer to a string. */
typedef int                   NWGPOS;        /* Co-ordinate. */
typedef int                   NWGDIM;        /* Dimension. */
typedef int                   NWGNUM;        /* Signed number. */
typedef int                   NWGMSG;        /* Message. */
typedef unsigned char         NWGCOLOUR;     /* Colour. */
typedef unsigned short        NWGFLAGS;      /* 16-bit flags. */
typedef int                   NWGTYPE;       /* Object type/style. */
typedef unsigned short        NWGKEY;        /* Keycode in my format. */
typedef int                   NWGTIME;       /* Time in seconds. */
typedef unsigned char FAR *   NWGSCNPTR;     /* Pointer to screen. */
typedef unsigned char FAR *   NWGFARBUF;     /* Pointer to FAR memory. */
typedef void *                NWGVARPTR;     /* Pointer to variable data types. */
typedef void FAR *            NWGFARPTR;     /* Far pointer to variable data types. */

/**** Other useful defs. *****************************************************/
#ifndef VOID
#define VOID        void
#endif

#ifndef NULL
#define NULL        0
#endif

#ifndef TRUE
#define TRUE        1
#endif

#ifndef FALSE
#define FALSE       0
#endif

/* _NWGTYPES_H */
#endif
