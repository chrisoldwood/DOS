/******************************************************************************
** (C) Chris Wood 1995.
**
** BRDGMENU.C - The bridge menu.
**
******************************************************************************* 
*/

#include <nwcalls.h>
#include <nwgfx.h>
#include "helpids.h"
#include "nodes.h"

/**** Global Vars. ***********************************************************/
static NWGMENUITEM  MenuList[1];             /* Menu items. */
static char *Option1 = "Option1";            /* . */

/**** Function Prototypes. ***************************************************/
NWGNUM BridgeMenuProc(NWGMSG, NWGNUM, NWGMENUITEMS);

/******************************************************************************
** Display the network list.
*/
void BridgeMenu(PNETNODE pNode)
{
     NWGMENU Menu;

     /* Add the menu labels. */
     MenuList[0].pItem = Option1;
     
     /* Fill in menu details. */
     Menu.iPosType   = MP_CENTRED;  
     Menu.iSX        = 20;
     Menu.iSY        = 5;   
     Menu.fFlags     = MF_BASIC | MF_CANCEL;
     Menu.pTitle     = "Other Networks";    
     Menu.tTimeOut   = (NWGTIME) NULL;  
     Menu.fnMenuProc = (NWGMENUPROC) BridgeMenuProc;
     Menu.iNumItems  = 1; 
     Menu.pItems     = MenuList;    
     Menu.iCurrent   = 0;    
     Menu.iHelpID    = IDH_SCAN;    

     /* Show the menu. */
     //NWGFXPopupMenu(&BridgeMenu);
     NWGFXInfoMessage("Cant access other networks yet!");
}

/******************************************************************************
** Callback for bridge menu. 
*/
NWGNUM BridgeMenuProc(NWGMSG wMsg, NWGNUM wSelected, NWGMENUITEMS pItems)
{
     /* Decode menu message. */
     switch(wMsg)
     {
          /* Item selected. */
          case MENU_SELECT:
               break;

          /* Safe. */
          default:
               break;
     }

     /* Don't update. */
     return FALSE;
}
