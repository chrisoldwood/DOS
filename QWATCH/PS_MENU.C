/******************************************************************************
** (C) Chris Wood 1995.
**
** PS_MENU.C - Show and handle the print server selection menu.
**
******************************************************************************* 
*/

#include <string.h>
#include <malloc.h>
#include <nwcalls.h>
#include <nwgfx.h>
#include "fileserv.h"
#include "printjob.h"
#include "prtqueue.h"
#include "prntserv.h"
#include "nwerrs.h"
#include "helpids.h"

/**** Global Vars. ***********************************************************/
static NWGMENU      Servers;       /* The menu. */
static NWGMENUITEMS ServItems;     /* Array of menu items. */

extern NWGDIM  wDispWidth;         /* The display width in chars. */
extern NWGDIM  wDispHeight;        /* The display height in chars. */

/**** Function Prototypes. ***************************************************/

/******************************************************************************
** Setup and display the print server menu.
*/
NWOBJ_ID PrintServerMenu(PPRINTQUEUE pQueue)
{
     int       iSelected;                    /* Server selected. */
     NWNUM     lNumServers;                  /* Number of print servers. */
     int       iLoop, iIndex;                /* Server counter, index. */
     int       iLen, iMaxLen = 0;            /* Max length of menu strings. */
     NWOBJ_ID  QServers[MAX_PRINT_SERVERS];  /* List of servers servicing queue. */
     NWCCODE   wRetVal;                      /* NW return code. */
     char      *pItem;                       /* return string. */
     
     /* Get number of jobs and servers servicing. */
     wRetVal = NWReadQueueCurrentStatus2(pQueue->ConnID, pQueue->QueueID, NULL, 
                         (NWNUM NWFAR *) NULL, (NWNUM NWFAR *) &lNumServers, 
                         (NWOBJ_ID NWFAR *) QServers, NULL);

     /* Check the number of servers. */
     if (!lNumServers)
     {
          /* Notify user. */
          NWGFXInfoMessage("No Print Servers found");
          return 0;
     }
     else if (lNumServers == 1)
     {
          /* Only one, so no menu needed. */
          return QServers[0];
     }
     
     /* Allocate memory for the menu items. */
     ServItems = (NWGMENUITEMS) malloc(sizeof(NWGMENUITEM) * (int) lNumServers);
     if (!ServItems)
     {
          /* Show error message. */
          NWGFXErrorMessage("Malloc failure!");
          return 0;
     }
     
     /* Get full server names. */
     for (iLoop=0; iLoop < (int) lNumServers; iLoop++)
     {
          /* Get print server index. */
          iIndex = GetPSIndexByObjID(QServers[iLoop]);

          /* Get name and host, if recognised. */
          if (iIndex != -1)
               ServItems[iLoop].pItem = GetPSNameAndHost(iIndex);
          else
               ServItems[iLoop].pItem = "Unknown";
               
          /* Keep track of longest string. */
          iLen = strlen(ServItems[iLoop].pItem);
          if (iLen > iMaxLen)
               iMaxLen = iLen;
     }
     
     /* Fill in menu details. */
     Servers.iPosType   = MP_CENTRED;  

     /* Calculate menu width. */
     iMaxLen += 4;
     if (iMaxLen > 18)
          Servers.iSX = iMaxLen;
     else
          Servers.iSX = 18;
          
     /* Calculate menu height to fit all entries if possible. */
     if ((int)lNumServers > wDispHeight-10)
          Servers.iSY = wDispHeight - 10;   
     else
          Servers.iSY = (int)lNumServers + 4;   

     Servers.fFlags     = MF_BASIC;
     Servers.pTitle     = "Select Server";
     Servers.tTimeOut   = 0L;  
     Servers.fnMenuProc = NULL;
     Servers.iNumItems  = (NWGNUM) lNumServers; 
     Servers.pItems     = ServItems;    
     Servers.iCurrent   = 0;    
     Servers.iHelpID    = IDH_PRINTSERVER;    

     /* Display job list popup. */
     iSelected = NWGFXPopupMenu(&Servers);

     /* Check for a selection. */
     if (iSelected)
     {
          /* Save pointer to string. */
          pItem = ServItems[iSelected-1].pItem;
     }

     /* Free up menu items. */
     free(ServItems);

     /* User cancelled. */
     if (!iSelected)
          return 0;

     /* Check for valid name. */
     if (strcmp(pItem, "Unknown") == 0)
     {
          /* Notify user. */
          NWGFXInfoMessage("Unrecognised Print Server");
          
          return 0;
     }

     /* Return servers object ID. */
     return GetPSObjID(iSelected-1);
}
