/******************************************************************************
** (C) Chris Wood 1995.
**
** NWERRS.C - My NetWare error routines.
**
******************************************************************************* 
*/

#include <stdio.h>
#include <nwcalls.h>

/**** Global Vars. ***********************************************************/
static char szErrorString[80];          /* Error value as string. */

/******************************************************************************
** Convert a NetWare error code into a string.
*/
char * ErrToString(NWCCODE wErrCode)
{
     switch(wErrCode)
     {
          default:
               /* Convert error code to string. */
               sprintf(szErrorString, "Unknown Error: 0x%04X", wErrCode);
               return szErrorString;
     }
}
