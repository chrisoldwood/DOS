/******************************************************************************
** (C) Chris Wood 1994/95.
**
** NWGHELP.H - Structures and definitions for the on-line help.
**
******************************************************************************* 
*/

/* First check for previous inclusion. */
#ifndef _NWGHELP_H
#define _NWGHELP_H

/* Check for C++ compiler. */
#ifdef __cplusplus
extern "C" {
#endif

/**** Structures. ***********************************************************/
typedef struct tagHelpString {          /* HelpText line of text. */
     NWGSTR    pString;                 /* A line of text. */
} HELPSTR, * HELPSTRS;
     
typedef struct tagHelpSection {         /* HelpText section. */
     NWGDIM    iWidth;                  /* Help window width. */
     NWGDIM    iHeight;                 /* Help window height. */
     NWGSTR    pTitle;                  /* Help section title. */
     NWGNUM    iNumLines;               /* Number of lines of text in section. */
     HELPSTRS  pText;                   /* Lines of text in section. */
} NWGHELP;

/* Check for C++ compiler. */
#ifdef __cplusplus
}
#endif

/* _NWGHELP_H */
#endif
