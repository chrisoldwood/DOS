/******************************************************************************
** (C) Chris Wood 1995.
**
** WSDLG.H - The workstation dialog controls.
**
******************************************************************************* 
*/

/**** Menu/Dialog sub-controls. **********************************************/
#define WS_CONTROLS      17            /* Number of dialog controls. */

/* Parameter strings indexes. */
#define NODE_NAME        1
#define NODE_ADDRESS     3
#define SHELL_VERSION    5
#define IPX_VERSION      7
#define SPX_VERSION      9
#define IPX_SENT         10
#define IPX_MALFORMED    10
#define SPX_SENT         10
#define SPX_FAILED       10
#define SPX_RECEIVED     10

/**** Dialog controls. *******************************************************/
NWGDLGCTRL WSControls[WS_CONTROLS] = {
/* Type,       X,  Y,  W, Flags,        Value,       Custom data. */
{ DCTL_LABEL,   1,  1, 10, DCF_TEXTLEFT, "Node:",        (NWGVARPTR) NULL },
{ DCTL_STEXT,  11,  1, 10, DCF_NONE,     NULL,           (NWGVARPTR) NULL },
{ DCTL_LABEL,   1,  2, 10, DCF_TEXTLEFT, "Address:",     (NWGVARPTR) NULL },
{ DCTL_STEXT,  11,  2, 10, DCF_NONE,     NULL,           (NWGVARPTR) NULL },
{ DCTL_LABEL,   1,  4, 10, DCF_TEXTLEFT, "Shell: ",      (NWGVARPTR) NULL },
{ DCTL_STEXT,  11,  4, 10, DCF_NONE,     NULL,           (NWGVARPTR) NULL },
{ DCTL_LABEL,   1,  5, 10, DCF_TEXTLEFT, "IPX:   ",      (NWGVARPTR) NULL },
{ DCTL_STEXT,  11,  5, 10, DCF_NONE,     NULL,           (NWGVARPTR) NULL },
{ DCTL_LABEL,   1,  6, 10, DCF_TEXTLEFT, "SPX:   ",      (NWGVARPTR) NULL },
{ DCTL_STEXT,  11,  6, 10, DCF_NONE,     NULL,           (NWGVARPTR) NULL },
{ DCTL_LABEL,   1,  9, 10, DCF_TEXTLEFT, "IPX Packets:", (NWGVARPTR) NULL },
{ DCTL_LABEL,  14,  8, 10, DCF_TEXTLEFT, "Sent",         (NWGVARPTR) NULL },
{ DCTL_LABEL,  20,  8, 10, DCF_TEXTLEFT, "Malformed",    (NWGVARPTR) NULL },
{ DCTL_LABEL,   1, 12, 10, DCF_TEXTLEFT, "SPX Packets:", (NWGVARPTR) NULL },
{ DCTL_LABEL,  14, 11, 10, DCF_TEXTLEFT, "Sent",         (NWGVARPTR) NULL },
{ DCTL_LABEL,  20, 11, 10, DCF_TEXTLEFT, "Failed",       (NWGVARPTR) NULL },
{ DCTL_LABEL,  28, 11, 10, DCF_TEXTLEFT, "Received",     (NWGVARPTR) NULL }
};
