/*******************************************************************************
** (C) Chris Wood 1994/95.
**
** NWGMENU.C - Popup menus handler.
**
********************************************************************************
*/

#include <string.h>
#include <malloc.h>
#include "nwgfx.h"

#ifdef DEBUG
#include <stdio.h>
#include <stdlib.h>
#endif

/**** Portability. ***********************************************************/
#ifdef _MSC_VER
/* For Microsoft C. */
#define STRNICMP    _strnicmp
#else
/* For everyone else. */
#define STRNICMP    strnicmp
#endif

/**** Global Variables ********************************************************/
NWGCHAR        szAppName[20];      /* The application name. */

extern NWGDIM     iScnWidth;       /* The screen width in chars. */
extern NWGDIM     iScnHeight;      /* The screen height in chars. */
extern NWGTYPE    iScnType;        /* The screen type, colour or mono. */
extern NWGDIM     iTitleHt;        /* The title bar height. */    
extern NWGBOOL    bAllowExit;      /* Allow the exit menu to be shown. */

/**** External Functions ******************************************************/
extern VOID NWGFXCalcAbsoluteFrame(NWGTYPE, NWGPOS, NWGPOS, NWGPOS, NWGPOS,
                                   NWGPOS *, NWGPOS *, NWGPOS *, NWGPOS *);
static VOID NWGFXDrawMenu(NWGMENU *, NWGBOOL, NWGNUM, NWGNUM, NWGNUM);


