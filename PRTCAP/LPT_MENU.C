/******************************************************************************
** (C) Chris Wood 1995.
**
** LPT_MENU.C - Show and handle the LPT port menu.
**
******************************************************************************* 
*/

#include <nwcalls.h>
#include <nwgfx.h>
#include "helpids.h"

/**** Global Vars. ***********************************************************/
static NWGMENUITEM PortList[3];                       /* Menu items list. */

/**** Function Prototypes. ***************************************************/
NWGNUM LPTMenuProc(NWGMSG, NWGNUM, NWGMENUITEMS);

extern void SettingsDlg(int iPort);

/******************************************************************************
** Setup and display the LPT port selection menu.
*/
void LPTMenu(void)
{
     NWGMENU   LPTPort;                      /* The menu. */

     /* Add the server names. */
     PortList[0].pItem = "LPT 1";
     PortList[1].pItem = "LPT 2";
     PortList[2].pItem = "LPT 3";
     
     /* Fill in menu details. */
     LPTPort.iPosType   = MP_CENTRED;  
     LPTPort.iSX        = 16;
     LPTPort.iSY        = 7;   
     LPTPort.fFlags     = MF_BASIC | MF_CANCEL;    
     LPTPort.pTitle     = "Select Port";    
     LPTPort.tTimeOut   = (NWGTIME) NULL;  
     LPTPort.fnMenuProc = (NWGMENUPROC) LPTMenuProc;
     LPTPort.iNumItems  = 3; 
     LPTPort.pItems     = PortList;    
     LPTPort.iCurrent   = 0;    
     LPTPort.iHelpID    = IDH_LPTPORT;    

     /* Show Select Queues popup. */
     NWGFXPopupMenu(&LPTPort);
}

/******************************************************************************
** Callback for LPT port menu. 
*/
NWGNUM LPTMenuProc(NWGMSG wMsg, NWGNUM wSelected, NWGMENUITEMS pItems)
{
     /* Decode menu message. */
     switch(wMsg)
     {
          /* Item selected. */
          case MENU_SELECT:
               SettingsDlg(wSelected);
               break;

          /* User wishes to cancel menu. */
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
