/******************************************************************************
** (C) Chris Wood 1995.
**
** LPT_DLG.C - Show and handle the LPT settings dialog.
**
******************************************************************************* 
*/

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <nwcalls.h>
#include <nwpsrv.h>
#include <nwgfx.h>
#include "nwerrs.h"
#include "helpids.h"
#include "lpt_dlg.h"
#include "fileserv.h"
#include "prtforms.h"
#include "prtqueue.h"

#define PF_FLUSH    4
#define PF_NOFFEED  8     
#define PF_DOTABS   64
#define PF_BANNER   128

/**** Global Vars. ***********************************************************/
static NWCAPTURE_FLAGS1 Flags1;              /* NW capture settings. */
static NWCAPTURE_FLAGS2 Flags2;              /* NW capture settings. */
static char Status[5];                       /* Capture status. */
static char ServerName[50];                  /* File server name. */
static char QueueName[50];                   /* Printer queue name. */
static char DoTabs[5];                       /* Expand tabs. */
static char TabSize[5];                      /* Tab size. */
static char DoBanner[5];                     /* Print banner. */
static char Copies[5];                       /* Number of copies. */
static char FormFeed[5];                     /* Do a form feed. */
static char Lines[5];                        /* Page height. */
static char Width[5];                        /* Page width. */
static char *YesStr = "YES";                 /* Yes string. */
static char *NoStr = "NO";                   /* No string. */
static char *NullStr = "";                   /* NULL string. */
static int  iLPTPort;                        /* The port we are looking at. */

extern NWGDIM  wDispHeight;                  /* The display height in chars. */
extern NWGMENUITEM FormItems[MAX_PRINT_FORMS];    
                                             /* Form names for menu. */

/**** Function Prototypes. ***************************************************/
NWGBOOL SettingsDlgProc(NWGMSG wMsg, NWGNUM wCtrl, NWGVARPTR pValue);
void GetPortInfo();

/******************************************************************************
** Show the settings for the printer port.
*/
void SettingsDlg(int iPort)
{
     NWGDIALOG PortDlg;                 /* The dialog. */

     /* Save the port number. */
     iLPTPort = iPort;
     
     /* Get current status. */
     GetPortInfo();
     
     /* Fill in the dialog details. */
     PortDlg.iPosType  = DP_CENTRED;
     PortDlg.iSX       = 32;
     PortDlg.iSY       = 19;
     PortDlg.fFlags    = DF_BASIC;   
     PortDlg.pTitle    = "Printer Port Settings";
     PortDlg.tTimeOut  = 0;
     PortDlg.fnDlgProc = SettingsDlgProc;
     PortDlg.iNumCtls  = LPT_CONTROLS;
     PortDlg.pControls = LPTControls;
     PortDlg.iCurrent  = STATUS+1;
     PortDlg.iHelpID   = IDH_LPTDIALOG;

     /* Display the dialog. */
     NWGFXDialog(&PortDlg);
}

