/******************************************************************************
** (C) Chris Wood 1994/95.
**
** NWGDLG.H - Structures and definitions for dialogs and their controls.
**
******************************************************************************* 
*/

/* First check for previous inclusion. */
#ifndef _NWGDLG_H
#define _NWGDLG_H

/* Check for C++ compiler. */
#ifdef __cplusplus
extern "C" {
#endif

/**** Callback function. *****************************************************/
typedef NWGBOOL (*NWGDLGPROC)(NWGMSG, NWGNUM, NWGVARPTR);   /* The dialog function. */

/**** Structures. ************************************************************/
typedef struct tagDlgControl {          /* A dialog control. */
     NWGTYPE        iType;              /* The control type. */
     NWGPOS         iXOffset;           /* Control X-Offset; from dialog origin. */
     NWGPOS         iYOffset;           /* Control Y-Offset; from dialog origin. */
     NWGDIM         iWidth;             /* Field width. */
     NWGFLAGS       fFlags;             /* The control flags. */
     NWGSTR         pValue;             /* Pointer to current value. */
     NWGVARPTR      pCustom;            /* Pointer to control specific data. */
} NWGDLGCTRL, * NWGDLGCTRLS;

typedef struct tagDialogBox {           /* A dialog box. */
     NWGTYPE        iPosType;           /* Dialog position specifier. */
     NWGPOS         iSX,iSY;            /* Top-Left corner or Width/Height. */
     NWGPOS         iDX,iDY;            /* Bottom-Right corner or Width/Height. */
     NWGFLAGS       fFlags;             /* Dialog style. */
     NWGSTR         pTitle;             /* Dialog title. */
     NWGTIME        tTimeOut;           /* Timer length. */
     NWGDLGPROC     fnDlgProc;          /* Dialog callback function. */
     NWGNUM         iNumCtls;           /* Number of controls. */
     NWGDLGCTRLS    pControls;          /* List of dialog controls. */
     NWGNUM         iCurrent;           /* Current control selected. */
     NWGNUM         iHelpID;            /* Dialogs' help section. */
} NWGDIALOG;

/**** Menu position specifiers. **********************************************/
#define DP_ABSOLUTE      0         /* Dialog corners as absolute co-ordinates. */
#define DP_RELATIVE      1         /* Dialog origin is relative to screen edges, of width & height. */
#define DP_CENTRED       2         /* Position is centre of the screen, of width & height */

/**** Dialog window style flags. *********************************************/
#define DF_BASIC         0x00      /* Basic dialog style. */
#define DF_STATIC        0x01      /* Dialog shows static info. (single border) */
#define DF_CANCEL        0x02      /* Dialog requires cancel message. */
#define DF_TIMER         0x04      /* Dialog requires timer. */

/**** Dialog control types. **************************************************/
#define DCTL_LABEL       0         /* Static text label. */
#define DCTL_STEXT       1         /* Static text field. */
#define DCTL_ETEXT       2         /* Editable text field. */
#define DCTL_DECVALUE    3         /* Editable decimal number. */
#define DCTL_HEXVALUE    4         /* Editable hex number. */
#define DCTL_TOGGLE      5         /* multi-state toggle. */
#define DCTL_MENU        6         /* Menu. */
#define DCTL_DIALOG      7         /* Dialog. */
#define DCTL_CUSTOM      8         /* Custom control. */

/**** Dialog control flags. **************************************************/
#define DCF_NONE         0x00      /* Control has no flags. */

/* Text label flags. */
#define DCF_TEXTLEFT     0x01      /* Text is left justified. */
#define DCF_TEXTCENTRE   0x02      /* Text is centre justified. */
#define DCF_TEXTRIGHT    0x04      /* Text is right justified. */

/**** Dialog callback messages. **********************************************/
#define DIALOG_CTL_INIT  0         /* Control selected. */
#define DIALOG_CTL_DONE  1         /* Control editing done. */
#define DIALOG_CTL_EXEC  2         /* Handle custom control. */
#define DIALOG_CANCEL    3         /* Dialog is closing. */
#define DIALOG_TIMEOUT   4         /* Timeout occurred. */

/* Check for C++ compiler. */
#ifdef __cplusplus
}
#endif

/* _NWGDLG_H */
#endif
