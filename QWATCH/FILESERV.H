/******************************************************************************
** (C) Chris Wood 1995.
**
** FILESERV.H - Structures, defs & prototypes for accessing file servers.
**
******************************************************************************* 
*/

/**** Global Defs ************************************************************/ 
#define MAX_FILE_SERVERS 10        /* Maxmimum supported file servers. */
#define MAX_FSNAME_LEN   50        /* Maximum file server name length. */

/**** Global Structures ******************************************************/ 

/* A File server (connection). */
typedef struct tagFileServer {
     NWOBJ_ID       ObjectID;                     /* The objects' ID. */
     NWCONN_HANDLE  ConnID;                       /* The connection handle. */
     char           Name[MAX_FSNAME_LEN+1];       /* The servers' name. */
} FILESERVER, *PFILESERVER, *FS_LIST;             /* Aliases. */

/**** Prototypes *************************************************************/ 
int GetNumFSConnections(void);
NWCCODE GetFSConnections(void);
NWCONN_HANDLE GetFSConnHandle(int iServer);
char * GetFSName(int iServer);
