/******************************************************************************
** (C) Chris Wood 1995.
**
** WSDLG.C - Show and handle the workstation dialog.
**
******************************************************************************* 
*/

#include <stdio.h>
#include <nwcalls.h>
#include <diag.h>
#include <nwgfx.h>
#include <string.h>
#include <memory.h>
#include "nodes.h"
#include "helpids.h"
#include "wsdlg.h"

/**** Global Vars. ***********************************************************/
static BeginDiagnosticStruct  DiagStruct;    /* Diagnostic info. */
static AllResponseData        ResData;       /* Response structure. */
static ShellVersionStruct     ShellVer;      /* Shell versions. */
static IPXStatisticsStruct    IPXStats;      /* IPX statistics. */
static SPXStatisticsStruct    SPXStats;      /* SPX statistics. */
static OSVersionStruct        OSVersion;     /* OS Version. */
static ServerNameTableStruct  ServerNames;   /* List of server names. */
static IPXSPXVersion          IPXSPXVer;     /* IPX/SPX versions. */

static char szName[10];                      /* Nodes name. */
static char szAddress[20];                   /* Nodes' address. */
static char szShellVersion[20];              /* Nodes' shell version. */
static char szIPXVersion[20];                /* Nodes' IPX version. */
static char szSPXVersion[20];                /* Nodes' SPX version. */
static char szIPXSent[20];                   /* IPX packets sent. */
static char szIPXMalformed[20];              /* IPX packets malformed. */
static char szSPXSent[20];                   /* SPX packets sent. */
static char szSPXFailed[20];                 /* SPX packets failed. */
static char szSPXReceived[20];               /* SPX packets received. */

/**** Function Prototypes. ***************************************************/
void GetWorkstationInfo(PNETNODE pNode);

/******************************************************************************
** Show the settings for the workstation.
*/
void WSDialog(PNETNODE pNode)
{
     NWGDIALOG Dialog;

     /* Get info. */
     GetWorkstationInfo(pNode);
     
     /* Fill in the dialog details. */
     Dialog.iPosType  = DP_CENTRED;
     Dialog.iSX       = 40;
     Dialog.iSY       = 18;
     Dialog.fFlags    = DF_STATIC;   
     Dialog.pTitle    = "Workstation Info";
     Dialog.tTimeOut  = 0;
     Dialog.fnDlgProc = NULL;
     Dialog.iNumCtls  = WS_CONTROLS;
     Dialog.pControls = WSControls;
     Dialog.iCurrent  = 0;
     Dialog.iHelpID   = IDH_SCAN;

     /* Display the dialog. */
     NWGFXDialog(&Dialog);
}

/******************************************************************************
** Get the info on a workstation.
** I think that CompList[0] is actually the number of components in the list,
** so we return index - 1 to get the actual component position.
*/
BYTE GetComponentIndex(BYTE *CompList, BYTE iNumComponents, BYTE iComponent)
{
     BYTE iLoop;
     
     /* Loop through components. */
     for (iLoop=1; iLoop <= iNumComponents; iLoop++)
     {
          /* Found it? */
          if (CompList[iLoop] == iComponent)
               return (BYTE) (iLoop-1);
     }
     
     /* Not found. */
     return 0xFF;
}

/******************************************************************************
** Get the info on a workstation.
*/
void GetWorkstationInfo(PNETNODE pNode)
{
     int  iRetVal;                           /* Diagnostics return code. */
     WORD wConnID;                           /* SPX Connection ID. */
     BYTE uCompList[54];                     /* Target component list. */
     BYTE iCompIDX;                          /* Component index. */

     /* Show wait message. */
     NWGFXWaitMessage(TRUE);
     
     /* Set nodes' name. */
     strcpy(szName, pNode->szName);
     WSControls[NODE_NAME].pValue = szName;

     /* Set nodes' address. */
     sprintf(szAddress, "%.2X%.2X%.2X%.2X%.2X%.2X", (int) pNode->Address[0],
               (int) pNode->Address[1], (int) pNode->Address[2],
               (int) pNode->Address[3], (int) pNode->Address[4],
               (int) pNode->Address[5]);
     WSControls[NODE_ADDRESS].pValue = szAddress;

     /* Setup diagnostic structure. */
     memcpy(DiagStruct.network, pNode->Network, 4);
     memcpy(DiagStruct.node, pNode->Address, 6);
     memcpy(DiagStruct.socket, pNode->Socket, 2);
     
     /* Start diagnostic calls. */
     iRetVal = BeginDiagnostics(&DiagStruct, &wConnID, uCompList);
     
     /* Get the Shell version info. */
     iCompIDX = GetComponentIndex(uCompList, (BYTE) pNode->iNumComps, SHELL_COMPONENT);
     iRetVal  = GetShellVersionInfo(wConnID, iCompIDX, &ResData, &ShellVer);
     iRetVal  = GetOSVersionInfo(wConnID, iCompIDX, &ResData, &OSVersion);
     iRetVal  = GetServerNameTable(wConnID, iCompIDX, &ResData, &ServerNames);

     /* Get IPXSPX Info. */
     iCompIDX = GetComponentIndex(uCompList, (BYTE) pNode->iNumComps, IPX_SPX_COMPONENT);
     iRetVal  = GetIPXSPXVersion(wConnID, iCompIDX, &ResData, &IPXSPXVer);
     iRetVal  = GetIPXStatistics(wConnID, iCompIDX, &ResData, &IPXStats);
     iRetVal  = GetSPXStatistics(wConnID, iCompIDX, &ResData, &SPXStats);

     /* End diagnostic call. */
     EndDiagnostics(wConnID);
     
     /* Set shell version. */
     sprintf(szShellVersion, "V%d.%d", (int) ShellVer.major, (int) ShellVer.minor);
     WSControls[SHELL_VERSION].pValue = szShellVersion;

     /* Set IPX version. */
     sprintf(szIPXVersion, "V%d.%d", (int) IPXSPXVer.IPXMajorVersion, 
                                     (int) IPXSPXVer.IPXMinorVersion);
     WSControls[IPX_VERSION].pValue = szIPXVersion;

     /* Set SPX version. */
     sprintf(szSPXVersion, "V%d.%d", (int) IPXSPXVer.SPXMajorVersion, 
                                     (int) IPXSPXVer.SPXMinorVersion);
     WSControls[SPX_VERSION].pValue = szSPXVersion;

     /* Hide wait message. */
     NWGFXWaitMessage(FALSE);
}