/******************************************************************************
** Settings dialog callback.
*/
NWGBOOL SettingsDlgProc(NWGMSG wMsg, NWGNUM wCtrl, NWGVARPTR pValue)
{
     NWGBOOL   bYes;                    /* Menu answer. */
     NWCONN_HANDLE hServer;             /* Default server. */
     NWOBJ_ID  QueueID;                 /* Default queue ID. */
     char      QueueName[50];           /* Default queue name. */
     DWORD     FormID;                  /* Default form ID. */
     char      FormName[50];            /* Default form name. */
     WORD      FormNumber;              /* Default form type. */
     WORD      FormLength;              /* Default form length. */
     WORD      FormWidth;               /* Default form width. */
     int       iSelected;               /* Menu item selected. */
     int       iNumForms;               /* Number of forms. */
     int       iMaxNameLen;             /* Max form name length. */
     int       iNumQueues;              /* Number of queues to watch. */
     NWGMENUITEMS QItems;               /* Array of menu items. */
     PQ_LIST   QList;                   /* Print queue list. */
     NWGMENU   Queues;                  /* The menu. */
     int       iNumConns, iLoop;        /* Number of file server connections. */
     int       iQueueCnt;               /* Total number of queues. */
     
     /* Decode dialog message. */
     switch(wMsg)
     {
          /* Check for change. */
          case DIALOG_CTL_DONE:
               if (wCtrl == SHOWTABS)
               {
                    /* Get item selected. */
                    iSelected = (int)(long) pValue;
                    
                    /* Get default connection ID. */
                    NWGetConnectionHandle((BYTE NWFAR*) ServerName, 0, &hServer, NULL);

                    /* Was it No? */
                    if (iSelected == 1)
                    {
                         /* Change setting. */
                         Flags1.printFlags &= ~PF_DOTABS;
                    }
                    else
                    {
                         /* Change setting. */
                         Flags1.printFlags |= PF_DOTABS;
                    }

                    /* Update setting. */
                    NWSetCaptureFlags(hServer, (NWLPT) iLPTPort, &Flags1);

                    /* Update dialog. */
                    GetPortInfo();
               }
               else if (wCtrl == SHOWBANNER)
               {
                    /* Get item selected. */
                    iSelected = (int)(long) pValue;
                    
                    /* Get default connection ID. */
                    NWGetConnectionHandle((BYTE NWFAR*) ServerName, 0, &hServer, NULL);

                    /* Was it No? */
                    if (iSelected == 1)
                    {
                         /* Change setting. */
                         Flags1.printFlags &= ~PF_BANNER;
                    }
                    else
                    {
                         /* Change setting. */
                         Flags1.printFlags |= PF_BANNER;
                    }

                    /* Update setting. */
                    NWSetCaptureFlags(hServer, (NWLPT) iLPTPort, &Flags1);

                    /* Update dialog. */
                    GetPortInfo();
               }
               else if (wCtrl == FORMFEED)
               {
                    /* Get item selected. */
                    iSelected = (int)(long) pValue;
                    
                    /* Get default connection ID. */
                    NWGetConnectionHandle((BYTE NWFAR*) ServerName, 0, &hServer, NULL);

                    /* Was it No? */
                    if (iSelected == 1)
                    {
                         /* Change setting. */
                         Flags1.printFlags |= PF_NOFFEED;
                    }
                    else
                    {
                         /* Change setting. */
                         Flags1.printFlags &= ~PF_NOFFEED;
                    }

                    /* Update setting. */
                    NWSetCaptureFlags(hServer, (NWLPT) iLPTPort, &Flags1);

                    /* Update dialog. */
                    GetPortInfo();
               }
               break;

          /* Custom control. */
          case DIALOG_CTL_EXEC:
               if (wCtrl == STATUS)
               {
                    /* Are we capturing? */
                    if (strcmp(LPTControls[STATUS].pValue, "Captured") == 0)
                    {
                         /* Choose whether to stop capture. */
                         bYes = NWGFXQueryMenu("Remove Capture", FALSE);
                         if (bYes)
                         {
                              /* Remove capture. */
                              NWCancelCapture((NWLPT) iLPTPort);
                         
                              /* Update dialog. */
                              GetPortInfo();
                              return TRUE;
                         }
                    }
                    else /* Not capturing. */
                    {
                         /* Choose whether to start capture. */
                         bYes = NWGFXQueryMenu("Start Capture ", FALSE);
                         if (bYes)
                         {
                              /* Get default connection ID. */
                              NWGetDefaultConnectionID(&hServer);

                              /* Get first queue ID. */
                              QueueID = (NWOBJ_ID) -1;
                              NWScanObject(hServer, "*", OT_PRINT_QUEUE, &QueueID, QueueName,
                                             NULL, NULL, NULL, NULL);
                              
                              /* Get first form. */
                              FormID = (DWORD) -1;
                              NWPSPdfScanForm(NWPS_BINDERY_SERVICE_PRE_40, hServer, &FormID, 
                                             FormName);

                              /* Get form info. */
                              NWPSPdfReadForm(NWPS_BINDERY_SERVICE_PRE_40, hServer, FormName,
                                             &FormNumber, &FormLength, &FormWidth);
                              
                              /* Setup default capture settings. */
                              strcpy((NWGSTR) Flags1.jobDescription, "");
                              Flags1.jobControlFlags = 0;
                              Flags1.tabSize = 8;
                              Flags1.numCopies = 1;
                              Flags1.printFlags = PF_FLUSH | PF_DOTABS | PF_BANNER;
                              Flags1.maxLines = FormLength;
                              Flags1.maxChars = FormWidth;
                              strcpy((NWGSTR) Flags1.formName, FormName);
                              Flags1.formType = FormNumber;
                              strcpy((NWGSTR) Flags1.bannerText, "LST:");
                              Flags1.flushCaptureTimeout = 0x0000;
                              Flags1.flushCaptureOnClose = 0x0000;
                              
                              /* Start capture. */
                              NWSetCaptureFlags(hServer, (NWLPT) iLPTPort, &Flags1);
                              NWStartQueueCapture(hServer, (NWLPT) iLPTPort, QueueID, QueueName);
                              
                              /* Update dialog. */
                              GetPortInfo();
                              return TRUE;
                         }
                    }
               }
               else if (wCtrl == QUEUENAME)
               {
                    /* Show wait message. */
                    NWGFXWaitMessage(TRUE);

                    /* Reset number of queues found. */
                    iNumQueues = 0;
                    QItems = NULL;
     
                    /* Get number of file servers. */
                    iNumConns = GetNumFSConnections();

                    /* Get number of queues of all servers. */
                    for(iLoop=0; iLoop < iNumConns; iLoop++)
                    {
                         /* Find out how many queues are on this server. */
                         iNumQueues += GetNumPrintQueues(GetFSConnHandle(iLoop));
                    }
                    
                    /* Check to see if we found any. */
                    if (!iNumQueues)
                    {
                         /* Hide wait message. */
                         NWGFXWaitMessage(FALSE);

                         /* Show error message. */
                         NWGFXInfoMessage("No Printer Queues Found");
                         return FALSE;
                    }

                    /* Allocate memory for menu items. */
                    QItems = (NWGMENUITEMS) malloc(sizeof(NWGMENUITEM) * iNumQueues);
                    if (!QItems)
                    {
                         /* Hide wait message. */
                         NWGFXWaitMessage(FALSE);

                         /* Show error message. */
                         NWGFXErrorMessage("Malloc failure!");
                         return FALSE;
                    }
     
                    /* Allocate memory for print queue list. */
                    QList = (PPRINTQUEUE) malloc(sizeof(PRINTQUEUE) * iNumQueues);
                    if (!QList)
                    {
                         /* Hide wait message. */
                         NWGFXWaitMessage(FALSE);

                         /* Show error message. */
                         NWGFXErrorMessage("Malloc failure!");
                         return FALSE;
                    }

                    /* Reset queue count. */
                    iQueueCnt = 1;
                    
                    /* Get queues for each of the servers. */
                    for(iLoop=0; iLoop < iNumConns; iLoop++)
                         iQueueCnt += GetPrintQueues(GetFSConnHandle(iLoop), &QList[iQueueCnt-1]);

                    /* Set menu items pointer to queue info string. */
                    for (iLoop=0; iLoop < iNumQueues; iLoop++)
                         QItems[iLoop].pItem = QList[iLoop].Name;
     
                    /* Fill in menu details. */
                    Queues.iPosType   = MP_CENTRED;  
                    Queues.iSX        = 36;

                    /* Calculate menu height to fit all entries if possible. */
                    if (iNumQueues > wDispHeight-10)
                         Queues.iSY = wDispHeight - 10;   
                    else
                         Queues.iSY = iNumQueues + 4;   

                    Queues.fFlags     = MF_BASIC;    
                    Queues.pTitle     = "Set Queue";    
                    Queues.tTimeOut   = (NWGTIME) 0;  
                    Queues.fnMenuProc = (NWGMENUPROC) NULL;
                    Queues.iNumItems  = iNumQueues; 
                    Queues.pItems     = QItems;    
                    Queues.iCurrent   = 0;    
                    Queues.iHelpID    = IDH_QUEUENAME;    

                    /* Hide wait message. */
                    NWGFXWaitMessage(FALSE);

                    /* Display queue menu. */
                    iSelected = NWGFXPopupMenu(&Queues);
                    
                    /* Check for cancel. */
                    if (iSelected)
                    {
                         /* Change to index. */
                         iSelected--;

                         /* Remove current capture. */
                         NWCancelCapture((NWLPT) iLPTPort);

                         /* Update setting. */
                         NWStartQueueCapture(QList[iSelected].ConnID, (NWLPT) iLPTPort, 
                              QList[iSelected].QueueID, QList[iSelected].Name);

                         /* Update dialog. */
                         GetPortInfo();
                    }

                    /* Free menu items/print queue list memory. */
                    free(QItems);
                    free(QList);
               }
               else if (wCtrl == FORMNAME)
               {
                    /* Get default connection ID. */
                    NWGetConnectionHandle((BYTE NWFAR*) ServerName, 0, &hServer, NULL);

                    /* Get a list of the supported forms. */
                    GetForms(hServer);

                    /* Get total number of forms. */
                    iNumForms = GetNumForms();

                    /* Calculate form menu width (adjust for borders). */
                    iMaxNameLen = GetFormMaxNameLen() + 4;
                    if (iMaxNameLen > 12)
                         FormMenu.iSX = iMaxNameLen;
                    else
                         FormMenu.iSX = 12;

                    /* Calculate menu height to fit all entries if possible. */
                    if (iNumForms > wDispHeight-10)
                         FormMenu.iSY        = wDispHeight - 10;   
                    else
                         FormMenu.iSY        = iNumForms + 4;   
    
                    /* Finish off menu details. */
                    FormMenu.iNumItems = iNumForms; 
                    FormMenu.pItems    = FormItems;
                    
                    /* Show menu. */
                    iSelected = NWGFXPopupMenu(&FormMenu);
                    
                    /* Check for selection. */
                    if (iSelected)
                    {
                         /* Change to index. */
                         iSelected--;
                         
                         /* Change settings. */
                         strcpy((NWGSTR) Flags1.formName, GetFormName(iSelected));
                         Flags1.maxLines = GetFormLength(iSelected);
                         Flags1.maxChars = GetFormWidth(iSelected);
                         Flags1.formType = GetFormNumber(iSelected);
                    
                         /* Update setting. */
                         NWSetCaptureFlags(hServer, (NWLPT) iLPTPort, &Flags1);

                         /* Update dialog. */
                         GetPortInfo();
                    }
               }
               break;
               
          /* Safe. */
          default:
               break;
     }
     
     /* Don't update. */
     return FALSE;
}

