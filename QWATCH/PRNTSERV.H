/******************************************************************************
** (C) Chris Wood 1995.
**
** PRNTSERV.H - Structures, defs & prototypes for accessing print servers.
**
******************************************************************************* 
*/

/**** Global Defs ************************************************************/ 
#define MAX_PRINT_SERVERS 20       /* Maximum supported print servers. */
#define MAX_PSNAME_LEN    50       /* Maximum print server name length. */

/**** Global Structures ******************************************************/ 

/* A Print server (connection). */
typedef struct tagPrintServer {
     NWOBJ_ID       ObjectID;                /* The objects' ID. */
     NWCONN_HANDLE  ConnID;                  /* File server connection ID. */
     WORD           SPXConnID;               /* The SPX connection ID. */
     char           Name[MAX_PSNAME_LEN+1];  /* The servers' name. */
     char           MenuName[50];            /* The menu string. */
} PRINTSERVER, *PPRINTSERVER, *PS_LIST;      /* Aliases. */

/**** Prototypes *************************************************************/ 
void GetPrintServers(void);
int GetNumPrintServers(void);
NWCONN_HANDLE GetPSConnHandle(int iServer);
WORD GetPSSPXConn(int iServer);
char * GetPSName(int iServer);
char * GetPSNameByObjID(NWOBJ_ID ID);
int GetPSIndexByObjID(NWOBJ_ID ObjectID);
char * GetPSNameAndHost(int iServer);
NWOBJ_ID GetPSObjID(int iServer);
NWCCODE PSConnect(int iServer);
void PSDisconnect(int iServer);
int GetQueuesPrinter(int iServer, PPRINTQUEUE pQueue);
