/******************************************************************************
** (C) Chris Wood 1995.
**
** CALLMENU.C - Show and handle the call type menu.
**
******************************************************************************* 
*/

#include <nwcalls.h>
#include <nwgfx.h>
#include "helpids.h"

/**** Global Vars. ***********************************************************/
static NWGMENUITEM  TypeList[2];             /* Menu items. */
static char *MakeStr   = "Make Call";        /* Initiate call menu item. */
static char *AcceptStr = "Accept Call";      /* Accept another call menu item. */

/**** Function Prototypes. ***************************************************/
NWGNUM CallMenuProc(NWGMSG, NWGNUM, NWGMENUITEMS);

extern char * UserMenu(void);
extern void MakeCall(char *);
extern void AcceptCall(char *);

/******************************************************************************
** Setup and display the call type menu.
*/
void CallMenu(void)
{
     NWGMENU CallSelect;                     /* The menu. */

     /* Add the menu labels. */
     TypeList[0].pItem = MakeStr;
     TypeList[1].pItem = AcceptStr;
     
     /* Fill in menu details. */
     CallSelect.iPosType   = MP_CENTRED;  
     CallSelect.iSX        = 16;
     CallSelect.iSY        = 6;   
     CallSelect.fFlags     = MF_BASIC | MF_CANCEL;
     CallSelect.pTitle     = "Call Type";    
     CallSelect.tTimeOut   = (NWGTIME) NULL;  
     CallSelect.fnMenuProc = (NWGMENUPROC) CallMenuProc;
     CallSelect.iNumItems  = 2; 
     CallSelect.pItems     = TypeList;    
     CallSelect.iCurrent   = 0;    
     CallSelect.iHelpID    = IDH_CALL;    

     /* Show Select Queues popup. */
     NWGFXPopupMenu(&CallSelect);
}

/******************************************************************************
** Callback for call type menu. 
*/
NWGNUM CallMenuProc(NWGMSG wMsg, NWGNUM wSelected, NWGMENUITEMS pItems)
{
     char *UserName;                          /* User to call. */
     
     /* Decode menu message. */
     switch(wMsg)
     {
          /* Item selected. */
          case MENU_SELECT:
               /* Select a user. */
               UserName = UserMenu();

               /* Check for cancel. */
               if (!UserName)
                    return FALSE;

               /* Disable exit menu and help.*/
               NWGFXAllowExit(FALSE);
               NWGFXAllowHelp(FALSE);
               
               /* Check choice. */
               if (wSelected == 1)
               {
                    /* Start the call. */
                    MakeCall(UserName);
               }
               else
               {
                    /* Accept call. */
                    AcceptCall(UserName);
               }

               /* Re-enable exit menu and help.*/
               NWGFXAllowExit(TRUE);
               NWGFXAllowHelp(TRUE);
               break;

          /* User wishes to exit. */
          case MENU_CANCEL:
               return NWGFXExitMenu();
               break;

          /* Safe. */
          default:
               break;
     }

     /* Don't update. */
     return FALSE;
}
