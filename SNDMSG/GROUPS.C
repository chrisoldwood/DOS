/******************************************************************************
** (C) Chris Wood 1995.
**
** GROUPS.C - Functions to access groups.
**
******************************************************************************* 
*/

#include <malloc.h>
#include <string.h>
#include <nwcalls.h>
#include "nwerrs.h"
#include "groups.h"

/**** Global Vars. ***********************************************************/
static GROUP_LIST pGroups;             /* The list of groups. */
static int        iNumGroups;          /* Number of groups. */

/******************************************************************************
** Get the number groups on a given server.
*/
int GetGroupCount(NWCONN_HANDLE hServer)
{
     NWCCODE   wFailure = 0;            /* List end flag. */
     NWOBJ_ID  GroupID = (NWOBJ_ID) -1; /* Group ID returned; -1 is for find first. */

     /* Reset count. */
     iNumGroups = 0;
          
     /* Search until list exhausted. */
     while(!wFailure)
     {
          /* Search for next. */
          wFailure = NWScanObject(hServer, (char NWFAR *) "*", OT_USER_GROUP, 
                         (NWOBJ_ID NWFAR *) &GroupID, NULL, NULL, NULL, NULL,
                         NULL);

          /* Check for last group. */
          if (!wFailure)
               iNumGroups++;
     }

     return iNumGroups;
}

/******************************************************************************
** Get the group info for each user on a server.
*/
int GetGroups(NWCONN_HANDLE hServer)
{
     NWCCODE   wFailure = 0;            /* List end flag. */
     NWOBJ_ID  GroupID = (NWOBJ_ID) -1; /* Group ID returned; -1 is for find first. */
     int       iGroup;                   /* Group counter. */
     
     /* Get total number of users. */
     GetGroupCount(hServer);
     
     /* Check number. */
     if (!iNumGroups)
          return 0;

     /* Free old list. */
     if (pGroups)
          free(pGroups);
          
     /* Allocate memory for the list. */
     pGroups = (GROUP_LIST) malloc(sizeof(GROUP) * iNumGroups);
     if (!pGroups)
          return 0;
          
     /* Reset count. */
     iGroup = 0;
          
     /* Search until list exhausted. */
     while(!wFailure)
     {
          /* Search for next. */
          wFailure = NWScanObject(hServer, (char NWFAR *) "*", OT_USER_GROUP, 
                         (NWOBJ_ID NWFAR *) &GroupID, (char NWFAR *) pGroups[iGroup].Name, 
                         NULL, NULL, NULL, NULL);

          /* Check for last server. */
          if (!wFailure)
          {
               /* Save object ID. */
               pGroups[iGroup].GroupID = GroupID;
               
               /* Update count. */
               iGroup++;
          }
     }

     return iNumGroups;
}

/******************************************************************************
** Get the groups name by index.
*/
char * GetGroupName(int iIndex)
{
     return pGroups[iIndex].Name;
}
