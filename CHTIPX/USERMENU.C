/******************************************************************************
** (C) Chris Wood 1995.
**
** USERMENU.C - Show and handle the user select menu.
**
******************************************************************************* 
*/

#include <malloc.h>
#include <nwcalls.h>
#include <nwgfx.h>
#include "helpids.h"
#include "users.h"

/**** Global Vars. ***********************************************************/
static int iNumUsers;                        /* Total users. */

/**** Function Prototypes. ***************************************************/

/******************************************************************************
** Setup and display the users selection menu.
*/
char * UserMenu(void)
{
     NWGMENU UserSelect;                     /* The menu. */
     NWGMENUITEMS MenuList;                  /* Menu items. */
     int iLoop;                              /* User counter. */
     NWCONN_HANDLE hServer;                  /* Primary server ID. */
     int iSelected;                          /* User selected. */

     /* Show wait message. */
     NWGFXWaitMessage(TRUE);

     /* Get the default connection ID. */
     NWGetPrimaryConnectionID(&hServer);
     
     /* Get the users on the default server. */
     iNumUsers = GetUsers(hServer);
     
     /* Check we found users. */
     if (!iNumUsers)
     {
          /* Hide wait message. */
          NWGFXWaitMessage(FALSE);

          /* Show error message. */
          NWGFXInfoMessage("No Users Found");
          return NULL;
     }
     
     /* Allocate memory for menu items. */
     MenuList = (NWGMENUITEMS) malloc(sizeof(NWGMENUITEM) * iNumUsers);
     if (!MenuList)
     {
          /* Hide wait message. */
          NWGFXWaitMessage(FALSE);

          /* Show error message. */
          NWGFXErrorMessage("Malloc failure!");
          return NULL;
     }

     /* Set menu items pointer to queue info string. */
     for (iLoop=0; iLoop < iNumUsers; iLoop++)
          MenuList[iLoop].pItem = GetUserName(iLoop);

     /* Fill in menu details. */
     UserSelect.iPosType   = MP_CENTRED;  
     UserSelect.iSX        = 30;
     UserSelect.iSY        = 20;   
     UserSelect.fFlags     = MF_BASIC;
     UserSelect.pTitle     = "Select User";    
     UserSelect.tTimeOut   = (NWGTIME) NULL;  
     UserSelect.fnMenuProc = (NWGMENUPROC) NULL;
     UserSelect.iNumItems  = iNumUsers; 
     UserSelect.pItems     = MenuList;
     UserSelect.iCurrent   = 0;    
     UserSelect.iHelpID    = IDH_USER;

     /* Hide wait message. */
     NWGFXWaitMessage(FALSE);

     /* Show Select Queues popup. */
     iSelected = NWGFXPopupMenu(&UserSelect);
     
     /* Check for cancel. */
     if (!iSelected)
          return NULL;
          
     /* Convert to index. */
     iSelected--;
     
     return GetUserName(iSelected);
}
