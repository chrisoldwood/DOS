/******************************************************************************
** (C) Chris Wood 1995.
**
** PRTFORMS.C - Functions to access print forms.
**
******************************************************************************* 
*/

#include <string.h>
#include <nwcalls.h>
#include <nwps_pdf.h>
#include "printjob.h"
#include "prtqueue.h"
#include "prtforms.h"
#include "fileserv.h"
#include "nwerrs.h"
#include <nwgfx.h>

/**** Global Vars. ***********************************************************/
static PRINTFORM Forms[MAX_PRINT_FORMS];    
                                        /* List of supported forms. */
static NWNUMBER wNumForms;              /* Number of forms found. */

NWGMENUITEM FormItems[MAX_PRINT_FORMS]; /* For the form menu. */

/******************************************************************************
** Get a list of the supported forms.
*/
void GetForms(PPRINTQUEUE pQueue)
{
     NWCCODE   wFailure = 0;            /* List end flag. */
     DWORD     Sequence = (DWORD) -1;   /* Sequence number; -1 is for find first. */

     /* Reset queue count. */
     wNumForms = 0;
          
     /* Search until list exhausted. */
     while(!wFailure)
     {
          /* Search for next. */
          wFailure = NWPSPdfScanForm(NWPS_BINDERY_SERVICE_PRE_40, pQueue->ConnID, &Sequence,
                         (char NWFAR *) Forms[wNumForms].Name);

          /* Check for last form. */
          if (!wFailure)
          {
               /* Get full form description. */
               NWPSPdfReadForm(NWPS_BINDERY_SERVICE_PRE_40, pQueue->ConnID, 
                    (char NWFAR *) Forms[wNumForms].Name, (WORD NWFAR *) &Forms[wNumForms].Number, 
                    (WORD NWFAR *) &Forms[wNumForms].Length, (WORD NWFAR *) &Forms[wNumForms].Width);
     
               /* Fill in the menu items. */
               FormItems[wNumForms].pItem = Forms[wNumForms].Name;
               
               /* Inc. total. */
               wNumForms++;
          }
          else if (wFailure != 0x89FC)
          {
               NWGFXInfoMessage(ErrToString(wFailure));
          }
     }
}

/******************************************************************************
** Get the number of forms in the list.
*/
int GetNumForms(void)
{
     return (int) wNumForms;
}

/******************************************************************************
** Get the name of a form by index.
*/
char * GetFormName(int iIndex)
{
     return Forms[iIndex].Name;
}

/******************************************************************************
** Get the number of a form by index.
*/
WORD GetFormNumber(int iIndex)
{
     return Forms[iIndex].Number;
}

/******************************************************************************
** Get the longest form name.
*/
int GetFormMaxNameLen(void)
{
     int iLen, iMax=0;        /* Current, Max name length. */
     int iLoop;               /* Server counter. */
     
     /* Go through the list. */
     for (iLoop=0; iLoop < (int) wNumForms; iLoop++)
     {
          /* Find current name length. */
          iLen = strlen(Forms[iLoop].Name);
          
          /* Largest? */
          if (iLen > iMax)
               iMax = iLen;
     }

     return iMax;
}