/*******************************************************************************
** Create and handle a popup menu. The menu position and size is given by 
** (sx,sy) and (dx,dy). The menu structure provides all the other information. 
** The callback function determines how the menu behaves:- 
** If no callback is supplied the menu returns 0 if ESC is pressed, or the 
** number of the item selected (1-n) with the menu items tagged in the menu 
** structure passed in if an extended menu.
** If a callback is supplied then the menu calls this function when any event
** occurs, selection/insertion/deletion/cancelation. 0 is always returned when
** the menu is cancelled.
*/
NWGNUM NWGFXPopupMenu(NWGMENU * mMenu) 
{
     NWGFARBUF lpMenuBuf;               /* Menu save buffer. */
     NWGPOS    iSX, iSY, iDX, iDY;      /* Menu position.*/
     NWGNUM    iVisible;                /* Number of visible options at any time. */
     NWGNUM    iLoop;                   /* Counter. */
     NWGNUM    iFirst, iLast;           /* First & Last items that fits in the window. */
     NWGNUM    iCurrent, iOldCurr;      /* The currently selected item, and old one. */
     NWGBOOL   bRestart;                /* Return code from menu proc. */
     NWGCOLOUR cNormOptCol;             /* The colour to display the options normally. */
     NWGCOLOUR cSelOptCol;              /* The colour to display the options when selected. */
     NWGCOLOUR cMenuBGCol;              /* The colour of the menu background. */
     NWGBOOL   bExit;                   /* User wants to exit. */
     NWGKEY    wKey;                    /* Last key pressed. */
     NWGCHAR   szSelBuffer[80];         /* Typed selection buffer. */
     NWGPOS    iSelPos;                 /* Typed selection character position. */
     NWGNUM    iMatch, iOldMatch;       /* Current & Previous search match. */
     NWGNUM    iPage;                   /* 'Page' where new match lies. */
     NWGBOOL   bTagged;                 /* Any items tagged? */
     NWGNUM    iOldHelpID;              /* The old help section. */
     NWGNUM    iOldItems;               /* The old number of items. */
          
#ifdef DEBUG
     if ( (!mMenu) || ( (mMenu->iNumItems) && (!mMenu->pItems) ) )
     {
          NWGFXSetCursorPos(0, 0);
          printf("NWGFXPopupMenu(): NULL menu/items pointer.");
          return 0;
     }
#endif

     /* Calculate menu position in absolute co-ordinates. */
     NWGFXCalcAbsoluteFrame(mMenu->iPosType, mMenu->iSX, mMenu->iSY, mMenu->iDX, 
                              mMenu->iDY, &iSX, &iSY, &iDX, &iDY);

     /* First save the screen area under the menu. */
     lpMenuBuf = NWGFXSaveRect(iSX, iSY, iDX, iDY);

     /* Get the options-text display colours. */
     if (iScnType == MONO_SCREEN)
     {
          cMenuBGCol  = DARK;
          cNormOptCol = NORMAL;
          cSelOptCol  = BRIGHT;
     }
     else /* iScnType == COLOUR_SCREEN */
     {
          cMenuBGCol  = BLUE;
          cNormOptCol = WHITE;
          cSelOptCol  = YELLOW;
     }

     /* Set the menus' help section. */
     iOldHelpID = NWGFXGetHelpSection();
     NWGFXSetHelpSection(mMenu->iHelpID);

     /* Get current item. */
     iCurrent = mMenu->iCurrent;
     iOldCurr = iCurrent;

     /* Setup for first time through. */
     iOldItems = mMenu->iNumItems + 1;
     
     /* Calculate number of visible options. */
     iVisible = iDY - iSY - 3;

     /* If the menu items have been changed by the callback, restart here. */
restart:

     /* Set the tag flag to false for all entries. */
     for(iLoop=0; iLoop < mMenu->iNumItems; iLoop++)
          mMenu->pItems[iLoop].bTagged = FALSE;

     /* Check for any changes. */
     if ( (mMenu->iCurrent != iOldCurr)
       || (mMenu->iNumItems != iOldItems) )
     {
          /* Check for any change in selected item. */
          if (mMenu->iCurrent != iOldCurr)
          {
               /* Update current item. */
               iCurrent = mMenu->iCurrent;
               iOldCurr = iCurrent;
          }
     
          /* Check for change to number of items. */
          if (mMenu->iNumItems != iOldItems)
          {
               /* Update. */
               iOldItems = mMenu->iNumItems;
          }
          
          /* Calculate first and last visible items. */
          if (mMenu->iNumItems > iVisible)
          {
               /* Check selected item is in view. */
               if ( (iCurrent < mMenu->iNumItems)
                 && (iCurrent > (iVisible-1)) )
               {
                    /* Attempt to make it the top item. */
                    iFirst = iCurrent;
                    iLast  = iFirst + iVisible - 1;
               
                    /* Re-check last value. */
                    if (iLast >= mMenu->iNumItems)
                    {
                         iLast  = mMenu->iNumItems - 1;
                         iFirst = iLast - iVisible + 1;
                    }
               }
               else
               {
                    /* Validate current item. */
                    if (iCurrent >= mMenu->iNumItems)
                         iCurrent = mMenu->iNumItems-1;
                    
                    iFirst = 0;
                    iLast  = iVisible-1;
               }
          }
          else /* All are visible. */
          {
               /* Check that current is valid. */
               if (iCurrent >= mMenu->iNumItems)
                    iCurrent = mMenu->iNumItems - 1;
                    
               /* Mark all as visible. */
               iFirst = 0;
               iLast  = mMenu->iNumItems-1;
          }
     }
     
     /* Draw the menu active. */
     NWGFXDrawMenu(mMenu, TRUE, iFirst, iLast, iCurrent);

     /* Clear the selection buffer and variables. */
     szSelBuffer[0] = '\0';
     iSelPos = 0;
     iOldMatch = mMenu->iNumItems;
     
     /* Main loop to poll the keyboard and handle the menu. */
     while(TRUE)
     {
          /* Wait for keypress or timeout if needed. */
          if (mMenu->fFlags & MF_TIMER)
               wKey = NWGFXTimedWaitForKeyPress(mMenu->tTimeOut);
          else
               wKey = NWGFXWaitForKeyPress();
          
          /* Decode keycode. */
          switch(wKey)
          {
               /* Move cursor up. */
               case KEY_UP:
                    /* Check we have entries. */
                    if (!mMenu->iNumItems)
                         break;
                         
                    /* Check if at top. */
                    if (iCurrent > 0)
                    {
                         /* Clear the typed buffer, if one */
                         if (iSelPos)
                         {
                              szSelBuffer[0] = '\0';
                              iSelPos = 0;
                              iOldMatch = mMenu->iNumItems;
                              NWGFXHideCursor();
                         }

                         /* Check if we need to scroll the list. */
                         if (iCurrent <= iFirst)
                         {
                              /* Remove highlight. */
                              NWGFXHiliteField(iSX+3, iDX-1, iSY+3+iCurrent-iFirst, mMenu->pItems[iCurrent].bTagged, FALSE);

                              /* Update visible options counters. */
                              iFirst--;
                              iLast--;
                              iCurrent--;

                              /* Scroll list down. */
                              NWGFXScrollRectDown(iSX+3, iSY+3, iDX-1, iDY-1);
                              if (mMenu->pItems[iFirst].bTagged == FALSE)
                                   NWGFXDrawString(iSX+3, iSY+3, TEXT_LEFT, mMenu->pItems[iFirst].pItem, cNormOptCol);
                              else
                                   NWGFXDrawString(iSX+3, iSY+3, TEXT_LEFT, mMenu->pItems[iFirst].pItem, cSelOptCol);

                              /* Highlight new option. */
                              NWGFXHiliteField(iSX+3, iDX-1, iSY+3+iCurrent-iFirst, mMenu->pItems[iCurrent].bTagged, TRUE);

                              /* Show scrollbar DOWN nudge. */
                              NWGFXPlotChar(iSX+1, iDY-1, DOWNNUDGE, cNormOptCol);

                              /* If at start of list remove scrollbar up nudge. */
                              if(iFirst == 0)
                                   NWGFXPlotChar(iSX+1, iSY+3, ' ', cNormOptCol);
                         }
                         else
                         {
                              /* Change hilighted option. */
                              NWGFXHiliteField(iSX+3, iDX-1, iSY+3+iCurrent-iFirst, mMenu->pItems[iCurrent].bTagged, FALSE);
                              iCurrent--;
                              NWGFXHiliteField(iSX+3, iDX-1, iSY+3+iCurrent-iFirst, mMenu->pItems[iCurrent].bTagged, TRUE);
                         }
                    }
                    break;
                    
               /* Move cursor down. */
               case KEY_DOWN:
                    /* Check we have entries. */
                    if (!mMenu->iNumItems)
                         break;
                         
                    /* Check if at bottom. */
                    if (iCurrent < mMenu->iNumItems-1)
                    {
                         /* Clear the typed buffer, if one */
                         if (iSelPos)
                         {
                              szSelBuffer[0] = '\0';
                              iSelPos = 0;
                              iOldMatch = mMenu->iNumItems;
                              NWGFXHideCursor();
                         }

                         /* Check if we need to scroll the list. */
                         if (iCurrent >= iLast)
                         {
                              /* Remove highlight. */
                              NWGFXHiliteField(iSX+3, iDX-1, iSY+3+iCurrent-iFirst, mMenu->pItems[iCurrent].bTagged, FALSE);

                              /* Update visible options counters. */
                              iFirst++;
                              iLast++;
                              iCurrent++;

                              /* Scroll list up. */
                              NWGFXScrollRectUp(iSX+3, iSY+3, iDX-1, iDY-1);
                              if (mMenu->pItems[iLast].bTagged == FALSE)
                                   NWGFXDrawString(iSX+3, iDY-1, TEXT_LEFT, mMenu->pItems[iLast].pItem, cNormOptCol);
                              else
                                   NWGFXDrawString(iSX+3, iDY-1, TEXT_LEFT, mMenu->pItems[iLast].pItem, cSelOptCol);

                              /* Highlight new option. */
                              NWGFXHiliteField(iSX+3, iDX-1, iSY+3+iCurrent-iFirst, mMenu->pItems[iCurrent].bTagged, TRUE);

                              /* Show scrollbar UP nudge. */
                              NWGFXPlotChar(iSX+1, iSY+3, UPNUDGE, cNormOptCol);
                              
                              /* If at end of list remove scrollbar down nudge. */
                              if(iLast == mMenu->iNumItems-1)
                                   NWGFXPlotChar(iSX+1, iDY-1, ' ', cNormOptCol);
                         }
                         else
                         {
                              /* Change hilighted option. */
                              NWGFXHiliteField(iSX+3, iDX-1, iSY+3+iCurrent-iFirst, mMenu->pItems[iCurrent].bTagged, FALSE);
                              iCurrent++;
                              NWGFXHiliteField(iSX+3, iDX-1, iSY+3+iCurrent-iFirst, mMenu->pItems[iCurrent].bTagged, TRUE);
                         }
                    }
                    break;

               /* Move up one page.*/
               case KEY_PAGEUP:
                    /* Check we have entries. */
                    if (!mMenu->iNumItems)
                         break;
                         
                    /* Clear the typed buffer, if one */
                    if (iSelPos)
                    {
                         szSelBuffer[0] = '\0';
                         iSelPos = 0;
                         iOldMatch = mMenu->iNumItems;
                         NWGFXHideCursor();
                    }

                    /* Remove highlight. */
                    NWGFXHiliteField(iSX+3, iDX-1, iSY+3+iCurrent-iFirst, mMenu->pItems[iCurrent].bTagged, FALSE);

                    /* Check for page flip. */
                    if (iFirst > 0)
                    {
                         /* Flip one page. */
                         iFirst -= iVisible;
                         if (iFirst < 0)
                              iFirst = 0;
                         iLast = iFirst + iVisible - 1;

                         /* Redisplay. */
                         NWGFXClearRect(iSX+3, iSY+3, iDX-1, iDY-1);
                         for(iLoop=iFirst; iLoop <= iLast; iLoop++)
                         {
                              if (mMenu->pItems[iLoop].bTagged == FALSE)
                                   NWGFXDrawString(iSX+3, iSY+3+iLoop-iFirst, TEXT_LEFT, mMenu->pItems[iLoop].pItem, cNormOptCol);
                              else
                                   NWGFXDrawString(iSX+3, iSY+3+iLoop-iFirst, TEXT_LEFT, mMenu->pItems[iLoop].pItem, cSelOptCol);
                         }

                         /* Update scrollbar nudges. */
                         NWGFXPlotChar(iSX+1, iDY-1, DOWNNUDGE, cNormOptCol);
                         if(iFirst == 0)
                              NWGFXPlotChar(iSX+1, iSY+3, ' ', cNormOptCol);
                    }

                    /* Set current to first option. */
                    iCurrent = iFirst;

                    /* Highlight new option. */
                    NWGFXHiliteField(iSX+3, iDX-1, iSY+3+iCurrent-iFirst, mMenu->pItems[iCurrent].bTagged, TRUE);
                    break;

               /* Move down one page.*/
               case KEY_PAGEDOWN:
                    /* Check we have entries. */
                    if (!mMenu->iNumItems)
                         break;
                         
                    /* Clear the typed buffer, if one */
                    if (iSelPos)
                    {
                         szSelBuffer[0] = '\0';
                         iSelPos = 0;
                         iOldMatch = mMenu->iNumItems;
                         NWGFXHideCursor();
                    }

                    /* Remove highlight. */
                    NWGFXHiliteField(iSX+3, iDX-1, iSY+3+iCurrent-iFirst, mMenu->pItems[iCurrent].bTagged, FALSE);

                    /* Check for page flip. */
                    if (iLast < mMenu->iNumItems-1)
                    {
                         /* Flip one page. */
                         iLast += iVisible;
                         if (iLast > mMenu->iNumItems-1)
                              iLast = mMenu->iNumItems-1;
                         iFirst = iLast - iVisible + 1;

                         /* Redisplay. */
                         NWGFXClearRect(iSX+3, iSY+3, iDX-1, iDY-1);
                         for(iLoop=iFirst; iLoop <= iLast; iLoop++)
                         {
                              if (mMenu->pItems[iLoop].bTagged == FALSE)
                                   NWGFXDrawString(iSX+3, iSY+3+iLoop-iFirst, TEXT_LEFT, mMenu->pItems[iLoop].pItem, cNormOptCol);
                              else
                                   NWGFXDrawString(iSX+3, iSY+3+iLoop-iFirst, TEXT_LEFT, mMenu->pItems[iLoop].pItem, cSelOptCol);
                         }

                         /* Update scrollbar nudges. */
                         NWGFXPlotChar(iSX+1, iSY+3, UPNUDGE, cNormOptCol);
                         if(iLast == mMenu->iNumItems-1)
                              NWGFXPlotChar(iSX+1, iDY-1, ' ', cNormOptCol);
                    }

                    /* Set current to last option. */
                    iCurrent = iLast;

                    /* Highlight new option. */
                    NWGFXHiliteField(iSX+3, iDX-1, iSY+3+iCurrent-iFirst, mMenu->pItems[iCurrent].bTagged, TRUE);
                    break;

               /* Move to start.*/
               case KEY_CTL_PAGEUP:
                    /* Check we have entries. */
                    if (!mMenu->iNumItems)
                         break;
                         
                    /* Clear the typed buffer, if one */
                    if (iSelPos)
                    {
                         szSelBuffer[0] = '\0';
                         iSelPos = 0;
                         iOldMatch = mMenu->iNumItems;
                         NWGFXHideCursor();
                    }

                    /* Remove highlight. */
                    NWGFXHiliteField(iSX+3, iDX-1, iSY+3+iCurrent-iFirst, mMenu->pItems[iCurrent].bTagged, FALSE);

                    /* Check for page flip. */
                    if (iFirst > 0)
                    {
                         /* Flip to start. */
                         iFirst = 0;
                         iLast = iFirst + iVisible - 1;

                         /* Redisplay. */
                         NWGFXClearRect(iSX+3, iSY+3, iDX-1, iDY-1);
                         for(iLoop=iFirst; iLoop <= iLast; iLoop++)
                         {
                              if (mMenu->pItems[iLoop].bTagged == FALSE)
                                   NWGFXDrawString(iSX+3, iSY+3+iLoop-iFirst, TEXT_LEFT, mMenu->pItems[iLoop].pItem, cNormOptCol);
                              else
                                   NWGFXDrawString(iSX+3, iSY+3+iLoop-iFirst, TEXT_LEFT, mMenu->pItems[iLoop].pItem, cSelOptCol);
                         }

                         /* Update scrollbar nudges. */
                         NWGFXPlotChar(iSX+1, iDY-1, DOWNNUDGE, cNormOptCol);
                         if(iFirst == 0)
                              NWGFXPlotChar(iSX+1, iSY+3, ' ', cNormOptCol);
                    }

                    /* Set current to first option. */
                    iCurrent = iFirst;

                    /* Highlight new option. */
                    NWGFXHiliteField(iSX+3, iDX-1, iSY+3+iCurrent-iFirst, mMenu->pItems[iCurrent].bTagged, TRUE);
                    break;

               /* Move to end.*/
               case KEY_CTL_PAGEDOWN:
                    /* Check we have entries. */
                    if (!mMenu->iNumItems)
                         break;
                         
                    /* Clear the typed buffer, if one */
                    if (iSelPos)
                    {
                         szSelBuffer[0] = '\0';
                         iSelPos = 0;
                         iOldMatch = mMenu->iNumItems;
                         NWGFXHideCursor();
                    }

                    /* Remove highlight. */
                    NWGFXHiliteField(iSX+3, iDX-1, iSY+3+iCurrent-iFirst, mMenu->pItems[iCurrent].bTagged, FALSE);

                    /* Check for page flip. */
                    if (iLast < mMenu->iNumItems-1)
                    {
                         /* Flip to end. */
                         iLast = mMenu->iNumItems-1;
                         iFirst = iLast - iVisible + 1;
                         
                         /* Redisplay. */
                         NWGFXClearRect(iSX+3, iSY+3, iDX-1, iDY-1);
                         for(iLoop=iFirst; iLoop <= iLast; iLoop++)
                         {
                              if (mMenu->pItems[iLoop].bTagged == FALSE)
                                   NWGFXDrawString(iSX+3, iSY+3+iLoop-iFirst, TEXT_LEFT, mMenu->pItems[iLoop].pItem, cNormOptCol);
                              else
                                   NWGFXDrawString(iSX+3, iSY+3+iLoop-iFirst, TEXT_LEFT, mMenu->pItems[iLoop].pItem, cSelOptCol);
                         }

                         /* Update scrollbar nudges. */
                         NWGFXPlotChar(iSX+1, iSY+3, UPNUDGE, cNormOptCol);
                         if(iLast == mMenu->iNumItems-1)
                              NWGFXPlotChar(iSX+1, iDY-1, ' ', cNormOptCol);
                    }

                    /* Set current to last option. */
                    iCurrent = iLast;

                    /* Highlight new option. */
                    NWGFXHiliteField(iSX+3, iDX-1, iSY+3+iCurrent-iFirst, mMenu->pItems[iCurrent].bTagged, TRUE);
                    break;

               /* Toggle selection.*/
               case KEY_F5:
                    /* Check we have entries. */
                    if (!mMenu->iNumItems)
                         break;
                         
                    /* Check its an extended manu. */
                    if(mMenu->fFlags & MF_EXTENDED)
                    {
                         /* Remove highlight. */
                         NWGFXHiliteField(iSX+3, iDX-1, iSY+3+iCurrent-iFirst, mMenu->pItems[iCurrent].bTagged, FALSE);

                         /* Change tag and redisplay option. */
                         if (mMenu->pItems[iCurrent].bTagged == FALSE)
                         {
                              mMenu->pItems[iCurrent].bTagged = TRUE;
                              NWGFXDrawString(iSX+3, iSY+3+iCurrent-iFirst, TEXT_LEFT, mMenu->pItems[iCurrent].pItem, cSelOptCol);
                         }
                         else
                         {
                              mMenu->pItems[iCurrent].bTagged = FALSE;
                              NWGFXDrawString(iSX+3, iSY+3+iCurrent-iFirst, TEXT_LEFT, mMenu->pItems[iCurrent].pItem, cNormOptCol);
                         }

                         /* Restore highlight. */
                         NWGFXHiliteField(iSX+3, iDX-1, iSY+3+iCurrent-iFirst, mMenu->pItems[iCurrent].bTagged, TRUE);
                    }
                    break;

               /* Cancel all tagged selections.*/
               case KEY_F7:
                    /* Check we have entries. */
                    if (!mMenu->iNumItems)
                         break;
                         
                    /* Check its an extended manu. */
                    if(mMenu->fFlags & MF_EXTENDED)
                    {
                         /* Set all tags to false. */
                         for(iLoop=0; iLoop < mMenu->iNumItems; iLoop++)
                              mMenu->pItems[iLoop].bTagged = FALSE;

                         /* Remove highlight. */
                         NWGFXHiliteField(iSX+3, iDX-1, iSY+3+iCurrent-iFirst, mMenu->pItems[iCurrent].bTagged, FALSE);

                         /* Redisplay. */
                         NWGFXClearRect(iSX+3, iSY+3, iDX-1, iDY-1);
                         for(iLoop=iFirst; iLoop <= iLast; iLoop++)
                         {
                              if (mMenu->pItems[iLoop].bTagged == FALSE)
                                   NWGFXDrawString(iSX+3, iSY+3+iLoop-iFirst, TEXT_LEFT, mMenu->pItems[iLoop].pItem, cNormOptCol);
                              else
                                   NWGFXDrawString(iSX+3, iSY+3+iLoop-iFirst, TEXT_LEFT, mMenu->pItems[iLoop].pItem, cSelOptCol);
                         }
                         
                         /* Restore highlight. */
                         NWGFXHiliteField(iSX+3, iDX-1, iSY+3+iCurrent-iFirst, mMenu->pItems[iCurrent].bTagged, TRUE);
                    }
                    break;

               /* Insert new items.*/
               case KEY_INSERT:
                    /* Check for insert flag and valid callback address. */
                    if ( (mMenu->fFlags & MF_INSERT) && (mMenu->fnMenuProc) )
                    {
                         /* Clear the typed buffer, if one */
                         if (iSelPos)
                         {
                              szSelBuffer[0] = '\0';
                              iSelPos = 0;
                              iOldMatch = mMenu->iNumItems;
                              NWGFXHideCursor();
                         }

                         /* Display menu as inactive. */
                         NWGFXDrawMenu(mMenu, FALSE, iFirst, iLast, iCurrent);

                         /* Call function to insert new items. */
                         bRestart = mMenu->fnMenuProc(MENU_INSERT, iCurrent+1, mMenu->pItems);

                         /* Check for change. */
                         if (bRestart)
                              goto restart;

                         /* Re-Display menu as active again. */
                         NWGFXDrawMenu(mMenu, TRUE, iFirst, iLast, iCurrent);
                    }
                    break;

               /* Delete items.*/
               case KEY_DELETE:
                    /* Check we have entries. */
                    if (!mMenu->iNumItems)
                         break;
                         
                    /* Check for delete flag and valid callback address. */
                    if ( (mMenu->fFlags & MF_DELETE) && (mMenu->fnMenuProc) )
                    {
                         /* Clear the typed buffer, if one */
                         if (iSelPos)
                         {
                              szSelBuffer[0] = '\0';
                              iSelPos = 0;
                              iOldMatch = mMenu->iNumItems;
                              NWGFXHideCursor();
                         }

                         /* Display menu as inactive. */
                         NWGFXDrawMenu(mMenu, FALSE, iFirst, iLast, iCurrent);

                         /* Check menu type. */
                         if (mMenu->fFlags & MF_EXTENDED)
                         {
                              /* Reset tagged flag. */
                              bTagged = FALSE;
                              
                              /* Check for tagged options. */
                              for(iLoop=iFirst; iLoop <= iLast; iLoop++)
                              {
                                   if (mMenu->pItems[iLoop].bTagged == TRUE)
                                        bTagged = TRUE;
                              }
                         }

                         /* Call function to delete items. */
                         if ( (mMenu->fFlags & MF_EXTENDED) && (bTagged) )
                              bRestart = mMenu->fnMenuProc(MENU_DELETE, 0, mMenu->pItems);
                         else
                              bRestart = mMenu->fnMenuProc(MENU_DELETE, iCurrent+1, mMenu->pItems);

                         /* Check for change. */
                         if (bRestart)
                              goto restart;

                         /* Re-Display menu as active again. */
                         NWGFXDrawMenu(mMenu, TRUE, iFirst, iLast, iCurrent);
                    }
                    break;

               /* Select option. */
               case KEY_RETURN:
                    /* Check we have entries. */
                    if (!mMenu->iNumItems)
                         break;

                    /* Clear the typed buffer, if one */
                    if (iSelPos)
                    {
                         szSelBuffer[0] = '\0';
                         iSelPos = 0;
                         iOldMatch = mMenu->iNumItems;
                         NWGFXHideCursor();
                    }

                    /* The callback determines next action. */
                    if (mMenu->fnMenuProc)
                    {
                         /* Display menu as inactive. */
                         NWGFXDrawMenu(mMenu, FALSE, iFirst, iLast, iCurrent);

                         /* Check menu type. */
                         if (mMenu->fFlags & MF_EXTENDED)
                         {
                              /* Reset tagged flag. */
                              bTagged = FALSE;
                              
                              /* Check for tagged options. */
                              for(iLoop=iFirst; iLoop <= iLast; iLoop++)
                              {
                                   if (mMenu->pItems[iLoop].bTagged == TRUE)
                                        bTagged = TRUE;
                              }
                         }

                         /* Call the callback with the option selected. */
                         if ( (mMenu->fFlags & MF_EXTENDED) && (bTagged) )
                              bRestart = mMenu->fnMenuProc(MENU_SELECT, 0, mMenu->pItems);
                         else
                              bRestart = mMenu->fnMenuProc(MENU_SELECT, iCurrent+1, mMenu->pItems);

                         /* Check for change. */
                         if (bRestart)
                              goto restart;

                         /* Display menu as active again. */
                         NWGFXDrawMenu(mMenu, TRUE, iFirst, iLast, iCurrent);
                    }
                    else
                    {
                         /* Restore the old help. */
                         NWGFXSetHelpSection(iOldHelpID);

                         /* Replace the screen area under the menu and exit. */
                         NWGFXRestoreRect(iSX, iSY, iDX, iDY, lpMenuBuf);
                         return iCurrent+1;
                    }
                    break;
                    
               /* Exit menu. */
               case KEY_ESCAPE:
                    /* Clear the typed buffer, if one */
                    if (iSelPos)
                    {
                         szSelBuffer[0] = '\0';
                         iSelPos = 0;
                         iOldMatch = mMenu->iNumItems;
                         NWGFXHideCursor();
                    }

                    /* Check for exit callback and call. */
                    if ( (mMenu->fFlags & MF_CANCEL) && (mMenu->fnMenuProc) )
                         bExit = mMenu->fnMenuProc(MENU_CANCEL, 0, (VOID *) NULL);
                    else
                         bExit = TRUE;
                    
                    /* Check result. */
                    if (bExit)
                    {
                         /* Restore the old help. */
                         NWGFXSetHelpSection(iOldHelpID);

                         /* Replace the screen area under the menu. */
                         NWGFXRestoreRect(iSX, iSY, iDX, iDY, lpMenuBuf);
                         return 0;
                    }
                    break;

               /* Timeout occurred. */
               case KEY_TIMEOUT:
                    /* Check for timer flag and valid callback address. */
                    if ( (mMenu->fFlags & MF_TIMER) && (mMenu->fnMenuProc) )
                    {
                         /* Clear the typed buffer, if one */
                         if (iSelPos)
                         {
                              szSelBuffer[0] = '\0';
                              iSelPos = 0;
                              iOldMatch = mMenu->iNumItems;
                              NWGFXHideCursor();
                         }

                         /* Callback with timeout message. */
                         bRestart = mMenu->fnMenuProc(MENU_TIMEOUT, iCurrent+1, mMenu->pItems);

                         /* Check for change. */
                         if (bRestart)
                              goto restart;

                         /* Redisplay. */
                         NWGFXClearRect(iSX+3, iSY+3, iDX-1, iDY-1);
                         for(iLoop=iFirst; iLoop <= iLast; iLoop++)
                         {
                              if (mMenu->pItems[iLoop].bTagged == FALSE)
                                   NWGFXDrawString(iSX+3, iSY+3+iLoop-iFirst, TEXT_LEFT, mMenu->pItems[iLoop].pItem, cNormOptCol);
                              else
                                   NWGFXDrawString(iSX+3, iSY+3+iLoop-iFirst, TEXT_LEFT, mMenu->pItems[iLoop].pItem, cSelOptCol);
                         }
                         
                         /* Highlight current. */
                         NWGFXHiliteField(iSX+3, iDX-1, iSY+3+iCurrent-iFirst, mMenu->pItems[iCurrent].bTagged, TRUE);
                    }
                    break;

               /* Clear typed selection. */
               case KEY_F3:
                    /* Check we have entries. */
                    if (!mMenu->iNumItems)
                         break;

                    /* Clear the buffer and hide the cursor. */
                    szSelBuffer[0] = '\0';
                    iSelPos = 0;
                    iOldMatch = mMenu->iNumItems;
                    NWGFXHideCursor();
                    break;
                    
               /* User may be typing their selection. */                    
               default:
                    /* Check we have entries. */
                    if (!mMenu->iNumItems)
                         break;

                    /* Check characters range. */
                    if ( (wKey < 256) && (wKey > 31) )
                    {
                         /* Add character to search buffer. */
                         szSelBuffer[iSelPos++] = (NWGCHAR) wKey;
                         szSelBuffer[iSelPos]   = '\0';

                         /* Search for a match. */
                         iMatch = mMenu->iNumItems;
                         for(iLoop=0; iLoop < mMenu->iNumItems; iLoop++)
                         {
                              /* Check the first iSelPos-1 characters. */
                              if (STRNICMP(mMenu->pItems[iLoop].pItem, szSelBuffer, iSelPos) == 0)
                              {
                                   iMatch = iLoop;
                                   break;
                              }
                        }
                         
                         /* Check to see if we found anything or the same one. */
                         if (iMatch == mMenu->iNumItems)
                         {
                              /* No match, remove character from buffer. */
                              szSelBuffer[--iSelPos] = '\0';
                         }
                         else /* 0 <= iMatch < mMenu->iNumItems. */
                         {
                              /* Check match is in view. */
                              if ( (iMatch < iFirst) || (iMatch > iLast) )
                              {
                                   /* Remove highlight. */
                                   NWGFXHiliteField(iSX+3, iDX-1, iSY+3+iCurrent-iFirst, mMenu->pItems[iCurrent].bTagged, FALSE);

                                   /* Find out which 'page' it's in. */
                                   iPage = iMatch / iVisible;

                                   /* Re-calculate first & last positions. */
                                   iFirst = iPage * iVisible;
                                   iLast  = iFirst + iVisible - 1;
                                   if (iLast > mMenu->iNumItems-1)
                                   {
                                        iLast = mMenu->iNumItems-1;
                                        iFirst = iLast - iVisible + 1;
                                   }

                                   /* Redisplay. */
                                   NWGFXClearRect(iSX+3, iSY+3, iDX-1, iDY-1);
                                   for(iLoop=iFirst; iLoop <= iLast; iLoop++)
                                   {
                                        if (mMenu->pItems[iLoop].bTagged == FALSE)
                                             NWGFXDrawString(iSX+3, iSY+3+iLoop-iFirst, TEXT_LEFT, mMenu->pItems[iLoop].pItem, cNormOptCol);
                                        else
                                             NWGFXDrawString(iSX+3, iSY+3+iLoop-iFirst, TEXT_LEFT, mMenu->pItems[iLoop].pItem, cSelOptCol);
                                   }

                                   /* Update top scrollbar nudge. */
                                   if (iFirst == 0)
                                        NWGFXPlotChar(iSX+1, iSY+3, ' ', cNormOptCol);
                                   else
                                        NWGFXPlotChar(iSX+1, iSY+3, UPNUDGE, cNormOptCol);
                                   
                                   /* Update bottom scrollbar nudge. */
                                   if(iLast == mMenu->iNumItems-1)
                                        NWGFXPlotChar(iSX+1, iDY-1, ' ', cNormOptCol);
                                   else
                                        NWGFXPlotChar(iSX+1, iDY-1, DOWNNUDGE, cNormOptCol);

                                   /* Update current and last match. */
                                   iOldMatch = iMatch;
                                   iCurrent  = iMatch;

                                   /* Highlight new option. */
                                   NWGFXHiliteField(iSX+3, iDX-1, iSY+3+iCurrent-iFirst, mMenu->pItems[iCurrent].bTagged, TRUE);
                              }
                              else
                              {
                                   /* In view; check if new match. */
                                   if (iOldMatch != iMatch)
                                   {
                                        /* Remove highlight. */
                                        NWGFXHiliteField(iSX+3, iDX-1, iSY+3+iCurrent-iFirst, mMenu->pItems[iCurrent].bTagged, FALSE);

                                        /* Update current and last match. */
                                        iOldMatch = iMatch;
                                        iCurrent  = iMatch;

                                        /* Highlight new option. */
                                        NWGFXHiliteField(iSX+3, iDX-1, iSY+3+iCurrent-iFirst, mMenu->pItems[iCurrent].bTagged, TRUE);
                                   }
                              }

                              /* Move cursor along one character. */
                              NWGFXSetCursorPos(iSX+3+iSelPos-1, iSY+3+iCurrent-iFirst);
                         }
                    }
                    else if (wKey == KEY_BACKSPACE)
                    {
                         /* Check for typed selection. */
                         if (iSelPos)
                         {
                              /* Remove last character from buffer. */
                              szSelBuffer[--iSelPos] = '\0';

                              /* Check for start position. */
                              if (iSelPos)
                              {
                                   /* Move cursor back one character. */
                                   NWGFXSetCursorPos(iSX+3+iSelPos-1, iSY+3+iOldMatch-iFirst);
                              }
                              else
                              {
                                   /* No selection, so hide cursor. */
                                   NWGFXHideCursor();
                              }
                         }
                    }
                    break;
          }
     }
}

