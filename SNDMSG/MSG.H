/******************************************************************************
** (C) Chris Wood 1995.
**
** MSG.H - Structures, defs & prototypes for sending messages.
**
******************************************************************************* 
*/

/**** Prototypes *************************************************************/ 
NWGBOOL GetMessage(char* pBuffer);
void SendMessage(NWCONN_HANDLE hServer, char* pUser, char* pMsg);
