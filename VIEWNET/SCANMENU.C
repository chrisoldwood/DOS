/******************************************************************************
** (C) Chris Wood 1995.
**
** SCANMENU.C - The initial menu to start the scan.
**
******************************************************************************* 
*/

#include <nwgfx.h>
#include "helpids.h"

/**** Global Vars. ***********************************************************/
static NWGMENUITEM  MenuList[1];             /* Menu items. */
static char *ScanNet = "Scan";               /* Start scanning the network. */

/**** Function Prototypes. ***************************************************/
NWGNUM ScanMenuProc(NWGMSG, NWGNUM, NWGMENUITEMS);

extern void DoScan(void);

/******************************************************************************
** Setup and display the initial menu.
*/
void ScanMenu(void)
{
     NWGMENU ScanMenu;                     /* The menu. */

     /* Add the menu labels. */
     MenuList[0].pItem = ScanNet;
     
     /* Fill in menu details. */
     ScanMenu.iPosType   = MP_CENTRED;  
     ScanMenu.iSX        = 20;
     ScanMenu.iSY        = 5;   
     ScanMenu.fFlags     = MF_BASIC | MF_CANCEL;
     ScanMenu.pTitle     = "Network Options";    
     ScanMenu.tTimeOut   = (NWGTIME) NULL;  
     ScanMenu.fnMenuProc = (NWGMENUPROC) ScanMenuProc;
     ScanMenu.iNumItems  = 1; 
     ScanMenu.pItems     = MenuList;    
     ScanMenu.iCurrent   = 0;    
     ScanMenu.iHelpID    = IDH_SCAN;    

     /* Show the menu. */
     NWGFXPopupMenu(&ScanMenu);
}

/******************************************************************************
** Callback for scan menu. 
*/
NWGNUM ScanMenuProc(NWGMSG wMsg, NWGNUM wSelected, NWGMENUITEMS pItems)
{
     /* Decode menu message. */
     switch(wMsg)
     {
          /* Item selected. */
          case MENU_SELECT:
               /* Lets go. */
               DoScan();
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
