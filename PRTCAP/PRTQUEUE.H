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
} PRINTQUEUE, *PPRINTQUEUE, *PQ_LIST;             /* Aliases. */

/**** Prototypes *************************************************************/ 
int GetNumPrintQueues(NWCONN_HANDLE hServer);
int GetPrintQueues(NWCONN_HANDLE hServer, PQ_LIST Queues);
