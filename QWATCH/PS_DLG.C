/******************************************************************************
** (C) Chris Wood 1995.
**
** PS_DLG.C - Show and handle the print server dialog.
**
******************************************************************************* 
*/

#include <stdio.h>
#include <string.h>
#include <nwpsrv.h>
#include <nwcalls.h>
#include <nwgfx.h>
#include "fileserv.h"
#include "printjob.h"
#include "prtqueue.h"
#include "prntserv.h"
#include "nwerrs.h"
#include "ps_dlg.h"
#include "helpids.h"

#define PS_UPDATE   1                        /* Update time for dialog. */

typedef unsigned long ULONG;

/**** Global Vars. ***********************************************************/
static NWGDIALOG ServerDlg;                  /* The dialog. */
static int iIndex;                           /* Print server index. */
static PPRINTQUEUE pQueue;                   /* Queue we're interested in. */
static char PrinterName[50];                 /* The printers name. */
static char FormName[50];                    /* The mounted form. */
static char Status[30];                      /* Printer status. */
static char CopiesReq[10];                   /* Copies requested. */
static char CopiesDone[10];                  /* Copies done. */
static char CopySize[10];                    /* Size of a copy. */
static char BytesSent[10];                   /* Bytes sent. */
static char CopyBar[40];                     /* Copy graph bar. */
static char TotalBar[40];                    /* Total graph bar. */

static char *Graph = "±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±";
static char Block  = 'Û';

/**** Function Prototypes. ***************************************************/
NWGBOOL PrintServerDlgProc(NWGMSG wMsg, NWGNUM wCtrl, NWGVARPTR pValue);
void UpdatePSInfo(void);

/******************************************************************************
** Connect to and display info about the print server.
*/
void PrintServerDlg(NWOBJ_ID lServer, PPRINTQUEUE Queue)
{
     NWCCODE wRetVal;         /* NW return code. */

     /* Get the servers index. */
     iIndex = GetPSIndexByObjID(lServer);
     
     /* Validate. */
     if (iIndex < 0)
     {
          /* Display error. */
          NWGFXInfoMessage("Unrecognised print server");
          return;
     }
     
     /* Save the Queue globally. */
     pQueue = Queue;
     
     /* Show wait message. */
     NWGFXWaitMessage(TRUE);

     /* Connect to Print Server. */
     wRetVal = PSConnect(iIndex);
     if (wRetVal)
     {
          /* Hide wait message. */
          NWGFXWaitMessage(FALSE);

          /* Display error. */
          NWGFXInfoMessage(ErrToString(wRetVal));

          /* Disconnect. */
          PSDisconnect(iIndex);
          return;
     }

     /* Hide wait message. */
     NWGFXWaitMessage(FALSE);

     /* Setup the data fields. */
     PSControls[SERVERNAME].pValue  = GetPSName(iIndex);
     PSControls[PRINTERNAME].pValue = PrinterName;
     PSControls[FORMNAME].pValue    = FormName;
     PSControls[STATUS].pValue      = Status;
     PSControls[TOTCOPIES].pValue   = CopiesReq;
     PSControls[DONECOPIES].pValue  = CopiesDone;
     PSControls[TOTSIZE].pValue     = CopySize;
     PSControls[DONESIZE].pValue    = BytesSent;
     PSControls[COPYBAR].pValue     = CopyBar;
     PSControls[TOTALBAR].pValue    = TotalBar;
     
     /* Get current status. */
     UpdatePSInfo();
     
     /* Fill in the dialog details. */
     ServerDlg.iPosType  = DP_CENTRED;
     ServerDlg.iSX       = 40;
     ServerDlg.iSY       = 19;
     ServerDlg.pTitle    = "Printer/Print Server Info";
     ServerDlg.tTimeOut  = PS_UPDATE;
     ServerDlg.fnDlgProc = PrintServerDlgProc;
     ServerDlg.iNumCtls  = PS_CONTROLS;
     ServerDlg.pControls = PSControls;
     ServerDlg.iCurrent  = 0;
     ServerDlg.iHelpID   = IDH_PSINFO;

     /* Display the dialog. */
     NWGFXDialog(&ServerDlg);
     
     /* Disconnect. */
     PSDisconnect(iIndex);
}

