/******************************************************************************
** (C) Chris Wood 1995.
**
** PJ_DLG.H - The print job dialog controls.
**
******************************************************************************* 
*/

/**** Menu/Dialog sub-controls. **********************************************/
/* The status menu. */
NWGMENUITEM StatSet[3] = { { FALSE, "Ready" }, 
                           { FALSE, "Hold (User)" },
                           { FALSE, "Hold (Operator)" } };

NWGMENU StatMenu = { MP_CENTRED, 20, 7, 0, 0, MF_BASIC, "Set Status", 0, NULL, 
                         3, StatSet, 0, 3 };

/* The form menu. */
NWGMENU FormMenu = { MP_CENTRED, 0, 0, 0, 0, MF_BASIC, "Set Form", 0, NULL, 
                         0, NULL, 0, 3 };

/* The print server dialog. */

/* The position menu. */
NWGMENUITEM PosSet[2] = { { FALSE, "First" }, 
                          { FALSE, "Last"  } };

NWGMENU PosMenu = { MP_CENTRED, 16, 6, 0, 0, MF_BASIC, "Set Position", 0, NULL, 
                         2, PosSet, 0, 3 };

#define PJ_CONTROLS 22        /* Number of print job dialog controls. */

/* Job parameter strings indexes. */
#define JOBUSER     1
#define JOBTITLE    3
#define JOBFORM     5
#define JOBSIZE     7
#define JOBSTATUS   9
#define JOBSERVER   11
#define JOBTARGET   13
#define JOBPOS      15
#define JOBNUMBER   17
#define JOBDATE     19
#define JOBTIME     21

/**** Dialog controls. *******************************************************/
NWGDLGCTRL PJControls[PJ_CONTROLS] = {
/* Type,       X,  Y,  W, Flags,        Value,       Custom data. */
{ DCTL_LABEL,  1,  1, 10, DCF_TEXTLEFT, "User:",     (NWGVARPTR) NULL },
{ DCTL_STEXT, 11,  1, 50, DCF_NONE,     NULL,        (NWGVARPTR) NULL },
{ DCTL_LABEL,  1,  2, 10, DCF_TEXTLEFT, "Title:",    (NWGVARPTR) NULL },
{ DCTL_STEXT, 11,  2, 50, DCF_NONE,     NULL,        (NWGVARPTR) NULL },

{ DCTL_LABEL,  1,  3, 10, DCF_TEXTLEFT, "Form:",     (NWGVARPTR) NULL },
{ DCTL_MENU,  11,  3,  3, DCF_NONE,     NULL,        (NWGVARPTR) &FormMenu },
{ DCTL_LABEL, 32,  3, 10, DCF_TEXTLEFT, "Size:",     (NWGVARPTR) NULL },
{ DCTL_STEXT, 40,  3, 10, DCF_NONE,     NULL,        (NWGVARPTR) NULL },

{ DCTL_LABEL,  1,  5, 10, DCF_TEXTLEFT, "Status:",   (NWGVARPTR) NULL },
{ DCTL_MENU,  11,  5,  6, DCF_NONE,     NULL,        (NWGVARPTR) &StatMenu },
{ DCTL_LABEL,  1,  6, 10, DCF_TEXTLEFT, "Server:",   (NWGVARPTR) NULL },
{ DCTL_CUSTOM,11,  6, 20, DCF_NONE,     NULL,        (NWGVARPTR) NULL },
{ DCTL_LABEL, 32,  6, 10, DCF_TEXTLEFT, "Target:",   (NWGVARPTR) NULL },
{ DCTL_STEXT, 40,  6, 20, DCF_NONE,     NULL,        (NWGVARPTR) NULL },

{ DCTL_LABEL,  1,  8, 10, DCF_TEXTLEFT, "Position:", (NWGVARPTR) NULL },
{ DCTL_MENU,  11,  8,  3, DCF_NONE,     NULL,        (NWGVARPTR) &PosMenu },
{ DCTL_LABEL, 32,  8, 10, DCF_TEXTLEFT, "Number:",   (NWGVARPTR) NULL },
{ DCTL_STEXT, 40,  8,  5, DCF_NONE,     NULL,        (NWGVARPTR) NULL },
{ DCTL_LABEL,  1,  9, 10, DCF_TEXTLEFT, "Date:",     (NWGVARPTR) NULL },
{ DCTL_STEXT, 11,  9,  8, DCF_NONE,     NULL,        (NWGVARPTR) NULL },
{ DCTL_LABEL, 32,  9, 10, DCF_TEXTLEFT, "Time:",     (NWGVARPTR) NULL },
{ DCTL_STEXT, 40,  9,  8, DCF_NONE,     NULL,        (NWGVARPTR) NULL }
};
