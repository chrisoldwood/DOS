/******************************************************************************
** (C) Chris Wood 1995.
**
** FILESERV.C - Functions to access file servers.
**
******************************************************************************* 
*/

#include <nwcalls.h>
#include "fileserv.h"

/**** Global Vars. ***********************************************************/
static FILESERVER Servers[MAX_FILE_SERVERS];    
                                        /* File servers we are connected to. */
static NWNUMBER wNumServers;            /* Number of file servers we are connected to. */

/**** Function Prototypes. ***************************************************/

/******************************************************************************
** Get a list of the file servers we are connected to. This should be called
** before any file server function.
*/
NWCCODE GetFSConnections(void)
{
     NWCCODE        wRetVal;                      /* Return code. */
     NWNUMBER       wLoop;                        /* Counter. */
     NWCONN_HANDLE  hConnList[MAX_FILE_SERVERS];  /* Tmp list of conn handles. */

     /* Get list of connection handles. */
     wRetVal = NWGetConnectionList((NWLOCAL_MODE) NULL, (NWCONN_HANDLE NWFAR *) hConnList, 
                              MAX_FILE_SERVERS, (NWNUMBER NWFAR *) &wNumServers);

     /* Check for call failure.*/
     if (wRetVal)
          return wRetVal;

     /* Check for no connections.*/
     if (!wNumServers)
          return wRetVal;
          
     /* Fill in file server connection table. */
     for(wLoop=0; wLoop<wNumServers; wLoop++)
     {
          /* Copy in handle.*/
          Servers[wLoop].ConnID = hConnList[wLoop];
          
          /* Get server name. */
          wRetVal = NWGetFileServerName(hConnList[wLoop], (char NWFAR *) Servers[wLoop].Name);

          /* Got an error, bail out. */
          if (wRetVal)
               return wRetVal;
     }
     
     /* Okay. */
     return SUCCESSFUL;
}

/******************************************************************************
** Get the number of file servers we are connected to.
*/
int GetNumFSConnections(void)
{
     return (int) wNumServers;
}

/******************************************************************************
** Get the connection handle of a server
*/
NWCONN_HANDLE GetFSConnHandle(int iServer)
{
     return Servers[iServer].ConnID;
}

/******************************************************************************
** Get the name of a file server.
*/
char * GetFSName(int iServer)
{
     return Servers[iServer].Name;
}
