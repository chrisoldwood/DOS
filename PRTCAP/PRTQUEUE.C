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
#include "prtqueue.h"

/**** Global Vars. ***********************************************************/

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
** Get a list of print queues on a given server.
*/
int GetPrintQueues(NWCONN_HANDLE hServer, PQ_LIST Queues)
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
          {
               /* Save connection handle. */
               Queues[iNumQueues].ConnID = hServer;
          
               /* Save queue ID. */
               Queues[iNumQueues].QueueID = QueueID;
          
               /* Get printer queue name. */
               NWGetObjectName(hServer, QueueID, (char NWFAR *) Queues[iNumQueues].Name, NULL);

               /* Update server count. */
               iNumQueues++;
          }
     }

     return iNumQueues;
}
