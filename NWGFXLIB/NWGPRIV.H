/******************************************************************************
** (C) Chris Wood 1994/95.
**
** NWGPRIV.H - Structures and definitions private to the library.
**
******************************************************************************* 
*/

/* First check for previous inclusion. */
#ifndef _NWGPRIV_H
#define _NWGPRIV_H

/* 
** Needed in NWGINIT.C & NWGCORE.C
** Make sure that the data structures are compiled aligned to byte boundries.
*/
typedef struct tagCursor {         /* Cursor position. */
     NWGBYTE   uX;                 /* Column. */
     NWGBYTE   uY;                 /* Row. */
}NWGCURPOS;

typedef struct tagVideoData {      /* BIOS Video data area. */
     NWGBYTE   uMode;              /* Current screen mode. */
     NWGWORD   wNumColumns;        /* Number of columns. */
     NWGWORD   wBufferSize;        /* Size of video buffer. */
     NWGWORD   wBufferOffset;      /* Offset to start of video buffer. */
     NWGCURPOS CursorPos[8];       /* Array of cursor positions for each page. */
     NWGWORD   wCursorMode;        /* Start and end lines of cursor character. */
     NWGBYTE   uActivePage;        /* Currently active page. */
     NWGWORD   wCRTAddress;        /* Address of CRT controller. */
     NWGBYTE   uCRTModeSet;        /* Value of Mode Control Register. */
     NWGBYTE   uCRTPalette;        /* Value of CGA colour select. */
     NWGBYTE   uFiller[29];        /* Dummy array to account for gap in data block. */
     NWGBYTE   uNumRows;           /* Number of rows. */
     NWGWORD   wFontHeight;        /* Height of screen font. */
     NWGBYTE   uScnFlags1;         /* Misc. flags. */
     NWGBYTE   uScnFlags2;         /* Misc. flags. */
     NWGBYTE   uScnFlags3;         /* Misc. flags. */
     NWGBYTE   uDisplayCode;       /* Display Combination Code. */
     NWGFARPTR lpSaveArea;         /* Pointer to BIOS save area. */
}NWGVIDEO, FAR *NWGVIDPTR;

/* _NWGPRIV_H */
#endif
