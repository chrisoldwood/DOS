/******************************************************************************
** (C) Chris Wood 1995.
**
** FS_MENU.C - Show and handle the file server selection menu.
**
******************************************************************************* 
*/

#include <malloc.h>
#include <nwcalls.h>
#include <nwgfx.h>
#include "fileserv.h"
#include "helpids.h"

/**** Global Vars. ***********************************************************/
extern NWGDIM  wDispWidth;         /* The display width in chars. */
extern NWGDIM  wDispHeight;        /* The display height in chars. */

/**** Function Prototypes. ***************************************************/
NWGNUM FileServerMenuProc(NWGMSG, NWGNUM, NWGMENUITEMS);

extern PrintQueueMenu(int);

/******************************************************************************
** Setup and display the file server selection menu.
*/
void FileServerMenu(void)
{
     int            iNumConns, iLoop;        /* Number of file server connections. */
     int            iMaxNameLen;             /* Max file server name length. */
     NWGMENU        ServSelect;              /* The menu. */
     NWGMENUITEMS   ServList;                /* Menu items list. */

     /* Get the number of connections. */
     iNumConns = GetNumFSConnections();
     
     /* Allocate memory for server menu list. */
     ServList = (NWGMENUITEMS) malloc(sizeof(NWGMENUITEM) * (iNumConns + 1));
     if(!ServList)
          NWGFXErrorMessage("Malloc failure!");
     
     /* Fill in our special entry. */
     ServList[0].pItem = "All Servers";
     
     /* Add the actual server names. */
     for (iLoop=0; iLoop < iNumConns; iLoop++)
          ServList[iLoop+1].pItem = GetFSName(iLoop);
     
     /* Fill in menu details. */
     ServSelect.iPosType   = MP_CENTRED;  

     /* Calculate menu width (adjust for borders). */
     iMaxNameLen = GetFSMaxNameLen() + 4;
     if (iMaxNameLen > 18)
          ServSelect.iSX = iMaxNameLen;
     else
          ServSelect.iSX = 18;

     /* Calculate menu height to fit all entries if possible. */
     if (iNumConns > wDispHeight-10)
          ServSelect.iSY        = wDispHeight - 10;   
     else
          ServSelect.iSY        = iNumConns + 5;   
     
     ServSelect.fFlags     = MF_BASIC | MF_CANCEL;    
     ServSelect.pTitle     = "Select Server";    
     ServSelect.tTimeOut   = (NWGTIME) NULL;  
     ServSelect.fnMenuProc = (NWGMENUPROC) FileServerMenuProc;
     ServSelect.iNumItems  = iNumConns + 1; 
     ServSelect.pItems     = ServList;    
     ServSelect.iCurrent   = 0;    
     ServSelect.iHelpID    = IDH_FILESERVER;    

     /* Show Select Queues popup. */
     NWGFXPopupMenu(&ServSelect);

     /* Free menu list memory. */
     free(ServList);
}

/******************************************************************************
** Callback for server selection menu. 
*/
NWGNUM FileServerMenuProc(NWGMSG wMsg, NWGNUM wSelected, NWGMENUITEMS pItems)
{
     /* Decode menu message. */
     switch(wMsg)
     {
          /* Item selected. */
          case MENU_SELECT:
               /* Display print queues. */
               PrintQueueMenu(wSelected);
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
