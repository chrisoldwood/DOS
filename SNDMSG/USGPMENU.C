/******************************************************************************
** (C) Chris Wood 1995.
**
** USGPMENU.C - Show and handle the user/group menu.
**
******************************************************************************* 
*/

#include <nwcalls.h>
#include <nwgfx.h>
#include "helpids.h"

/**** Global Vars. ***********************************************************/
static NWGMENUITEM  TypeList[2];   /* Menu items. */
static char *Users = "User(s)";    /* User menu item. */
static char *Group = "Group";      /* Group menu item. */

extern NWGDIM  wDispWidth;         /* The display width in chars. */
extern NWGDIM  wDispHeight;        /* The display height in chars. */

/**** Function Prototypes. ***************************************************/
NWGNUM UserGroupMenuProc(NWGMSG, NWGNUM, NWGMENUITEMS);

extern void UsersMenu(void);
extern void GroupMenu(void);

/******************************************************************************
** Setup and display the user/group selection menu.
*/
void UserGroupMenu(void)
{
     NWGMENU TypeSelect;                     /* The menu. */

     /* Add the menu labels. */
     TypeList[0].pItem = Users;
     TypeList[1].pItem = Group;
     
     /* Fill in menu details. */
     TypeSelect.iPosType   = MP_RELATIVE;  
     TypeSelect.iSX        = 21;
     TypeSelect.iSY        = 8;   
     TypeSelect.iDX        = 12;
     TypeSelect.iDY        = 6;   
     TypeSelect.fFlags     = MF_BASIC;
     TypeSelect.pTitle     = "To Whom";    
     TypeSelect.tTimeOut   = (NWGTIME) NULL;  
     TypeSelect.fnMenuProc = (NWGMENUPROC) UserGroupMenuProc;
     TypeSelect.iNumItems  = 2; 
     TypeSelect.pItems     = TypeList;    
     TypeSelect.iCurrent   = 0;    
     TypeSelect.iHelpID    = IDH_USRGRP;    

     /* Show Select Queues popup. */
     NWGFXPopupMenu(&TypeSelect);
}

/******************************************************************************
** Callback for user/group selection menu. 
*/
NWGNUM UserGroupMenuProc(NWGMSG wMsg, NWGNUM wSelected, NWGMENUITEMS pItems)
{
     /* Decode menu message. */
     switch(wMsg)
     {
          /* Item selected. */
          case MENU_SELECT:
               if (wSelected == 1)
               {
                    /* Display users menu. */
                    UsersMenu();
               }
               else
               {
                    /* Display groups menu. */
                    GroupMenu();
               }
               break;

          /* Safe. */
          default:
               break;
     }

     /* Don't update. */
     return FALSE;
}
