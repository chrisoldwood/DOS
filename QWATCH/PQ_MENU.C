/******************************************************************************
** (C) Chris Wood 1995.
**
** PQ_MENU.C - Show and handle the print queue selection menu.
**
******************************************************************************* 
*/

#include <malloc.h>
#include <nwcalls.h>
#include <nwgfx.h>
#include "fileserv.h"
#include "printjob.h"
#include "prtqueue.h"
#include "helpids.h"

#define QUEUE_UPDATE     5         /* Seconds between queue updates. */

/**** Global Vars. ***********************************************************/
static int  iNumQueues;            /* Number of queues to watch. */
static NWGMENUITEMS QItems;        /* Array of menu items. */
static PQ_LIST      QList;         /* Print queue list. */

extern NWGDIM  wDispWidth;         /* The display width in chars. */
extern NWGDIM  wDispHeight;        /* The display height in chars. */

/**** Function Prototypes. ***************************************************/
NWGNUM PrintQueueMenuProc(NWGMSG, NWGNUM, NWGMENUITEMS);

extern void PrintJobsMenu(PPRINTQUEUE);

/******************************************************************************
** Setup and display the print queue selection menu.
*/
void PrintQueueMenu(int iSelection)
{
     NWGMENU   Queues;                  /* The menu. */
     int       iNumConns, iLoop;        /* Number of file server connections. */
     int       iQueueCnt;               /* Total number of queues. */
     
     /* Show wait message. */
     NWGFXWaitMessage(TRUE);

     /* Reset number of queues found. */
     iNumQueues = 0;
     QItems = NULL;
     
     /* Get number of file servers. */
     iNumConns = GetNumFSConnections();

     /* Check item selected. */
     if (iSelection == 1)
     {
          /* Get number of queues of all servers. */
          for(iLoop=0; iLoop < iNumConns; iLoop++)
          {
               /* Find out how many queues are on this server. */
               iNumQueues += GetNumPrintQueues(GetFSConnHandle(iLoop));
          }
     }
     else /* iSelection > 1. */
     {
          /* Find out how many queues are on just this server. */
          iNumQueues = GetNumPrintQueues(GetFSConnHandle(iSelection-2));
     }
                    
     /* Check to see if we found any. */
     if (!iNumQueues)
     {
          /* Hide wait message. */
          NWGFXWaitMessage(FALSE);

          /* Show error message. */
          NWGFXInfoMessage("No Printer Queues Found");
          return;
     }

     /* Allocate memory for menu items. */
     QItems = (NWGMENUITEMS) malloc(sizeof(NWGMENUITEM) * iNumQueues);
     if (!QItems)
     {
          /* Hide wait message. */
          NWGFXWaitMessage(FALSE);

          /* Show error message. */
          NWGFXErrorMessage("Malloc failure!");
          return;
     }
     
     /* Allocate memory for print queue list. */
     QList = (PPRINTQUEUE) malloc(sizeof(PRINTQUEUE) * iNumQueues);
     if (!QList)
     {
          /* Hide wait message. */
          NWGFXWaitMessage(FALSE);

          /* Show error message. */
          NWGFXErrorMessage("Malloc failure!");
          return;
     }

     /* Get info about the queues. */
     if (iSelection == 1)
     {
          /* Reset queue count. */
          iQueueCnt = 1;
          
          /* Get queues for each of the servers. */
          for(iLoop=0; iLoop < iNumConns; iLoop++)
               iQueueCnt += GetPrintQueues(GetFSConnHandle(iLoop), &QList[iQueueCnt-1]);
     }
     else /* iSelection > 1. */
     {
          /* Get queues for just one server. */
          GetPrintQueues(GetFSConnHandle(iSelection-2), QList);
     }

     /* Set menu items pointer to queue info string. */
     for (iLoop=0; iLoop < iNumQueues; iLoop++)
          QItems[iLoop].pItem = QList[iLoop].MenuItem;
     
     /* Fill in menu details. */
     Queues.iPosType   = MP_ABSOLUTE;  
     Queues.iSX        = 0;
     Queues.iSY        = 3;   
     Queues.iDX        = wDispWidth - 1;
     Queues.iDY        = wDispHeight - 1;   
     Queues.fFlags     = MF_TIMER;    
     Queues.pTitle     = "Queues           Server(s)        Jobs Current        Form    Size   Status";    
     Queues.tTimeOut   = (NWGTIME) QUEUE_UPDATE;  
     Queues.fnMenuProc = (NWGMENUPROC) PrintQueueMenuProc;
     Queues.iNumItems  = iNumQueues; 
     Queues.pItems     = QItems;    
     Queues.iCurrent   = 0;    
     Queues.iHelpID    = IDH_QUEUES;    

     /* Hide wait message. */
     NWGFXWaitMessage(FALSE);

     /* Display queue monitor popup. */
     NWGFXPopupMenu(&Queues);
     
     /* Free menu items/print queue list memory. */
     free(QItems);
     free(QList);

     /* Free job lists. */
     for (iLoop=0; iLoop < iNumQueues; iLoop++)
          DeletePJList(&QList[iLoop]);
}

/******************************************************************************
** Callback for print queue selection menu. 
*/
NWGNUM PrintQueueMenuProc(NWGMSG wMsg, NWGNUM wSelected, NWGMENUITEMS pItems)
{
     int iLoop;                    /* Queue counter. */

     /* Decode menu message. */
     switch(wMsg)
     {
          /* Queue selected. */
          case MENU_SELECT:
               /* Display the list of jobs in the queue. */
               PrintJobsMenu(&QList[wSelected-1]);
               
               /* Show wait message. */
               NWGFXWaitMessage(TRUE);

               /* Update job lists. */
               for (iLoop=0; iLoop < iNumQueues; iLoop++)
                    GetPrintJobs(&QList[iLoop]);

               /* Hide wait message. */
               NWGFXWaitMessage(FALSE);
               break;

          /* Timout... */
          case MENU_TIMEOUT:
               /* Show wait message. */
               NWGFXWaitMessage(TRUE);

               /* Update job lists. */
               for (iLoop=0; iLoop < iNumQueues; iLoop++)
                    GetPrintJobs(&QList[iLoop]);

               /* Hide wait message. */
               NWGFXWaitMessage(FALSE);
               break;
               
          /* Safe. */
          default:
               break;
     }
     
     /* Don't update. */
     return FALSE;
}
