/******************************************************************************
** (C) Chris Wood 1995.
**
** PRINTJOB.C - Functions to access print jobs.
**
******************************************************************************* 
*/

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <nwcalls.h>
#include <nwgfx.h>
#include "prntserv.h"
#include "printjob.h"
#include "prtqueue.h"

/**** Global Vars. ***********************************************************/
static QueueJobListReply JobList;            /* List of jobs in queue. */

/**** Function Prototypes. ***************************************************/

/******************************************************************************
** Get a list of jobs for the given queue.
*/
void GetPrintJobs(PPRINTQUEUE Queue)
{
     PPRINTJOB LastJob, NewJob, ActiveJob;   /* Pointers to jobs. */
     int       iLoop;                        /* Job counter. */
     NWCCODE   wRetVal;                      /* Return code. */
     char      ServerName[20];               /* Temporary server name buffer. */
     char      QueueName[20];                /* Temporary queue name buffer. */
     char      UserName[50];                 /* Job user. */
     int       iForm;                        /* Job form type. */
     char      *Status;                      /* Job status. */

     /* Check for original queue error. */
     if (Queue->wError)
          return;
               
     /* First delete the old list. */
     DeletePJList(Queue);

     /* Get number of jobs in this queue. */
     wRetVal = NWGetQueueJobList2(Queue->ConnID, Queue->QueueID, 0, 
                                   (QueueJobListReply NWFAR *) &JobList);

     /* Check for error. */
     if (wRetVal)
     {
          /* Just set to no jobs. */
          Queue->iNumJobs = 0;
     }
     else
     {
          /* Save number of Jobs. */
          Queue->iNumJobs = (int) JobList.totalQueueJobs;
     }

     /* Get the servers' name. */
     if (!Queue->iNumServers)
          strcpy(ServerName, " ");
     else
     {
          /* Get first server name. */
          strncpy(ServerName, GetPSNameByObjID(Queue->ServerID), 20);

          /* Truncate name. */
          ServerName[13] = '\0';
               
          /* Check for more than one. */
          if (Queue->iNumServers > 1)
          {
               /* Signal more than one. */
               strcat(ServerName, "...");
          }
     }
 
     /* Make a copy of the queue name. */
     strncpy(QueueName, Queue->Name, 20);
     
     /* Truncate. */
     QueueName[15] = '\0';
     
     /* Check we have jobs to collate. */
     if (!Queue->iNumJobs)
     {
          /* Ensure job pointer is zero. */
          Queue->JobList = NULL;

          /* Just display the queue and server names. */
          sprintf(Queue->MenuItem, "%-16s %-16s", QueueName, ServerName);
          return;
     }
     
     /* Setup. */
     LastJob = NULL;
     ActiveJob = NULL;
     
     /* Loop round for each job. */
     for (iLoop=0; iLoop < Queue->iNumJobs; iLoop++)
     {
          /* Allocate memory for job. */
          NewJob = (PPRINTJOB) malloc(sizeof(PRINTJOB));
          if (!NewJob)
          {
               /* Display error and bail out. */
               NWGFXErrorMessage("Malloc failure!");
               break;
          }
          
          /* Point previous job to this one. */
          if (LastJob == NULL)
          {
               /* First job in list. */
               Queue->JobList = NewJob;
          }
          else
          {
               /* Just another job. */
               LastJob->Next = NewJob;
          }

          /* Set next pointer. */
          NewJob->Next = NULL;
               
          /* Point last one to the new one. */
          LastJob = NewJob;

          /* Get job info. */
          wRetVal = NWReadQueueJobEntry2(Queue->ConnID, Queue->QueueID, 
                    JobList.jobNumberList[iLoop], (NWQueueJobStruct NWFAR *) &NewJob->Info);

          /* Save error. */
          NewJob->wError = wRetVal;
          
          /* Check for error. */
          if (!NewJob->wError)
          {
               /* Get job size. */
               NWGetQueueJobFileSize2(Queue->ConnID, Queue->QueueID,
                    JobList.jobNumberList[iLoop], (NWSIZE NWFAR *) &NewJob->lSize);
                    
               /* Check for active job. */
               if (NewJob->Info.servicingServerID)
                    ActiveJob = NewJob;
               
               /* If first job in queue and no active job known. */
               if ( (!ActiveJob) && (NewJob->Info.jobPosition == 1) )
                    ActiveJob = NewJob;
          }
     }

     /* Extract job form. */
     iForm = ActiveJob->Info.jobType >> 8;

     /* Get jobs' user name. */
     NWGetObjectName(Queue->ConnID, ActiveJob->Info.clientID, (char NWFAR *) UserName, NULL);     
     
     /* Truncate. */
     UserName[13] = '\0';
     
     /* Decode job status. */
     if (ActiveJob->Info.jobControlFlags & 0x20)
     {
          /* Job is being added to the queue. */
          Status = "Adding";
     }
     else if ( (ActiveJob->Info.jobControlFlags & 0x40) 
            || (ActiveJob->Info.jobControlFlags & 0x80) )
     {
          /* Job is on hold; by the operator or user. */
          Status = "Hold";
     }
     else if (ActiveJob->Info.servicingServerID)
     {
          /* Job is currently active. */
          Status = "Active";
     }
     else
     {
          /* Must be waiting to be serviced. */
          Status = "Ready";
     }
     
     /* Fill in all job details. */
     sprintf(Queue->MenuItem, "%-16s %-16s  %2d  %-14s  %2d  %8ld  %-s", 
               QueueName, ServerName, Queue->iNumJobs, UserName, 
               iForm, ActiveJob->lSize, Status);
}

