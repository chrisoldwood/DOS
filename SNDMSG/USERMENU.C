/******************************************************************************
** (C) Chris Wood 1995.
**
** USERMENU.C - Show and handle the user select menu.
**
******************************************************************************* 
*/

#include <malloc.h>
#include <nwcalls.h>
#include <nwgfx.h>
#include "helpids.h"
#include "fileserv.h"
#include "users.h"
#include "msg.h"

/**** Global Vars. ***********************************************************/
static int iNumUsers;                        /* Total users. */

extern int iServer;

/**** Function Prototypes. ***************************************************/
NWGNUM UsersMenuProc(NWGMSG, NWGNUM, NWGMENUITEMS);

/******************************************************************************
** Setup and display the users selection menu.
*/
void UsersMenu(void)
{
     NWGMENU UserSelect;                     /* The menu. */
     NWGMENUITEMS MenuList;                  /* Menu items. */
     int iLoop;                              /* User counter. */

     /* Show wait message. */
     NWGFXWaitMessage(TRUE);

     /* Get the users on a server. */
     iNumUsers = GetUsers(GetFSConnHandle(iServer));
     
     /* Check we found users. */
     if (!iNumUsers)
     {
          /* Hide wait message. */
          NWGFXWaitMessage(FALSE);

          /* Show error message. */
          NWGFXInfoMessage("No Users Found");
          return;
     }
     
     /* Allocate memory for menu items. */
     MenuList = (NWGMENUITEMS) malloc(sizeof(NWGMENUITEM) * iNumUsers);
     if (!MenuList)
     {
          /* Hide wait message. */
          NWGFXWaitMessage(FALSE);

          /* Show error message. */
          NWGFXErrorMessage("Malloc failure!");
          return;
     }

     /* Set menu items pointer to queue info string. */
     for (iLoop=0; iLoop < iNumUsers; iLoop++)
          MenuList[iLoop].pItem = GetUserName(iLoop);

     /* Fill in menu details. */
     UserSelect.iPosType   = MP_RELATIVE;  
     UserSelect.iSX        = -32;
     UserSelect.iSY        = 4;   
     UserSelect.iDX        = 30;
     UserSelect.iDY        = 20;   
     UserSelect.fFlags     = MF_EXTENDED;
     UserSelect.pTitle     = "Select User(s)";    
     UserSelect.tTimeOut   = (NWGTIME) NULL;  
     UserSelect.fnMenuProc = (NWGMENUPROC) UsersMenuProc;
     UserSelect.iNumItems  = iNumUsers; 
     UserSelect.pItems     = MenuList;
     UserSelect.iCurrent   = 0;    
     UserSelect.iHelpID    = IDH_USERS;    

     /* Hide wait message. */
     NWGFXWaitMessage(FALSE);

     /* Show Select Queues popup. */
     NWGFXPopupMenu(&UserSelect);
}

/******************************************************************************
** Callback for users selection menu. 
*/
NWGNUM UsersMenuProc(NWGMSG wMsg, NWGNUM wSelected, NWGMENUITEMS pItems)
{
     char      Msg[60];                 /* The message. */
     NWGFRAME  MsgFrame;                /* The message sending frame. */
     NWGPOS    iTextX, iTextY;          /* Text position. */
     int       iLoop;                   /* User counter. */
     
     /* Decode menu message. */
     switch(wMsg)
     {
          /* Item selected. */
          case MENU_SELECT:
               /* Get the message. */
               if (!GetMessage(Msg))
                    return FALSE;
               
               /* Setup frame. */
               MsgFrame.iPosType = FP_ABSOLUTE;
               MsgFrame.fFlags = FF_BASIC | FF_TITLE;
               MsgFrame.pTitle = "Sending Message To User";

               /* Calculate frame position. */
               NWGFXCalcCentredFrame(42, 5, &MsgFrame.iSX, &MsgFrame.iSY,
                                            &MsgFrame.iDX, &MsgFrame.iDY);
               
               /* Create window frame. */
               NWGFXCreateWindow(&MsgFrame);

               /* Calculate text position. */
               iTextX = MsgFrame.iSX + ((MsgFrame.iDX - MsgFrame.iSX) / 2);
               iTextY = MsgFrame.iDY - 1;
               
               /* Check for multiple selection. */
               if (wSelected)
               {
                    /* Show status. */
                    NWGFXDrawString(iTextX, iTextY, TEXT_CENTRE, pItems[wSelected-1].pItem, WHITE);
                    
                    /* Send the message. */
                    SendMessage(GetFSConnHandle(iServer), pItems[wSelected-1].pItem, Msg);
               }
               else
               {
                    /* Loop for all users tagged. */
                    for (iLoop=0; iLoop < iNumUsers; iLoop++)
                    {
                         if (pItems[iLoop].bTagged)
                         {
                              /* Blank user name line. */
                              NWGFXDrawHorizLine(MsgFrame.iSX + 1, MsgFrame.iDX - 1, MsgFrame.iDY - 1, ' ', WHITE);

                              /* Show status. */
                              NWGFXDrawString(iTextX, iTextY, TEXT_CENTRE, pItems[iLoop].pItem, WHITE);

                              /* Send the message. */
                              SendMessage(GetFSConnHandle(iServer), pItems[iLoop].pItem, Msg);
                         }
                    }
               }

               /* Destroy window. */
               NWGFXDestroyWindow();
               break;

          /* Safe. */
          default:
               break;
     }

     /* Don't update. */
     return FALSE;
}
