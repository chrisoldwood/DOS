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
int  iServer;                      /* The server selected. */

extern NWGDIM  wDispWidth;         /* The display width in chars. */
extern NWGDIM  wDispHeight;        /* The display height in chars. */

/**** Function Prototypes. ***************************************************/
NWGNUM FileServerMenuProc(NWGMSG, NWGNUM, NWGMENUITEMS);

extern void UserGroupMenu(void);

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
     ServList = (NWGMENUITEMS) malloc(sizeof(NWGMENUITEM) * iNumConns);
     if(!ServList)
          NWGFXErrorMessage("Malloc failure!");
     
     /* Add the server names. */
     for (iLoop=0; iLoop < iNumConns; iLoop++)
          ServList[iLoop].pItem = GetFSName(iLoop);
     
     /* Fill in menu details. */
     ServSelect.iPosType = MP_CENTRED;  

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
          ServSelect.iSY        = iNumConns + 4;   
     
     ServSelect.fFlags     = MF_BASIC | MF_CANCEL;    
     ServSelect.pTitle     = "Select Server";    
     ServSelect.tTimeOut   = (NWGTIME) NULL;  
     ServSelect.fnMenuProc = (NWGMENUPROC) FileServerMenuProc;
     ServSelect.iNumItems  = iNumConns; 
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
               /* Save server selected. */
               iServer = wSelected-1;
               
               /* Display user/group menu. */
               UserGroupMenu();
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
