/******************************************************************************
** (C) Chris Wood 1995.
**
** IPXCODE.H - Structures, defs & prototypes for using IPX.
**
******************************************************************************* 
*/

/**** Global Defs ************************************************************/ 

/**** Global Structures ******************************************************/ 

/**** Prototypes *************************************************************/ 
NWGBOOL IPXSetup(NWCONN_NUM wConn);
void IPXShutDown(void);
NWGBOOL ReceivedData(void);
void ReceiveData(char *pBuffer);
NWGBOOL SentData(void);
void SendData(char *pBuffer);
void Yield(void);
