/******************************************************************************
** (C) Chris Wood 1995.
**
** LPT_DLG.H - The LPT dialog controls.
**
******************************************************************************* 
*/

/**** Menu/Dialog sub-controls. **********************************************/
#define LPT_CONTROLS     24             /* Number of dialog controls. */

/* Parameter strings indexes. */
#define STATUS      1
#define SERVERNAME  3
#define QUEUENAME   5
#define FORMNAME    7
#define SHOWTABS    9
#define TABSIZE     11
#define SHOWBANNER  13
#define BANNERNAME  15
#define NUMCOPIES   17
#define FORMFEED    19
#define PAGEHEIGHT  21
#define PAGEWIDTH   23

/* The expand tabs menu. */
NWGMENUITEM SetTabs[2] = { { FALSE, "No"  }, 
                           { FALSE, "Yes" } };

NWGMENU TabsMenu = { MP_CENTRED, 15, 6, 0, 0, MF_BASIC, "Expand Tabs", 0, NULL, 
                         2, SetTabs, 0, IDH_SHOWTABS};

/* The show banner menu. */
NWGMENUITEM SetBanner[2] = { { FALSE, "No"  }, 
                             { FALSE, "Yes" } };

NWGMENU BannerMenu = { MP_CENTRED, 16, 6, 0, 0, MF_BASIC, "Print Banner", 0, NULL, 
                         2, SetBanner, 0, IDH_SHOWBANNER};

/* The form feed menu. */
NWGMENUITEM SetFeed[2] = { { FALSE, "No"  }, 
                           { FALSE, "Yes" } };

NWGMENU FeedMenu = { MP_CENTRED, 18, 6, 0, 0, MF_BASIC, "Send Form Feed", 0, NULL, 
                         2, SetFeed, 0, IDH_FORMFEED};

/* The form menu. */
NWGMENU FormMenu = { MP_CENTRED, 0, 0, 0, 0, MF_BASIC, "Set Form", 0, NULL, 
                         0, NULL, 0, IDH_FORMNAME };

/**** Dialog controls. *******************************************************/
NWGDLGCTRL LPTControls[LPT_CONTROLS] = {
/* Type,       X,  Y,  W, Flags,        Value,       Custom data. */
{ DCTL_LABEL,   1,  1, 10, DCF_TEXTLEFT, "Status:", (NWGVARPTR) NULL },
{ DCTL_CUSTOM,  9,  1, 20, DCF_NONE,     NULL,      (NWGVARPTR) NULL },
{ DCTL_LABEL,   1,  3, 10, DCF_TEXTLEFT, "Server:", (NWGVARPTR) NULL },
{ DCTL_STEXT,   9,  3, 20, DCF_NONE,     NULL,      (NWGVARPTR) NULL },
{ DCTL_LABEL,   1,  4, 10, DCF_TEXTLEFT, "Queue:",  (NWGVARPTR) NULL },
{ DCTL_CUSTOM,  9,  4, 20, DCF_NONE,     NULL,      (NWGVARPTR) NULL },
{ DCTL_LABEL,   1,  5, 10, DCF_TEXTLEFT, "Form:",   (NWGVARPTR) NULL },
{ DCTL_CUSTOM,  9,  5, 20, DCF_NONE,     NULL,      (NWGVARPTR) NULL },

{ DCTL_LABEL,   1,  7, 10, DCF_TEXTLEFT, "Tabs:",   (NWGVARPTR) NULL },
{ DCTL_MENU,    9,  7,  5, DCF_NONE,     NULL,      (NWGVARPTR) &TabsMenu },
{ DCTL_LABEL,  15,  7, 10, DCF_TEXTLEFT, "Size:",   (NWGVARPTR) NULL },
{ DCTL_STEXT,  22,  7,  5, DCF_NONE,     NULL,      (NWGVARPTR) NULL },

{ DCTL_LABEL,   1,  9, 10, DCF_TEXTLEFT, "Banner:", (NWGVARPTR) NULL },
{ DCTL_MENU,    9,  9,  5, DCF_NONE,     NULL,      (NWGVARPTR) &BannerMenu },
{ DCTL_LABEL,  15,  9, 10, DCF_TEXTLEFT, "Name:",   (NWGVARPTR) NULL },
{ DCTL_STEXT,  22,  9,  5, DCF_NONE,     NULL,      (NWGVARPTR) NULL },

{ DCTL_LABEL,   1, 11, 10, DCF_TEXTLEFT, "Copies:", (NWGVARPTR) NULL },
{ DCTL_STEXT,   9, 11,  5, DCF_NONE,     NULL,      (NWGVARPTR) NULL },
{ DCTL_LABEL,  15, 11, 10, DCF_TEXTLEFT, "FFeed:",  (NWGVARPTR) NULL },
{ DCTL_MENU,   22, 11,  5, DCF_NONE,     NULL,      (NWGVARPTR) &FeedMenu },

{ DCTL_LABEL,   1, 13, 10, DCF_TEXTLEFT, "Lines:",  (NWGVARPTR) NULL },
{ DCTL_STEXT,   9, 13,  5, DCF_NONE,     NULL,      (NWGVARPTR) NULL },
{ DCTL_LABEL,  15, 13, 10, DCF_TEXTLEFT, "Width:",  (NWGVARPTR) NULL },
{ DCTL_STEXT,  22, 13,  5, DCF_NONE,     NULL,      (NWGVARPTR) NULL }
};