/*******************************************************************************
** Draw the current menu as active or inactive.
*/
static VOID NWGFXDrawMenu(NWGMENU *pMenu, NWGBOOL bActive, NWGNUM iFirst, 
                              NWGNUM iLast, NWGNUM iCurrent)
{
     NWGPOS    iSX, iSY, iDX, iDY;      /* Menu position.*/
     NWGNUM    iLoop;                   /* Counter. */
     NWGCOLOUR cNormOptCol;             /* The colour to display the options normally. */
     NWGCOLOUR cSelOptCol;              /* The colour to display the options when selected. */
     NWGCOLOUR cMenuBGCol;              /* The colour of the menu background. */
     NWGBOOL   bTagged;                 /* Any items tagged? */

     /* Calculate menu position in absolute co-ordinates. */
     NWGFXCalcAbsoluteFrame(pMenu->iPosType, pMenu->iSX, pMenu->iSY, pMenu->iDX, 
                              pMenu->iDY, &iSX, &iSY, &iDX, &iDY);

     /* Get the options-text display colours. */
     if (iScnType == MONO_SCREEN)
     {
          cMenuBGCol  = DARK;
          cNormOptCol = NORMAL;
          cSelOptCol  = BRIGHT;
     }
     else /* iScnType == COLOUR_SCREEN */
     {
          cMenuBGCol  = BLUE;
          cNormOptCol = WHITE;
          cSelOptCol  = YELLOW;
     }

     /* Draw the menu frame, title and scroll bar edge. */
     if (bActive)
     {
          NWGFXDrawFrame(iSX, iSY, iDX, iDY, TRUE, FRAME_DOUBLE, cSelOptCol, cMenuBGCol);
          NWGFXDrawString((iDX-iSX)/2+iSX+1, iSY+1, TEXT_CENTRE, pMenu->pTitle, cSelOptCol);
     }
     else
     {
          NWGFXDrawFrame(iSX, iSY, iDX, iDY, TRUE, FRAME_DOUBLE, cNormOptCol, cMenuBGCol);
          NWGFXDrawString((iDX-iSX)/2+iSX+1, iSY+1, TEXT_CENTRE, pMenu->pTitle, cNormOptCol);
     }
     
     /* Draw scrollbar. */
     NWGFXDrawVertLine(iSX+2, iSY+3, iDY-1, S_VERTBAR, cNormOptCol);

     /* Check for nudges. */
     if (iFirst != 0)
          NWGFXPlotChar(iSX+1, iSY+3, UPNUDGE, cNormOptCol);
     if (iLast != pMenu->iNumItems-1)
          NWGFXPlotChar(iSX+1, iDY-1, DOWNNUDGE, cNormOptCol);
     
     /* Check menu type. */
     if (pMenu->fFlags & MF_EXTENDED)
     {
          /* Reset tagged flag. */
          bTagged = FALSE;
          
          /* Highlight all selected options. */
          for(iLoop=iFirst; iLoop <= iLast; iLoop++)
          {
               /* Keep a note of any tagged items. */
               if (pMenu->pItems[iLoop].bTagged == FALSE)
                    NWGFXDrawString(iSX+3, iSY+3+iLoop-iFirst, TEXT_LEFT, pMenu->pItems[iLoop].pItem, cNormOptCol);
               else
               {
                    NWGFXDrawString(iSX+3, iSY+3+iLoop-iFirst, TEXT_LEFT, pMenu->pItems[iLoop].pItem, cSelOptCol);
                    bTagged = TRUE;
               }
          }
          
          /* Check for any tagged items. */
          if ( (!bTagged) && (!bActive) )
          {
               /* Must draw current item as selected. */
               NWGFXDrawString(iSX+3, iSY+3+iCurrent-iFirst, TEXT_LEFT, pMenu->pItems[iCurrent].pItem, cSelOptCol);
          }
     }
     else
     {
          /* Highlight only the selected option. */
          for(iLoop=iFirst; iLoop <= iLast; iLoop++)
          {
               if (iLoop == iCurrent)
                    NWGFXDrawString(iSX+3, iSY+3+iLoop-iFirst, TEXT_LEFT, pMenu->pItems[iLoop].pItem, cSelOptCol);
               else
                    NWGFXDrawString(iSX+3, iSY+3+iLoop-iFirst, TEXT_LEFT, pMenu->pItems[iLoop].pItem, cNormOptCol);
          }
     }

     /* Show cursor highlight. */
     if ( (bActive) && (pMenu->iNumItems) )
          NWGFXHiliteField(iSX+3, iDX-1, iSY+3+iCurrent-iFirst, pMenu->pItems[iCurrent].bTagged, TRUE);
}

