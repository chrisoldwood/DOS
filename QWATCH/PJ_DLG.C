/******************************************************************************
** (C) Chris Wood 1995.
**
** PJ_DLG.C - Show and handle the print job dialog.
**
******************************************************************************* 
*/

#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <nwcalls.h>
#include <nwgfx.h>
#include "fileserv.h"
#include "printjob.h"
#include "prtqueue.h"
#include "prntserv.h"
#include "nwerrs.h"
#include "pj_dlg.h"

#define JOBINFO_UPDATE   5              /* Timeout for update. */

/**** Global Vars. ***********************************************************/
static PRINTJOB    Job;                 /* The currently displayed job. */
static PPRINTQUEUE pQueue;              /* The jobs' queue. */
static NWGDIALOG JobDlg;                /* The dialog. */
static char      UserName[50];          /* The jobs' username. */
static char      ServerName[25];        /* The jobs' servicing server, if one. */
static char      TargetServ[25];        /* the jobs' target server. */
static char      *Status;               /* The jobs' status. */
static char      JobPosition[10];       /* The jobs' position. */
static short     wJobNumber;            /* Truncated job number. */
static char      JobNumber[10];         /* The jobs' number. */
static char      JobForm[10];           /* The jobs' form number. */
static char      JobSize[15];           /* The jobs' size in bytes. */
static BYTE      *Time;                 /* Pointer to the jobs time. */
static char      JobDate[10];           /* The jobs' date. */
static char      JobTime[10];           /* The jobs' time. */

/**** Function Prototypes. ***************************************************/
NWGBOOL PrintJobDlgProc(NWGMSG wMsg, NWGNUM wCtrl, NWGVARPTR pValue);
void GetJobDlgInfo(void);
void UpdateJobInfo(void);

/******************************************************************************
** Display info about the selected print job.
*/
void PrintJobDlg(PPRINTQUEUE Queue, PPRINTJOB pCurrJob)
{
     /* Save pointer to queue. */
     pQueue = Queue;
     
     /* Copy job details into global. */
     memcpy(&Job, pCurrJob, sizeof(PRINTJOB));
     
     /* Get the jobs details for display. */
     GetJobDlgInfo();
     
     /* Set dialog job strings. */
     PJControls[JOBUSER].pValue   = UserName;
     PJControls[JOBTITLE].pValue  = (NWGSTR) Job.Info.jobDescription;
     PJControls[JOBFORM].pValue   = JobForm;
     PJControls[JOBSIZE].pValue   = JobSize;
     PJControls[JOBSTATUS].pValue = Status;
     PJControls[JOBSERVER].pValue = ServerName;
     PJControls[JOBTARGET].pValue = TargetServ;
     PJControls[JOBPOS].pValue    = JobPosition;
     PJControls[JOBNUMBER].pValue = JobNumber;
     PJControls[JOBDATE].pValue   = JobDate;
     PJControls[JOBTIME].pValue   = JobTime;
     
     /* Fill in the dialog details. */
     JobDlg.iPosType  = DP_CENTRED;
     JobDlg.iSX       = 64;
     JobDlg.iSY       = 15;

     /* Check job state for dialog border. */
     if (Job.Info.servicingServerID)
          JobDlg.fFlags = DF_STATIC | DF_TIMER;   
     else
          JobDlg.fFlags = DF_BASIC | DF_TIMER;   

     JobDlg.pTitle    = "Job Information";
     JobDlg.tTimeOut  = JOBINFO_UPDATE;
     JobDlg.fnDlgProc = PrintJobDlgProc;
     JobDlg.iNumCtls  = PJ_CONTROLS;
     JobDlg.pControls = PJControls;

     /* Display the dialog. */
     NWGFXSetHelpSection(3);
     NWGFXDialog(&JobDlg);
}

/******************************************************************************
** Print job dialog callback.
*/
NWGBOOL PrintJobDlgProc(NWGMSG wMsg, NWGNUM wCtrl, NWGVARPTR pValue)
{
     NWGNUM    wValue;                  /* Menu calue. */
     
     /* Decode dialog message. */
     switch(wMsg)
     {
          /* Verify entry. */
          case DIALOG_SELECT:
               break;
               
          /* Timeout... */
          case DIALOG_TIMEOUT:
               /* Time to update. */
               UpdateJobInfo();
               return TRUE;
               
          /* Safe. */
          default:
               break;
     }
     
     /* Don't update. */
     return FALSE;
}