/******************************************************************************
** Print server dialog callback.
*/
NWGBOOL PrintServerDlgProc(NWGMSG wMsg, NWGNUM wCtrl, NWGVARPTR pValue)
{
     NWGNUM    wValue;                  /* Menu calue. */
     
     /* Decode dialog message. */
     switch(wMsg)
     {
          /* Check for change. */
          case DIALOG_CTL_DONE:
               break;
               
          /* Timeout... */
          case DIALOG_TIMEOUT:
               /* Time to update. */
               UpdatePSInfo();
               return TRUE;
               
          /* Safe. */
          default:
               break;
     }
     
     /* Don't update. */
     return FALSE;
}

/******************************************************************************
** Convert the status to a string.
*/
char * StatusToStr(BYTE uStatus)
{
     switch(uStatus)
     {
          case NWPS_PSTAT_JOB_WAIT:
               return "Waiting";

          case NWPS_PSTAT_FORM_WAIT:
               return "Waiting";

          case NWPS_PSTAT_PRINTING:
               return "Printing";

          case NWPS_PSTAT_PAUSED:
               return "Paused";

          case NWPS_PSTAT_STOPPED:
               return "Stopped";

          case NWPS_PSTAT_MARK_EJECT:
               return "Mark Eject";

          case NWPS_PSTAT_READY_TO_DOWN:
               return "Downing";

          case NWPS_PSTAT_NOT_CONNECTED:
               return "NOT CONNECTED";
     }
     
     return "Unknown problem";
}

/******************************************************************************
** Convert the trouble code to a string.
*/
char * TroubleToStr(BYTE uProblem)
{
     switch(uProblem)
     {
          case NWPS_PRINTER_RUNNING:
               return "Printing";

          case NWPS_PRINTER_OFFLINE:
               return "OFFLINE";

          case NWPS_PRINTER_PAPER_OUT:
               return "NO PAPER";
     }
     
     return "Unknown problem";
}

