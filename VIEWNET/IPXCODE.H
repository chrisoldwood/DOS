/******************************************************************************
** (C) Chris Wood 1995.
**
** IPXCODE.H - Structures, defs & prototypes for using IPX.
**
******************************************************************************* 
*/

/**** Defintions *************************************************************/ 

#ifndef NULL
#define NULL   (void *) 0
#endif

/* Pointer to any data. */
typedef void * PVOID;

/* Alias'. */
typedef ConfigurationResponseStruct NODECFG;
typedef ConfigurationResponseStruct * PNODECFG;
typedef ExclusionPacketStructure EXCLNODES;

/* Max ECB data size. */
#define MAX_DATA_SIZE    sizeof(NODECFG)

/**** Prototypes *************************************************************/ 
void ScanNetwork(NWINET_ADDR *pNetwork);
NWGBOOL ScanSetup(NWINET_ADDR *pNetwork);
void ScanTerm(void);
void GetLocalNetwork(NWINET_ADDR *pLocalAddr);
void Yield(void);
