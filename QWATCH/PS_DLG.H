/******************************************************************************
** (C) Chris Wood 1995.
**
** PS_DLG.H - The print server dialog controls.
**
******************************************************************************* 
*/

/**** Menu/Dialog sub-controls. **********************************************/
#define PS_CONTROLS 24              /* Number of print server dialog controls. */

/* Server parameter strings indexes. */
#define SERVERNAME  1
#define PRINTERNAME 3
#define FORMNAME    5
#define STATUS      7
#define TOTCOPIES   9
#define DONECOPIES  11
#define TOTSIZE     13
#define DONESIZE    15
#define COPYBAR     19
#define TOTALBAR    23

/**** Dialog controls. *******************************************************/
NWGDLGCTRL PSControls[PS_CONTROLS] = {
/* Type,       X,  Y,  W, Flags,        Value,       Custom data. */
{ DCTL_LABEL,   1,  1, 10, DCF_TEXTLEFT, "Server:",  (NWGVARPTR) NULL },
{ DCTL_STEXT,  10,  1, 50, DCF_NONE,     NULL,       (NWGVARPTR) NULL },
{ DCTL_LABEL,   1,  2, 10, DCF_TEXTLEFT, "Printer:", (NWGVARPTR) NULL },
{ DCTL_STEXT,  10,  2, 50, DCF_NONE,     NULL,       (NWGVARPTR) NULL },
{ DCTL_LABEL,   1,  3, 10, DCF_TEXTLEFT, "Form:",    (NWGVARPTR) NULL },
{ DCTL_STEXT,  10,  3, 50, DCF_NONE,     NULL,       (NWGVARPTR) NULL },
{ DCTL_LABEL,   1,  4, 10, DCF_TEXTLEFT, "Status:",  (NWGVARPTR) NULL },
{ DCTL_STEXT,  10,  4, 50, DCF_NONE,     NULL,       (NWGVARPTR) NULL },

{ DCTL_LABEL,   1,  6, 10, DCF_TEXTLEFT, "Copies:",  (NWGVARPTR) NULL },
{ DCTL_STEXT,  10,  6, 10, DCF_NONE,     NULL,       (NWGVARPTR) NULL },
{ DCTL_LABEL,  20,  6, 10, DCF_TEXTLEFT, "Done:",    (NWGVARPTR) NULL },
{ DCTL_STEXT,  27,  6, 10, DCF_NONE,     NULL,       (NWGVARPTR) NULL },
{ DCTL_LABEL,   1,  7, 10, DCF_TEXTLEFT, "Bytes:",   (NWGVARPTR) NULL },
{ DCTL_STEXT,  10,  7, 10, DCF_NONE,     NULL,       (NWGVARPTR) NULL },
{ DCTL_LABEL,  20,  7, 10, DCF_TEXTLEFT, "Done:",    (NWGVARPTR) NULL },
{ DCTL_STEXT,  27,  7, 10, DCF_NONE,     NULL,       (NWGVARPTR) NULL },

{ DCTL_LABEL,   3,  9, 10, DCF_TEXTLEFT, "0%",       (NWGVARPTR) NULL },
{ DCTL_LABEL,  16,  9, 10, DCF_TEXTLEFT, "Copy",     (NWGVARPTR) NULL },
{ DCTL_LABEL,  31,  9, 10, DCF_TEXTLEFT, "100%",     (NWGVARPTR) NULL },
{ DCTL_STEXT,   3, 10, 10, DCF_TEXTLEFT, NULL,       (NWGVARPTR) NULL },

{ DCTL_LABEL,   3, 12, 10, DCF_TEXTLEFT, "0%",       (NWGVARPTR) NULL },
{ DCTL_LABEL,  16, 12, 10, DCF_TEXTLEFT, "Total",    (NWGVARPTR) NULL },
{ DCTL_LABEL,  31, 12, 10, DCF_TEXTLEFT, "100%",     (NWGVARPTR) NULL },
{ DCTL_STEXT,   3, 13, 10, DCF_TEXTLEFT, NULL,       (NWGVARPTR) NULL },
};
