/******************************************************************************
** (C) Chris Wood 1995.
**
** PJ_MENU.C - Show and handle the print jobs menu.
**
******************************************************************************* 
*/

#include <malloc.h>
#include <nwcalls.h>
#include <nwgfx.h>
#include "fileserv.h"
#include "printjob.h"
#include "prtqueue.h"
#include "nwerrs.h"

#define JOBS_UPDATE     5          /* Seconds between jobs update. */

/**** Global Vars. ***********************************************************/
static NWGMENU      Jobs;          /* The menu. */
static NWGMENUITEMS JobItems;      /* Array of menu items. */
static PJ_LIST      JobList;       /* Print job list. */
static PPRINTQUEUE  JobQueue;      /* The currently watch queue. */

extern NWGDIM  wDispWidth;         /* The display width in chars. */
extern NWGDIM  wDispHeight;        /* The display height in chars. */

/**** Function Prototypes. ***************************************************/
NWGNUM PrintJobsMenuProc(NWGMSG, NWGNUM, NWGMENUITEMS);

extern PrintJobDlg(PPRINTQUEUE, PPRINTJOB);

/******************************************************************************
** Setup and display the print jobs menu.
*/
void PrintJobsMenu(PPRINTQUEUE Queue)
{
     int       iLoop;                   /* Job counter. */
     PPRINTJOB pJob;                    /* Job list pointer. */

     /* Check for original queue error. */
     if (Queue->wError)
     {
          /* Can't interrogate queue. */
          NWGFXInfoMessage(ErrToString(Queue->wError));
          return;
     }
     
     /* Show wait message. */
     NWGFXWaitMessage(TRUE);

     /* Save the job queue in a global. */
     JobQueue = Queue;
     
     /* Check for jobs in the queue. */
     if (Queue->iNumJobs)
     {
          /* Allocate memory for menu items. */
          JobItems = (NWGMENUITEMS) malloc(sizeof(NWGMENUITEM) * Queue->iNumJobs);
          if (!JobItems)
          {
               /* Hide wait message. */
               NWGFXWaitMessage(FALSE);

               /* Show error message. */
               NWGFXErrorMessage("Malloc failure!");
               return;
          }
     }
          
     /* Create the menu strings from the jobs info. */
     GetPJInfo(Queue);
     
     /* Get head of job list. */
     pJob = Queue->JobList;
     
     /* Set menu items pointer to job info string. */
     for (iLoop=0; iLoop < Queue->iNumJobs; iLoop++)
     {
          /* Get string. */
          JobItems[iLoop].pItem = pJob->MenuItem;
          
          /* Move on a job.*/
          pJob = pJob->Next;
     }
     
     /* Fill in menu details. */
     Jobs.iPosType   = MP_CENTRED;  
     Jobs.iSX        = 70;
     Jobs.iSY        = 16;   
     Jobs.fFlags     = MF_TIMER | MF_EXTENDED | MF_DELETE;    
     Jobs.pTitle     = "Pos User         Description                 Form   Size  Status ";
     Jobs.tTimeOut   = (NWGTIME) JOBS_UPDATE;  
     Jobs.fnMenuProc = (NWGMENUPROC) PrintJobsMenuProc;
     Jobs.iNumItems  = Queue->iNumJobs; 
     Jobs.pItems     = JobItems;    

     /* Hide wait message. */
     NWGFXWaitMessage(FALSE);

     /* Display job list popup. */
     NWGFXSetHelpSection(2);
     NWGFXPopupMenu(&Jobs);
     
     /* Free menu items memory. */
     free(JobItems);
}

/******************************************************************************
** Update the list of print jobs in the monitored queue. 
*/
void UpdatePrintJobs(void)
{
     int       iLoop;                   /* Job counter. */
     PPRINTJOB pJob;                    /* Job list pointer. */

     /* Show wait message. */
     NWGFXWaitMessage(TRUE);

     /* Update job list. */
     GetPrintJobs(JobQueue);

     /* Free the old menu items. */
     free(JobItems);
     
     /* Check for jobs in the queue. */
     if (JobQueue->iNumJobs)
     {
          /* Allocate memory for menu items. */
          JobItems = (NWGMENUITEMS) malloc(sizeof(NWGMENUITEM) * JobQueue->iNumJobs);
          if (!JobItems)
          {
               /* Hide wait message. */
               NWGFXWaitMessage(FALSE);

               /* Ensure menu items are NULL. */
               Jobs.iNumItems  = 0; 
               Jobs.pItems     = NULL;    
               
               /* Show error message. */
               NWGFXErrorMessage("Malloc failure!");
               return;
          }
     }
     else
     {
          /* Ensure menu items are NULL. */
          Jobs.iNumItems  = 0; 
          Jobs.pItems     = NULL;
        
          /* Hide wait message. */
          NWGFXWaitMessage(FALSE);

          /* Update. */
          return;
     }
     
     /* Create the menu strings from the jobs info. */
     GetPJInfo(JobQueue);
     
     /* Get head of job list. */
     pJob = JobQueue->JobList;
     
     /* Set menu items pointer to job info string. */
     for (iLoop=0; iLoop < JobQueue->iNumJobs; iLoop++)
     {
          /* Get string. */
          JobItems[iLoop].pItem = pJob->MenuItem;
     
          /* Move on a job.*/
          pJob = pJob->Next;
     }

     /* Update menu. */
     Jobs.iNumItems  = JobQueue->iNumJobs; 
     Jobs.pItems     = JobItems;    

     /* Hide wait message. */
     NWGFXWaitMessage(FALSE);
}

