/******************************************************************************
** (C) Chris Wood 1995.
**
** PRTQUEUE.H - Structures, defs & prototypes for accessing print queues.
**
******************************************************************************* 
*/

/**** Global Defs ************************************************************/ 
#define MAX_PQNAME_LEN  50         /* Maximum print queue name length. */

/**** Global Structures ******************************************************/ 

/* A Printer queue. */
typedef struct tagPrintQueue {
     NWCONN_HANDLE  ConnID;                       /* The server connection handle. */
     NWOBJ_ID       QueueID;                      /* The queue ID number. */
     char           Name[MAX_PQNAME_LEN+1];       /* The queue name. */
     int            iNumServers;                  /* Number of servers for queue. */
     NWOBJ_ID       ServerID;                     /* FIRST queue server ID. */
     NWCCODE        wError;                       /* Queue error code, if one. */
     int            iNumJobs;                     /* Number of jobs in the queue. */
     PJ_LIST        JobList;                      /* Pointer to job list. */
     char           MenuItem[80];                 /* Queue info as menu item. */
} PRINTQUEUE, *PPRINTQUEUE, *PQ_LIST;             /* Aliases. */

/**** Prototypes *************************************************************/ 
int GetNumPrintQueues(NWCONN_HANDLE hServer);
int GetPrintQueues(NWCONN_HANDLE hServer, PQ_LIST Queues);
