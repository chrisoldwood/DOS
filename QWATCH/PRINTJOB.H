/******************************************************************************
** (C) Chris Wood 1995.
**
** PRINTJOB.H - Structures, defs & prototypes for accessing print jobs.
**
******************************************************************************* 
*/

/**** Global Defs ************************************************************/ 

/**** Global Structures ******************************************************/ 

/* A Print Job. */
typedef struct tagPrintJob {
     NWQueueJobStruct    Info;          /* Standard NetWare job structure. */
     NWSIZE              lSize;         /* Job size. */
     NWCCODE             wError;        /* Job interrogation error. */
     char                MenuItem[80];  /* Job details as menu item. */
     struct tagPrintJob  *Next;         /* Pointer to next job in list. */
} PRINTJOB, *PPRINTJOB, *PJ_LIST;       /* Aliases. */

/**** Prototypes *************************************************************/ 
struct tagPrintQueue;

void GetPrintJobs(struct tagPrintQueue * Queue);
void GetPJInfo(struct tagPrintQueue * Queue);
void DeletePJList(struct tagPrintQueue * Queue);