/******************************************************************************
** Callback for print job menu. 
*/
NWGNUM PrintJobsMenuProc(NWGMSG wMsg, NWGNUM wSelected, NWGMENUITEMS pItems)
{
     int       iLoop;                   /* Menu item counter. */
     NWGBOOL   bDelete;                 /* Deletion confirm. */
     NWCCODE   wRetVal;                 /* Return code. */
     PPRINTJOB pJob;                    /* Job list pointer. */
     
     /* Decode menu message. */
     switch(wMsg)
     {
          /* Queue selected. */
          case MENU_SELECT:
               /* Check we've only got one selected. */
               if (!wSelected)
                    break;

               /* Get head of job list. */
                pJob = JobQueue->JobList;
     
               /* Run through the job list. */
               for (iLoop=1; iLoop < wSelected; iLoop++)
               {
                    /* Move on a job.*/
                    pJob = pJob->Next;
               }
                    
               /* Call the job dialog. */
               PrintJobDlg(JobQueue, pJob);
               
               /* Force an update. */
               UpdatePrintJobs();

               /* Restore help to this menu. */
               NWGFXSetHelpSection(2);
               return TRUE;

          /* Delete jobs from the queue. */
          case MENU_DELETE:
               /* Check for any items. */
               if (!JobQueue->iNumJobs)
                    break;
                    
               /* Get confirmation. */
               if (!wSelected)
                    bDelete = NWGFXQueryMenu("Delete All Marked Jobs? ");
               else
                    bDelete = NWGFXQueryMenu("Delete Selected Job?");
               
               /* Okay? */
               if (!bDelete)
                    break;
                    
               /* Get head of job list. */
                pJob = JobQueue->JobList;
     
               /* Run through the job list. */
               for (iLoop=0; iLoop < JobQueue->iNumJobs; iLoop++)
               {
                    /* Check item for deletion. */
                    if ( (!wSelected) && (pItems[iLoop].bTagged) )
                         bDelete = TRUE;
                    else if ( (wSelected) && (iLoop == wSelected-1) )
                         bDelete = TRUE;
                    else
                         bDelete = FALSE;
     
                    /* Delete this job? */
                    if (bDelete)
                    {
                         /* Try and delete it. */
                         wRetVal = NWRemoveJobFromQueue2(JobQueue->ConnID, 
                                        JobQueue->QueueID, pJob->Info.jobNumber);
                         
                         /* Check for error, ignore if job has been serviced. */
                         if ( (wRetVal) && (wRetVal != ERR_NO_Q_JOB) )
                         {
                              /* Check for access rights. */
                              if ( (wRetVal == ERR_NO_Q_RIGHTS)
                              || (wRetVal == ERR_NO_Q_JOB_RIGHTS) )
                              {
                                   /* Display error. */
                                   if (!wSelected)
                                        NWGFXInfoMessage("You have insufficient rights\r to delete these jobs");
                                   else
                                        NWGFXInfoMessage("You have insufficient rights\r to delete this job");
                              }
                              else /* Other error. */
                              {
                                   /* Display error. */
                                   NWGFXInfoMessage(ErrToString(wRetVal));
                              }
                              
                              /* Force a full update. */
                              UpdatePrintJobs();
                              
                              /* Bail out now. */
                              return TRUE;
                         }
                    }
                    
                    /* Move on a job.*/
                    pJob = pJob->Next;
               }
               
               /* Force a full update. */
               UpdatePrintJobs();
               return TRUE;

          /* Timeout... */
          case MENU_TIMEOUT:
               /* Time to update. */
               UpdatePrintJobs();
               return TRUE;
               
          /* Safe. */
          default:
               break;
     }
     
     /* Don't update. */
     return FALSE;
}