/******************************************************************************
** Convert the job details to strings for display in the dialog.
*/
void GetJobDlgInfo(void)
{
     /* Get jobs' user name. */
     NWGetObjectName(pQueue->ConnID, Job.Info.clientID, (char NWFAR *) UserName, NULL);     

     /* Check for server. */
     if (Job.Info.servicingServerID)
     {
          /* Make a copy of the server name. */
          strncpy(ServerName, GetPSNameByObjID(Job.Info.servicingServerID), 20);
          
          /* Truncate. */
          ServerName[20] = '\0';
     }
     else
     {
          /* Not being serviced. */
          strcpy(ServerName, "(none)");
     }
     
     /* check for target server. */
     if (Job.Info.targetServerID != 0xFFFFFFFF)
     {
          /* Make a copy of the server name. */
          strncpy(TargetServ, GetPSNameByObjID(Job.Info.targetServerID), 20);
          
          /* Truncate. */
          TargetServ[20] = '\0';
     }
     else
     {
          /* No target specified. */
          strcpy(TargetServ, "(any server)");
     }
     
     /* Decode job status. */
     if (Job.Info.jobControlFlags & 0x20)
     {
          /* Job is being added to the queue. */
          Status = "Adding";
     }
     else if (Job.Info.jobControlFlags & 0x80)
     {
          /* Job is on operator-hold. */
          Status = "O-Hold";
     }
     else if (Job.Info.jobControlFlags & 0x40) 
     {
          /* Job is on user-hold. */
          Status = "U-Hold";
     }
     else if (Job.Info.servicingServerID)
     {
          /* Job is currently active. */
          Status = "Active";
     }
     else
     {
          /* Must be waiting to be serviced. */
          Status = "Ready";
     }

     /* Create string from job position. */
     sprintf(JobPosition, "%d", Job.Info.jobPosition);

     /* Truncate job number. */
     wJobNumber = (short) Job.Info.jobNumber;
     
     /* Swap bytes to get actual value. */
     wJobNumber = ((wJobNumber >> 8) & 0x00FF) | ((wJobNumber << 8) & 0xFF00);
     
     /* Create string from job number. */
     sprintf(JobNumber, "%d", wJobNumber);

     /* Create string from job form type. */
     sprintf(JobForm, "%d", (int)(Job.Info.jobType >> 8));

     /* Create string from job size. */
     sprintf(JobSize, "%ld", Job.lSize);

     /* Get pointer to the time. */
     Time = Job.Info.jobEntryTime;
     
     /* Create string from job entry date. */
     sprintf(JobDate, "%d/%d/%d", (int)Time[2], (int)Time[1], (int)Time[0]);

     /* Create string from job entry time. */
     sprintf(JobTime, "%d:%d:%d", (int)Time[3], (int)Time[4], (int)Time[5]);

     /* Set dialog strings that are explicit pointers. */
     PJControls[JOBTITLE].pValue = (NWGSTR) Job.Info.jobDescription;
     PJControls[JOBSTATUS].pValue = Status;

     /* Check job state for dialog border. */
     if (Job.Info.servicingServerID)
          JobDlg.fFlags    = DF_STATIC | DF_TIMER;   
     else
          JobDlg.fFlags    = DF_BASIC | DF_TIMER;   
}

/******************************************************************************
** Update the info about the job.
*/
void UpdateJobInfo(void)
{
     NWCCODE   wRetVal;            /* Return code. */
     
     /* Show wait message. */
     NWGFXWaitMessage(TRUE);

     /* Get job info. */
     wRetVal = NWReadQueueJobEntry2(pQueue->ConnID, pQueue->QueueID, Job.Info.jobNumber, 
                                        (NWQueueJobStruct NWFAR *) &Job.Info);

     /* Check job still exists. */
     if (wRetVal)
     {
          /* Blank out job details. */
          PJControls[JOBUSER].pValue   = "";
          PJControls[JOBTITLE].pValue  = "";
          PJControls[JOBFORM].pValue   = "";
          PJControls[JOBSIZE].pValue   = "";
          PJControls[JOBSTATUS].pValue = "Complete";
          PJControls[JOBSERVER].pValue = "";
          PJControls[JOBTARGET].pValue = "";
          PJControls[JOBPOS].pValue    = "";
          PJControls[JOBNUMBER].pValue = "";
          PJControls[JOBDATE].pValue   = "";
          PJControls[JOBTIME].pValue   = "";
          JobDlg.tTimeOut = 0;
          JobDlg.fFlags   = DF_STATIC;   
     }
     else
     {
          /* Get job size. */
          wRetVal = NWGetQueueJobFileSize2(pQueue->ConnID, pQueue->QueueID, 
                    Job.Info.jobNumber, (NWSIZE NWFAR *) &Job.lSize);

          /* Convert for dialog display. */
          GetJobDlgInfo();
     }
          
     /* Hide wait message. */
     NWGFXWaitMessage(FALSE);
}
