/******************************************************************************
** (C) Chris Wood 1995.
**
** GRPMENU.C - Show and handle the group select menu.
**
******************************************************************************* 
*/

#include <malloc.h>
#include <nwcalls.h>
#include <nwgfx.h>
#include "helpids.h"
#include "fileserv.h"
#include "groups.h"
#include "msg.h"

/**** Global Vars. ***********************************************************/
static BYTE PropValue[128];             /* RAW property data. */

extern int iServer;

/**** Function Prototypes. ***************************************************/
NWGNUM GroupMenuProc(NWGMSG, NWGNUM, NWGMENUITEMS);

/******************************************************************************
** Setup and display the groups selection menu.
*/
void GroupMenu(void)
{
     NWGMENU GroupSelect;                    /* The menu. */
     NWGMENUITEMS MenuList;                  /* Menu items. */
     int iNumGroups;                         /* Total users. */
     int iLoop;                              /* User counter. */

     /* Show wait message. */
     NWGFXWaitMessage(TRUE);

     /* Get the users on a server. */
     iNumGroups = GetGroups(GetFSConnHandle(iServer));
     
     /* Check we found users. */
     if (!iNumGroups)
     {
          /* Hide wait message. */
          NWGFXWaitMessage(FALSE);

          /* Show error message. */
          NWGFXInfoMessage("No Groups Found");
          return;
     }
     
     /* Allocate memory for menu items. */
     MenuList = (NWGMENUITEMS) malloc(sizeof(NWGMENUITEM) * iNumGroups);
     if (!MenuList)
     {
          /* Hide wait message. */
          NWGFXWaitMessage(FALSE);

          /* Show error message. */
          NWGFXErrorMessage("Malloc failure!");
          return;
     }

     /* Set menu items pointer to queue info string. */
     for (iLoop=0; iLoop < iNumGroups; iLoop++)
          MenuList[iLoop].pItem = GetGroupName(iLoop);

     /* Fill in menu details. */
     GroupSelect.iPosType   = MP_RELATIVE;  
     GroupSelect.iSX        = -32;
     GroupSelect.iSY        = 4;   
     GroupSelect.iDX        = 30;
     GroupSelect.iDY        = 20;   
     GroupSelect.fFlags     = MF_BASIC;
     GroupSelect.pTitle     = "Select Group";    
     GroupSelect.tTimeOut   = (NWGTIME) NULL;  
     GroupSelect.fnMenuProc = (NWGMENUPROC) GroupMenuProc;
     GroupSelect.iNumItems  = iNumGroups; 
     GroupSelect.pItems     = MenuList;
     GroupSelect.iCurrent   = 0;    
     GroupSelect.iHelpID    = IDH_GROUP;    

     /* Hide wait message. */
     NWGFXWaitMessage(FALSE);

     /* Show Select Queues popup. */
     NWGFXPopupMenu(&GroupSelect);
}

/******************************************************************************
** Callback for group selection menu. 
*/
NWGNUM GroupMenuProc(NWGMSG wMsg, NWGNUM wSelected, NWGMENUITEMS pItems)
{
     char      Msg[60];                 /* The message. */
     NWGFRAME  MsgFrame;                /* The message sending frame. */
     NWGPOS    iTextX, iTextY;          /* Text position. */
     NWCCODE   wRetVal=0x0000;          /* Find return code. */
     char      UserName[50];            /* Next user in group (name). */
     NWOBJ_ID  UserID;                  /* Next user in group (ID). */
     NWFLAGS   fPropFlags;              /* Status flag. */
     NWFLAGS   fPropSec;                /* Security rights. */
     NWFLAGS   bHasValue;               /* Has a value. */
     NWFLAGS   bMore;                   /* More properties. */
     int       iSetIndex;               /* Property set index. */
     int       iUsers;                  /* Users counter. */
     
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
               
               /* Calculate text position. */
               iTextX = MsgFrame.iSX + ((MsgFrame.iDX - MsgFrame.iSX) / 2);
               iTextY = MsgFrame.iDY - 1;

               /* Set for search all. */
               UserID = (NWOBJ_ID) -1;
               iSetIndex = 1;
               
               /* Get members property. */
               wRetVal = NWScanProperty(GetFSConnHandle(iServer), 
                              (char NWFAR*) pItems[wSelected-1].pItem, OT_USER_GROUP,
                              (char NWFAR*) "GROUP_MEMBERS", (DWORD NWFAR*) &UserID, 
                              (char NWFAR*) NULL, (NWFLAGS NWFAR*) &fPropFlags, 
                              (NWFLAGS NWFAR*) &fPropSec, (NWFLAGS NWFAR*) &bHasValue, 
                              (NWFLAGS NWFAR*) &bMore);

               /* Check for members/error. */
               if ( (!bHasValue) || (wRetVal) )
               {
                    NWGFXInfoMessage("There are no users in the group");
                    return FALSE;
               }

               /* Create window frame. */
               NWGFXCreateWindow(&MsgFrame);
               
               /* Loop while succesful. */
               while(!wRetVal)
               {
                    wRetVal = NWReadPropertyValue(GetFSConnHandle(iServer), 
                                   (char NWFAR*) pItems[wSelected-1].pItem, OT_USER_GROUP,
                                   (char NWFAR*) "GROUP_MEMBERS", (NWSEGMENT_NUM) iSetIndex, 
                                   PropValue, (NWFLAGS NWFAR*) &bMore, 
                                   (NWFLAGS NWFAR*) &fPropFlags);
                    
                    if (!wRetVal)
                    {
                         /* Loop through RAW data. */
                         for (iUsers=0; iUsers < 32; iUsers++)
                         {
                              /* Get next user ID (convert from RAW BYTES to NWOBJ_ID). */
                              UserID = *(NWOBJ_ID *)&PropValue[iUsers * 4];
                              
                              /* Validate ID. */
                              if (UserID)
                              {
                                   /* Convert to user name. */
                                   wRetVal = NWGetObjectName(GetFSConnHandle(iServer),
                                                  UserID, (char NWFAR*) UserName, NULL);

                                   /* Validate. */
                                   if (!wRetVal)
                                   {
                                        /* Blank user name line. */
                                        NWGFXDrawHorizLine(MsgFrame.iSX + 1, MsgFrame.iDX - 1, MsgFrame.iDY - 1, ' ', WHITE);

                                        /* Show status. */
                                        NWGFXDrawString(iTextX, iTextY, TEXT_CENTRE, UserName, WHITE);
                    
                                        /* Send the message. */
                                        SendMessage(GetFSConnHandle(iServer), UserName, Msg);
                                   }
                              }
                         }
                         
                         /* Next block of data. */
                         iSetIndex++;
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
