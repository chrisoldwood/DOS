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
          case ERR_Q_IO_FAILURE:
               return "Queue I/O failure";

          case ERR_NO_QUEUE:
               return "Invalid Queue";

          case ERR_NO_Q_SERVER:
               return "No queue server";

          case ERR_NO_Q_RIGHTS:
               return "Insufficient rights";
               
          case ERR_Q_FULL:
               return "Queue full";

          case ERR_NO_Q_JOB:
               return "No queue job";

          case ERR_NO_Q_JOB_RIGHTS:
               return "Insufficient rights";

          case ERR_Q_IN_SERVICE:
               return "Queue in service";

          case ERR_Q_NOT_ACTIVE:
               return "Queue not active";
               
          case ERR_Q_HALTED:
               return "Queue halted";
               
          default:
               /* Convert error code to string. */
               sprintf(szErrorString, "Unknown Error: 0x%04X", wErrCode);
               return szErrorString;
     }
}