/* The query menu strings. */
static NWGSTR pNoStr  = "No";
static NWGSTR pYesStr = "Yes";

/*******************************************************************************
** Display a menu that requires a yes or no answer. The title for the menu is
** given and whether the Yes option is highlighted.
*/
NWGBOOL NWGFXQueryMenu(NWGSTR pTitle, NWGBOOL bYes)
{
     NWGNUM    iTitleLen;               /* Length of title. */
     NWGNUM    iResult;                 /* Return code from menu. */
     NWGMENUITEM Items[2];              /* Menu strings. */
     NWGMENU   mQuery;                  /* Query menu structure. */
     NWGPOS    iOldX, iOldY;            /* Old cursor position. */

#ifdef DEBUG
     if (!pTitle)
     {
          NWGFXSetCursorPos(0, 0);
          printf("NWGFXQueryMenu(): NULL title pointer.");
          return FALSE;
     }
#endif
     
     /* Get old cursor position and hide. */
     NWGFXGetCursorPos(&iOldX, &iOldY);
     NWGFXHideCursor();
     
     /* Get length of title. */
     iTitleLen = strlen(pTitle);
          
     /* Fill in menu strings. */
     Items[0].pItem = pNoStr;
     Items[1].pItem = pYesStr;
     
     /* Fill in menu details. */
     mQuery.iPosType   = MP_CENTRED;  
     mQuery.iSX        = iTitleLen + 4;
     mQuery.iSY        = 6;   
     mQuery.fFlags     = MF_BASIC;    
     mQuery.pTitle     = pTitle;
     mQuery.tTimeOut   = (NWGTIME) 0;
     mQuery.fnMenuProc = (NWGMENUPROC) NULL;
     mQuery.iNumItems  = 2;
     mQuery.pItems     = Items;

     /* Is "yes" to be the default option? */
     if (bYes)
          mQuery.iCurrent   = 1;
     else
          mQuery.iCurrent   = 0;

     /* Keep current help section. */
     mQuery.iHelpID    = NWGFXGetHelpSection();
     
     /* Display menu. */
     iResult = NWGFXPopupMenu(&mQuery);

     /* Restore cursor position. */
     NWGFXSetCursorPos(iOldX, iOldY);
     
     /* Check for reply of Yes. */
     if (iResult == 2)
          return TRUE;
     else
          return FALSE;
}

