/******************************************************************************
** (C) Chris Wood 1995.
**
** GROUPS.H - Structures, defs & prototypes for accessing groups.
**
******************************************************************************* 
*/

/**** Global Defs ************************************************************/ 
#define MAX_GROUP_LEN  50           /* Maximum group name length. */

/**** Global Structures ******************************************************/ 

/* A Group. */
typedef struct tagUser {
     NWCONN_HANDLE  ConnID;                       /* The server connection handle. */
     NWOBJ_ID       GroupID;                      /* The groups' ID number. */
     char           Name[MAX_GROUP_LEN+1];        /* The groups' name. */
} GROUP, *PGROUP, *GROUP_LIST;                       /* Aliases. */

/**** Prototypes *************************************************************/ 
int GetGroupCount(NWCONN_HANDLE hServer);
int GetGroups(NWCONN_HANDLE hServer);
char * GetGroupName(int iIndex);
