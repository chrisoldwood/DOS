/******************************************************************************
** (C) Chris Wood 1994/95.
**
** NWGMENU.H - Structures and definitions for menus.
**
******************************************************************************* 
*/

/* First check for previous inclusion. */
#ifndef _NWGMENU_H
#define _NWGMENU_H

/* Check for C++ compiler. */
#ifdef __cplusplus
extern "C" {
#endif

/**** Callback functions. ****************************************************/
typedef VOID (*NWGABORTPROC)(VOID);                    /* The exit function. */
typedef NWGBOOL (*NWGMENUPROC)(NWGMSG, NWGNUM, NWGVARPTR);   
                                                       /* The menu function. */

/**** Structures. ************************************************************/
typedef struct tagMenuItem {       /* A menu item. */
     NWGBOOL        bTagged;       /* The 'tagged' flag. */
     NWGSTR         pItem;         /* The menu string. */
} NWGMENUITEM, * NWGMENUITEMS;

typedef struct tagMenu {           /* A menu. */
     NWGTYPE        iPosType;      /* Menu position specifier. */
     NWGPOS         iSX,iSY;       /* Top-Left corner or Width/Height. */
     NWGPOS         iDX,iDY;       /* Bottom-Right corner or Width/Height. */
     NWGFLAGS       fFlags;        /* Menu style flags. */
     NWGSTR         pTitle;        /* The menu title. */
     NWGTIME        tTimeOut;      /* Timer length. */
     NWGMENUPROC    fnMenuProc;    /* The callback function. */
     NWGNUM         iNumItems;     /* Number of menu items. */
     NWGMENUITEMS   pItems;        /* Pointer to the list of items. */
     NWGNUM         iCurrent;      /* Current item selected. */
     NWGNUM         iHelpID;       /* Menus' help section. */
} NWGMENU;     

/**** Menu position specifiers. **********************************************/
#define MP_ABSOLUTE      0         /* Menu corners as absolute co-ordinates. */
#define MP_RELATIVE      1         /* Menu origin is relative to screen edges, of width & height. */
#define MP_CENTRED       2         /* Position is centre of the screen, of width & height */

/**** Menu style flags. ******************************************************/
#define MF_BASIC         0x00      /* Basic menu style. */
#define MF_EXTENDED      0x01      /* Allow multiple selections. */
#define MF_CANCEL        0x02      /* Send cancel message. */
#define MF_INSERT        0x04      /* Send insert message. */
#define MF_DELETE        0x08      /* Send delete message. */
#define MF_TIMER         0x10      /* Send timeout message. */

/**** Menu callback messages. ************************************************/
#define MENU_SELECT      0         /* Item(s) have been selected. */
#define MENU_CANCEL      1         /* User wishes to cancel. */
#define MENU_INSERT      2         /* Insert key pressed. */
#define MENU_DELETE      3         /* Delete key pressed. */
#define MENU_TIMEOUT     4         /* Timeout occurred. */

/* Check for C++ compiler. */
#ifdef __cplusplus
}
#endif

/* _NWGMENU_H */
#endif