/******************************************************************************
** Create the menu string from the list of print jobs.
*/
void GetPJInfo(PPRINTQUEUE Queue)
{
     PPRINTJOB Job;                /* Temporary job pointer. */
     int       iLoop;              /* Job counter. */
     char      UserName[50];       /* Job user. */
     int       iForm;              /* Job form type. */
     char      *Status;            /* Job status. */
     char      Title[30];          /* Job description. */
     
     /* Check there are jobs in the list. */
     if (!Queue->iNumJobs)
          return;

     /* Get the head. */
     Job = Queue->JobList;
     
     /* Run through the list. */
     for (iLoop=0; iLoop < Queue->iNumJobs; iLoop++)
     {
          /* Extract job form. */
          iForm = Job->Info.jobType >> 8;

          /* Get jobs' user name. */
          NWGetObjectName(Queue->ConnID, Job->Info.clientID, (char NWFAR *) UserName, NULL);     
     
          /* Truncate. */
          UserName[11] = '\0';
          
          /* Decode job status. */
          if (Job->Info.jobControlFlags & 0x20)
          {
               /* Job is being added to the queue. */
               Status = "Adding";
          }
          else if (Job->Info.jobControlFlags & 0x40) 
          {
               /* Job is on user-hold. */
               Status = "U-Hold";
          }
          else if (Job->Info.jobControlFlags & 0x80)
          {
               /* Job is on operator-hold. */
               Status = "O-Hold";
          }
          else if (Job->Info.servicingServerID)
          {
               /* Job is currently active. */
               Status = "Active";
          }
          else
          {
               /* Must be waiting to be serviced. */
               Status = "Ready";
          }

          /* Truncate job description, if necessary. */
          if (strlen((char*) Job->Info.jobDescription) > 28)
          {
               /* Copy truncated string and add terminator. */
               strncpy(Title, (char*) Job->Info.jobDescription, 25);
               Title[25] = '\0';
               
               /* Add truncation notify string. */
               strcat(Title, "...");
          }
          else
          {
               strcpy(Title, (char*) Job->Info.jobDescription);
          }
          
          /* Fill in all job details in menu string. */
          sprintf(Job->MenuItem, "%2d  %-12s %-28s %2d %8ld %s", Job->Info.jobPosition, 
                    UserName, Title, iForm, Job->lSize, Status);
          
          /* Move on. */
          Job = Job->Next;
     }
}

/******************************************************************************
** Delete the job list for a given queue.
*/
void DeletePJList(PPRINTQUEUE Queue)
{
     PPRINTJOB Job;           /* Temporary job pointer. */
     
     /* Check there are jobs in the list. */
     if (!Queue->iNumJobs)
          return;

     /* Get the head. */
     Job = Queue->JobList;
     
     /* Run through the list. */
     while(Job->Next)
     {
          /* Free and move on. */
          free(Job);
          Job = Job->Next;
     }
     
     /* Free the last entry. */
     free(Job);
}
