/******************************************************************************
** (C) Chris Wood 1995.
**
** PRNTSERV.C - Functions to access print servers.
**
******************************************************************************* 
*/

#include <string.h>
#include <nwcalls.h>
#include <nwpsrv.h>
#include "printjob.h"
#include "prtqueue.h"
#include "prntserv.h"
#include "fileserv.h"

/**** Global Vars. ***********************************************************/
static PRINTSERVER Servers[MAX_PRINT_SERVERS];    
                                        /* Print servers we are connected to. */
static NWNUMBER wNumServers;            /* Number of print servers we found. */

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

/******************************************************************************
** Get the index of the print server by Object ID.
*/
int GetPSIndexByObjID(NWOBJ_ID ObjectID)
{
     int  iLoop;
     
     /* Search the whole list. */
     for (iLoop=0; iLoop < (int) wNumServers; iLoop++)
     {
          /* Found it? */
          if (Servers[iLoop].ObjectID == ObjectID)
               return iLoop;
     }

     /* On a server we can't access? */
     return -1;
}

/******************************************************************************
** Create the menu string that consists of the server name and it's file server
** hosts' name.
*/
char * GetPSNameAndHost(int iServer)
{
     char FSName[MAX_FSNAME_LEN+1];          /* Temporary file server name. */
     
     /* Copy in print server name, and truncate. */
     strncpy(Servers[iServer].MenuName, Servers[iServer].Name, 25);
     Servers[iServer].MenuName[24] = '\0';
     
     /* Tag on open bracket. */
     strcat(Servers[iServer].MenuName, " (");
     
     /* Get the file server name, truncate and tag on. */
     strcpy(FSName, GetFSNameByConnID(Servers[iServer].ConnID));
     FSName[20] = '\0';
     strcat(Servers[iServer].MenuName, FSName);
     
     /* Tag on close bracket. */
     strcat(Servers[iServer].MenuName, ")");

     return Servers[iServer].MenuName;
}

/******************************************************************************
** Get the Object ID of the server by index.
*/
NWOBJ_ID GetPSObjID(int iServer)
{
     return Servers[iServer].ObjectID;
}

/******************************************************************************
** Connect to the print server. This returns if it went okay.
*/
NWCCODE PSConnect(int iServer)
{
     NWCCODE   wRetVal;       /* NW return code. */
     BYTE      uAccess;       /* Access level. */
     
     /* First attach to the server. */
     wRetVal = NWPSComAttachToPrintServer(NWPS_BINDERY_SERVICE_PRE_40, 
                    Servers[iServer].ConnID, 15, Servers[iServer].Name,
                    (WORD NWFAR *) &Servers[iServer].SPXConnID);
          
     /* Check return code. */
     if (wRetVal)
          return wRetVal;
          
     /* Now login. */
     wRetVal = NWPSComLoginToPrintServer(NWPS_BINDERY_SERVICE_PRE_40,
                    Servers[iServer].ConnID, Servers[iServer].SPXConnID,
                    (BYTE NWFAR *) &uAccess);
 
     return wRetVal;
 }
 
/******************************************************************************
** Disconnect from the print server.
*/
void PSDisconnect(int iServer)
{
     NWPSComDetachFromPrintServer(Servers[iServer].SPXConnID);
}
 
#define MAX_Q_PRINTERS   10

/******************************************************************************
** Disconnect from the print server.
*/
int GetQueuesPrinter(int iServer, PPRINTQUEUE pQueue)
{
     WORD MaxPrinters;                  /* Number of printers servicing queue. */
     WORD Printers[MAX_Q_PRINTERS];     /* List of printers. */
     
     /* Get the list of printers. */
     NWPSComGetPrintersServicingQ(Servers[iServer].SPXConnID, 
          (char NWFAR*) GetFSNameByConnID(Servers[iServer].ConnID),
          (char NWFAR*) pQueue->Name, MAX_Q_PRINTERS, (WORD NWFAR*) &MaxPrinters,
          (WORD NWFAR*) Printers);
    
     /* Only return the first(!). */
     if (MaxPrinters == 0)
          return -1;
     else
          return (int) Printers[0];
}
