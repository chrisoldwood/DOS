/******************************************************************************
** (C) Chris Wood 1995.
**
** USERS.C - Functions to access users.
**
******************************************************************************* 
*/

#include <malloc.h>
#include <string.h>
#include <nwcalls.h>
#include "nwerrs.h"
#include "users.h"

/**** Global Vars. ***********************************************************/
static USER_LIST pUsers;             /* The list of users. */
static int       iNumUsers;          /* Number of users. */

/******************************************************************************
** Get the number users on a given server.
*/
int GetUserCount(NWCONN_HANDLE hServer)
{
     NWCCODE   wFailure = 0;            /* List end flag. */
     NWOBJ_ID  UserID = (NWOBJ_ID) -1;  /* User ID returned; -1 is for find first. */

     /* Reset count. */
     iNumUsers = 0;
          
     /* Search until list exhausted. */
     while(!wFailure)
     {
          /* Search for next. */
          wFailure = NWScanObject(hServer, (char NWFAR *) "*", OT_USER, 
                         (NWOBJ_ID NWFAR *) &UserID, NULL, NULL, NULL, NULL,
                         NULL);

          /* Check for last user. */
          if (!wFailure)
               iNumUsers++;
     }

     return iNumUsers;
}

/******************************************************************************
** Get the user info for each user on a server.
*/
int GetUsers(NWCONN_HANDLE hServer)
{
     NWCCODE   wFailure = 0;            /* List end flag. */
     NWOBJ_ID  UserID = (NWOBJ_ID) -1;  /* User ID returned; -1 is for find first. */
     int       iUser;                   /* User counter. */
     
     /* Get total number of users. */
     GetUserCount(hServer);
     
     /* Free old list. */
     if (pUsers)
          free(pUsers);
          
     /* Check number. */
     if (!iNumUsers)
          return 0;
          
     /* Allocate memory for the list. */
     pUsers = (USER_LIST) malloc(sizeof(USER) * iNumUsers);
     if (!pUsers)
          return 0;
          
     /* Reset count. */
     iUser = 0;
          
     /* Search until list exhausted. */
     while(!wFailure)
     {
          /* Search for next. */
          wFailure = NWScanObject(hServer, (char NWFAR *) "*", OT_USER, 
                         (NWOBJ_ID NWFAR *) &UserID, (char NWFAR *) pUsers[iUser].Name, 
                         NULL, NULL, NULL, NULL);

          /* Check for last server. */
          if (!wFailure)
          {
               /* Save object ID. */
               pUsers[iUser].UserID = UserID;
               
               /* Update count. */
               iUser++;
          }
     }

     return iNumUsers;
}

/******************************************************************************
** Get the users name by index.
*/
char * GetUserName(int iIndex)
{
     return pUsers[iIndex].Name;
}