/*******************************************************************************
** Show the exit menu, and return the result.
*/
NWGBOOL NWGFXExitMenu(VOID)
{
     NWGSTR    pTitle;                  /* Menu title. */
     NWGNUM    iNameLen;                /* Length of application name. */
     NWGBOOL   bExit;                   /* Return code from menu. */
     
     /* Check for application name. */
     if (strlen(szAppName) > 0)
     {
          /* Allocate space for "Exit " + app title. */
          iNameLen = strlen(szAppName) + 6;
          pTitle = malloc(iNameLen);

          /* Create menu title. */
          strcpy(pTitle, "Exit ");
          strcat(pTitle, szAppName);
     }
     else
     {
          /* Allocate space for just "Exit". */
          iNameLen = 5;
          pTitle = malloc(iNameLen);

          /* Create menu title. */
          strcpy(pTitle, "Exit");
     }
          
     /* Call up query menu. */
     bExit = NWGFXQueryMenu(pTitle, TRUE);

     /* Free title memory. */
     free(pTitle);

     /* Return menu result. */
     return bExit;     
}

/*******************************************************************************
** Allow or disallow the exit menu to be shown. This is used to disable the 
** auto-exit function.
*/
VOID NWGFXAllowExit(NWGBOOL bCanExit)
{
     /* Set internal flag. */
     bAllowExit = bCanExit;
}
