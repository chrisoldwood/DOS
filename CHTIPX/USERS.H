/******************************************************************************
** (C) Chris Wood 1995.
**
** USERS.H - Structures, defs & prototypes for accessing users.
**
******************************************************************************* 
*/

/**** Global Defs ************************************************************/ 
#define MAX_USER_LEN  50           /* Maximum user name length. */

/**** Global Structures ******************************************************/ 

/* A user. */
typedef struct tagUser {
     NWCONN_HANDLE  ConnID;                       /* The server connection handle. */
     NWOBJ_ID       UserID;                       /* The users' ID number. */
     char           Name[MAX_USER_LEN+1];         /* The users' name. */
} USER, *PUSER, *USER_LIST;                       /* Aliases. */

/**** Prototypes *************************************************************/ 
int GetUserCount(NWCONN_HANDLE hServer);
int GetUsers(NWCONN_HANDLE hServer);
char * GetUserName(int iIndex);