/******************************************************************************
** Get the settings for the specified port.
*/
void GetPortInfo(void)
{
     NWCCODE wRetVal;                   /* NW return code. */
     
     /* Check for capture. */
     wRetVal = NWGetCaptureStatus((NWLPT) iLPTPort);
     if (wRetVal)
     {
          /* Get the capture settings. */
          wRetVal = NWGetCaptureFlags((NWLPT) iLPTPort, (NWCAPTURE_FLAGS1 NWFAR*) &Flags1, 
                         (NWCAPTURE_FLAGS2 NWFAR*) &Flags2);

          /* Set control info. */
          LPTControls[STATUS].pValue     = "Captured";
          LPTControls[SERVERNAME].pValue = ServerName;
          LPTControls[QUEUENAME].pValue  = QueueName;
          LPTControls[FORMNAME].pValue   = (NWGSTR) Flags1.formName;
          LPTControls[SHOWTABS].pValue   = DoTabs;
          LPTControls[TABSIZE].pValue    = TabSize;
          LPTControls[SHOWBANNER].pValue = DoBanner;
          LPTControls[BANNERNAME].pValue = (NWGSTR) Flags1.bannerText;
          LPTControls[NUMCOPIES].pValue  = Copies;
          LPTControls[FORMFEED].pValue   = FormFeed;
          LPTControls[PAGEHEIGHT].pValue = Lines;
          LPTControls[PAGEWIDTH].pValue  = Width;

          /* Ensure controls can be activated. */
          LPTControls[QUEUENAME].iType  = DCTL_CUSTOM;
          LPTControls[FORMNAME].iType   = DCTL_CUSTOM;
          LPTControls[SHOWTABS].iType   = DCTL_MENU;
          LPTControls[TABSIZE].iType    = DCTL_STEXT;
          LPTControls[SHOWBANNER].iType = DCTL_MENU;
          LPTControls[BANNERNAME].iType = DCTL_STEXT;
          LPTControls[NUMCOPIES].iType  = DCTL_STEXT;
          LPTControls[FORMFEED].iType   = DCTL_MENU;
          LPTControls[PAGEHEIGHT].iType = DCTL_STEXT;
          LPTControls[PAGEWIDTH].iType  = DCTL_STEXT;
          
          /* Get server name. */
          NWGetFileServerName(Flags2.connID, (char NWFAR*) ServerName);
          
          /* Get queue name. */
          NWGetObjectName(Flags2.connID, Flags2.queueID, (char NWFAR*) QueueName, NULL);
          
          /* Get whether expanding tabs. */
          if (Flags1.printFlags & 64)
               strcpy(DoTabs, YesStr);
          else
               strcpy(DoTabs, NoStr);
          
          /* Get tab size. */
          sprintf(TabSize, "%d", (int) Flags1.tabSize);
          
          /* Get whether to print banner. */
          if (Flags1.printFlags & 128)
               strcpy(DoBanner, YesStr);
          else
               strcpy(DoBanner, NoStr);
          
          /* Get number of copies. */
          sprintf(Copies, "%d", Flags1.numCopies);
          
          /* Get whether to do a form feed. */
          if (Flags1.printFlags & 8)
               strcpy(FormFeed, NoStr);
          else
               strcpy(FormFeed, YesStr);
          
          /* Get lines per page. */
          sprintf(Lines, "%d", Flags1.maxLines);
          
          /* Get page width. */
          sprintf(Width, "%d", Flags1.maxChars);
     }
     else
     {
          /* Set control info. */
          LPTControls[STATUS].pValue     = "Not Captured";
          LPTControls[SERVERNAME].pValue = NullStr;
          LPTControls[QUEUENAME].pValue  = NullStr;
          LPTControls[FORMNAME].pValue   = NullStr;
          LPTControls[SHOWTABS].pValue   = NullStr;
          LPTControls[TABSIZE].pValue    = NullStr;
          LPTControls[SHOWBANNER].pValue = NullStr;
          LPTControls[BANNERNAME].pValue = NullStr;
          LPTControls[NUMCOPIES].pValue  = NullStr;
          LPTControls[FORMFEED].pValue   = NullStr;
          LPTControls[PAGEHEIGHT].pValue = NullStr;
          LPTControls[PAGEWIDTH].pValue  = NullStr;
          
          /* Stop controls being activated. */
          LPTControls[QUEUENAME].iType  = DCTL_STEXT;
          LPTControls[FORMNAME].iType   = DCTL_STEXT;
          LPTControls[SHOWTABS].iType   = DCTL_STEXT;
          LPTControls[TABSIZE].iType    = DCTL_STEXT;
          LPTControls[SHOWBANNER].iType = DCTL_STEXT;
          LPTControls[BANNERNAME].iType = DCTL_STEXT;
          LPTControls[NUMCOPIES].iType  = DCTL_STEXT;
          LPTControls[FORMFEED].iType   = DCTL_STEXT;
          LPTControls[PAGEHEIGHT].iType = DCTL_STEXT;
          LPTControls[PAGEWIDTH].iType  = DCTL_STEXT;
     }
}
