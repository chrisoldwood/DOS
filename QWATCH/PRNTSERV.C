/******************************************************************************
** (C) Chris Wood 1995.
**
** PRNTSERV.C - Functions to access print servers.
**
******************************************************************************* 
*/

#include <string.h>
#include <nwcalls.h>
#include "prntserv.h"
#include "fileserv.h"

/**** Global Vars. ***********************************************************/
static PRINTSERVER Servers[MAX_PRINT_SERVERS];    
                                        /* Print servers we are connected to. */
static NWNUMBER wNumServers;            /* Number of print servers we found. */

/**** Function Prototypes. ***************************************************/

/******************************************************************************
** Get a list of all the print servers on all the file servers. This is used
** as a cache.
*/
void GetPrintServers(void)
{
     int            iFileServers, iLoop;          /* Number of file servers. */
     NWCCODE        wFailure;                     /* List end flag. */
     NWOBJ_ID       ServerID;                     /* Server ID returned; -1 is for find first. */
     NWCONN_HANDLE  hServer;                      /* Connection handle to server. */

     /* Get number of file servers. */
     iFileServers = GetNumFSConnections();

     /* Reset server count. */
     wNumServers = 0;

     /* Poll all file servers. */
     for (iLoop=0; iLoop < iFileServers; iLoop++)
     {
          /* Get server handle. */
          hServer = GetFSConnHandle(iLoop);

          /* Reset failure flag. */
          wFailure = FALSE;
          
          /* Reset search flag. */
          ServerID = (NWOBJ_ID) -1;
          
          /* Search until list exhausted. */
          while(!wFailure)
          {
               /* Search for next. */
               wFailure = NWScanObject(hServer, (char NWFAR *) "*", OT_PRINT_SERVER, 
                              (NWOBJ_ID NWFAR *) &ServerID, NULL, NULL, NULL, NULL,
                              NULL);

               /* Check for last server. */
               if (!wFailure)
               {
                    /* Save the object ID. */
                    Servers[wNumServers].ObjectID = ServerID;
               
                    /* Save the connection handle. */
                    Servers[wNumServers].ConnID = hServer;
                    
                    /* Get print server name. */
                    NWGetObjectName(hServer, ServerID, (char NWFAR *) Servers[wNumServers].Name, NULL);

                    /* Update count. */
                    wNumServers++;
               }
          }
     }
}

/******************************************************************************
** Get the number of print servers we are connected to.
*/
int GetNumPrintServers(void)
{
     return (int) wNumServers;
}

/******************************************************************************
** Get the connection handle of a print server by index
*/
NWCONN_HANDLE GetPSConnHandle(int iServer)
{
     return Servers[iServer].ConnID;
}

/******************************************************************************
** Get the SPX connection of a print server by index
*/
WORD GetPSSPXConn(int iServer)
{
     return Servers[iServer].SPXConnID;
}

/******************************************************************************
** Get the name of a print server by index.
*/
char * GetPSName(int iServer)
{
     return Servers[iServer].Name;
}

/******************************************************************************
** Get the name of the print server by object_id. This looks it up in our 
** structure rather than calling the server. Safe?
*/
char * GetPSNameByObjID(NWOBJ_ID ObjectID)
{
     int  iLoop;
     
     /* Search the whole list. */
     for (iLoop=0; iLoop < (int) wNumServers; iLoop++)
     {
          /* Found it? */
          if (Servers[iLoop].ObjectID == ObjectID)
               return Servers[iLoop].Name;
     }

     /* On a server we can't access. */
     return "Unknown";
}