/******************************************************************************
** Update the info about the print server and printer.
*/
void UpdatePSInfo(void)
{
     NWCCODE   wRetVal;                      /* NW return code. */
     int       iPrinterID;                   /* Printer servicing the queue. */
     WORD      wSPXConnID;                   /* Server SPX connection ID. */
     BYTE      uStatus;                      /* Printer status. */
     BYTE      uProbCode;                    /* Trouble code. */
     BYTE      bActive;                      /* Active flag. */
     BYTE      uMode;                        /* Service mode. */
     WORD      wForm;                        /* Mounted/Job form. */
     char      ServName[50];                 /* Server name. */
     char      QueueName[50];                /* Print queue name. */
     char      JobTitle[50];                 /* Job description. */
     WORD      wJobID;                       /* Job ID. */
     WORD      wCopiesReq, wCopiesDone;      /* Number of copies requested/done. */
     ULONG     lJobSize, lBytesSent;         /* Job size/bytes sent. */
     ULONG     lTotal, lTotalSent;           /* Total job size/bytes sent. */
     BYTE      bTabs;                        /* Expand tabs? */
     int       iLoop, iBlocks;               /* Graph counter. */
     
     /* Get the printer servicing the queue. */
     iPrinterID = GetQueuesPrinter(iIndex, pQueue);
     if (iPrinterID == -1)
     {
          /* Show no printer attached. */
          strcpy(PrinterName, "(none connected)");

          /* Blank other fields. */
          strcpy(PrinterName, "");
          strcpy(FormName, "");
          strcpy(Status, "");
          strcpy(CopiesReq, "");
          strcpy(CopiesDone, "");
          strcpy(CopySize, "");
          strcpy(BytesSent, "");
          strcpy(CopyBar, Graph);
          strcpy(TotalBar, Graph);
          
          /* Set dialog style. */
          ServerDlg.fFlags = DF_STATIC | DF_TIMER;   
     }
     else
     {
          /* Get the SPX connection ID. */
          wSPXConnID = GetPSSPXConn(iIndex);
     
          /* Get printer stats. */
          wRetVal = NWPSComGetPrinterStatus(wSPXConnID, (WORD)iPrinterID, (BYTE NWFAR*) &uStatus, 
               (BYTE NWFAR*) &uProbCode, (BYTE NWFAR*) &bActive, (BYTE NWFAR*) &uMode,
               (WORD NWFAR*) &wForm, (char NWFAR*) FormName, (char NWFAR*) PrinterName);

          /* Get form. */
          sprintf(FormName, "%d", wForm);
          
          /* Check status. */
          if (bActive)
          {
               /* Show status. */
               if (uProbCode)
                    strcpy(Status, TroubleToStr(uProbCode));
               else
                    strcpy(Status, StatusToStr(uStatus));

               /* Set dialog style. */
               ServerDlg.fFlags = DF_STATIC | DF_TIMER;
               
               wRetVal = NWPSComGetPrintJobStatus(wSPXConnID, (WORD)iPrinterID,
                    (char NWFAR*) ServName, (char NWFAR*) QueueName, (WORD NWFAR*) &wJobID,
                    (char NWFAR*) JobTitle, (WORD NWFAR*) &wCopiesReq, (DWORD NWFAR*) &lJobSize,
                    (WORD NWFAR*) &wCopiesDone, (DWORD NWFAR*) &lBytesSent, (WORD NWFAR*) &wForm,
                    (BYTE NWFAR*) &bTabs);
               
               /* Set copies requested. */
               sprintf(CopiesReq, "%d", wCopiesReq);

               /* Set copies done/current. */
               sprintf(CopiesDone, "%d", wCopiesDone);

               /* Set job size. */
               sprintf(CopySize, "%ld", lJobSize);

               /* Set bytes sent. */
               sprintf(BytesSent, "%ld", lBytesSent);

               /* Create basic copy graph. */
               strcpy(CopyBar, Graph);
               
               /* Calculate bytes sent. */
               iBlocks = (int) ((lBytesSent << 5) / lJobSize);
               
               /* Show done. */
               for (iLoop=0; iLoop < iBlocks; iLoop++)
                    CopyBar[iLoop] = Block;

               /* Check job size (1MB). */
               if (lJobSize > 1000000L)
               {
                    /* Knobble sizes to stop overflow. */
                    lJobSize   >>= 7;
                    lBytesSent >>= 7;
               }
               
               /* Calculate total job size and bytes sent. */
               lTotal     = lJobSize * wCopiesReq;
               lTotalSent = (lJobSize * (wCopiesDone)) + lBytesSent;
               
               /* Create basic total graph. */
               strcpy(TotalBar, Graph);
               
               /* Calculate total done. */
               iBlocks = (int) ((lTotalSent << 5) / lTotal);
               
               /* Create graph. */
               for (iLoop=0; iLoop < iBlocks; iLoop++)
                    TotalBar[iLoop] = Block;
          }
          else /* Not active. */
          {
               /* Show status. */
               if (uProbCode)
                    strcpy(Status, TroubleToStr(uProbCode));
               else
                    strcpy(Status, StatusToStr(uStatus));

               /* Set dialog style. */
               ServerDlg.fFlags = DF_STATIC | DF_TIMER;

               /* Blank other fields. */
               strcpy(CopiesReq, "");
               strcpy(CopiesDone, "");
               strcpy(CopySize, "");
               strcpy(BytesSent, "");
               strcpy(CopyBar, Graph);
               strcpy(TotalBar, Graph);
          }
     }
}
