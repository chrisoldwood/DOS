/******************************************************************************
** (C) Chris Wood 1995.
**
** PRTQUEUE.C - Functions to access print queues.
**
******************************************************************************* 
*/

#include <stdio.h>
#include <string.h>
#include <nwcalls.h>
#include "nwerrs.h"
#include "printjob.h"
#include "prtqueue.h"
#include "prntserv.h"

/**** Global Vars. ***********************************************************/
static NWOBJ_ID QServers[MAX_PRINT_SERVERS];
                                        /* List of servers servicing queue. */

/******************************************************************************
** Get the number of print queues on a given server.
*/
int GetNumPrintQueues(NWCONN_HANDLE hServer)
{
     NWCCODE   wFailure = 0;            /* List end flag. */
     NWOBJ_ID  QueueID = (NWOBJ_ID) -1; /* Queue ID returned; -1 is for find first. */
     int       iNumQueues;              /* Number of queues to date. */

     /* Reset queue count. */
     iNumQueues = 0;
          
     /* Search until list exhausted. */
     while(!wFailure)
     {
          /* Search for next. */
          wFailure = NWScanObject(hServer, (char NWFAR *) "*", OT_PRINT_QUEUE, 
                         (NWOBJ_ID NWFAR *) &QueueID, NULL, NULL, NULL, NULL,
                         NULL);

          /* Check for last server. */
          if (!wFailure)
               iNumQueues++;
     }

     return iNumQueues;
}

/******************************************************************************
** Get the number of print queues on a given server.
*/
int GetPrintQueues(NWCONN_HANDLE hServer, PQ_LIST Queues)
{
     NWCCODE   wFailure = 0;            /* List end flag. */
     NWOBJ_ID  QueueID = (NWOBJ_ID) -1; /* Queue ID returned; -1 is for find first. */
     int       iNumQueues;              /* Number of queues to date. */
     NWCCODE   wRetVal;                 /* NW return code. */
     NWNUM     lNumJobs;                /* Number of jobs in queue.*/
     NWNUM     lNumServers;             /* Number of servers servicing queue. */
     char      QueueName[20];           /* Temporaray print queue name buffer. */
     
     /* Reset queue count. */
     iNumQueues = 0;
          
     /* Search until list exhausted. */
     while(!wFailure)
     {
          /* Search for next. */
          wFailure = NWScanObject(hServer, (char NWFAR *) "*", OT_PRINT_QUEUE, 
                         (NWOBJ_ID NWFAR *) &QueueID, NULL, NULL, NULL, NULL,
                         NULL);

          /* Check for last server. */
          if (!wFailure)
          {
               /* Save connection handle. */
               Queues[iNumQueues].ConnID = hServer;
          
               /* Save queue ID. */
               Queues[iNumQueues].QueueID = QueueID;
          
               /* Get printer queue name. */
               NWGetObjectName(hServer, QueueID, (char NWFAR *) Queues[iNumQueues].Name, NULL);

               /* Get number of jobs and servers servicing. */
               wRetVal = NWReadQueueCurrentStatus2(hServer, QueueID, NULL, 
                         (NWNUM NWFAR *) &lNumJobs, (NWNUM NWFAR *) &lNumServers, 
                         (NWOBJ_ID NWFAR *) QServers, NULL);
          
               /* Save error code. */
               Queues[iNumQueues].wError = wRetVal;

               /* Check for error. */
               if (!wRetVal)
               {
                    /* Save number of servers. */
                    Queues[iNumQueues].iNumServers = (int) lNumServers;
                    
                    /* Save first server id. */
                    Queues[iNumQueues].ServerID = QServers[0];
                    
                    /* Ensure job list pointer is null. */
                    Queues[iNumQueues].JobList = NULL;
                    Queues[iNumQueues].iNumJobs = 0;
                    
                    /* Get print job list. */
                    GetPrintJobs(&Queues[iNumQueues]);
               }
               else
               {
                    /* Make a copy of the queue name. */
                    strncpy(QueueName, Queues[iNumQueues].Name, 20);
                    
                    /* Truncate. */
                    QueueName[15] = '\0';
                    
                    /* Just display the queue name and error. */
                    sprintf(Queues[iNumQueues].MenuItem, "%-16s %s", QueueName, 
                              ErrToString(wRetVal));
               }
               
               /* Update server count. */
               iNumQueues++;
          }
     }

     return iNumQueues;
}
