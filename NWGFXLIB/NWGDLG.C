/*******************************************************************************
** (C) Chris Wood 1994/95.
**
** NWGDLG.C - Dialog box handler.
**
********************************************************************************
*/

#include <string.h>
#include "nwgfx.h"

#ifdef DEBUG
#include <stdio.h>
#include <stdlib.h>
#endif

/**** Global Variables ********************************************************/
extern NWGDIM    iScnWidth;        /* The screen width in chars. */
extern NWGDIM    iScnHeight;       /* The screen height in chars. */
extern NWGTYPE   iScnType;         /* The screen type, colour or mono. */

/**** Internal Functions ******************************************************/
static VOID NWGFXDrawDialog(NWGDIALOG*, NWGBOOL, NWGNUM);

/**** External Functions ******************************************************/
extern VOID NWGFXCalcAbsoluteFrame(NWGTYPE, NWGPOS, NWGPOS, NWGPOS, NWGPOS, 
                                   NWGPOS *, NWGPOS *, NWGPOS *, NWGPOS *);

/*******************************************************************************
** Handle a dialog box. This draws all the controls onto the form, handles the
** switching between controls and the calling of the control subroutines to 
** handle the input.
*/
VOID NWGFXDialog(NWGDIALOG * dDialog) 
{
     NWGFARBUF lpDlgBuf;           /* Dialog save buffer. */
     NWGPOS    iSX, iSY, iDX, iDY; /* Dialog position. */
     NWGPOS    iY;                 /* Tmp position. */
     NWGPOS    iBX, iEX;           /* Beginning and end of a control field. */
     NWGKEY    wKey;               /* Last key pressed. */
     NWGBOOL   bRestart;           /* Restart dialog. */
     NWGNUM    iActCtrl;           /* Currently active control. */
     NWGNUM    iTmpCtrl;           /* Temporary control counter. */
     NWGBOOL   bFound;             /* Found something? */
     NWGMENU   *pMenu;             /* Popup menu control. */
     NWGNUM    wChoice;            /* Popup menu return value. */
     NWGDIALOG *pDialog;           /* Dialog control. */
     NWGNUM    iOldHelpID;         /* The old help section. */
     NWGNUM    iOldCtrl;           /* Old active control.*/
     
#ifdef DEBUG
     if ( (!dDialog) || (!dDialog->pControls) )
     {
          NWGFXSetCursorPos(0, 0);
          printf("NWGFXDialog(): NULL dialog/controls pointer.");
          return;
     }
#endif

     /* Calculate dialog position in absolute co-ordinates. */
     NWGFXCalcAbsoluteFrame(dDialog->iPosType, dDialog->iSX, dDialog->iSY, dDialog->iDX, 
                              dDialog->iDY, &iSX, &iSY, &iDX, &iDY);

     /* First save the screen area under the dialog. */
     lpDlgBuf = NWGFXSaveRect(iSX, iSY, iDX, iDY);

     /* Set the menus' help section. */
     iOldHelpID = NWGFXGetHelpSection();
     NWGFXSetHelpSection(dDialog->iHelpID);

     /* Get active control, if allowed. */
     iActCtrl = dDialog->iCurrent;
     iOldCtrl = iActCtrl;
     
     /* If the dialog is to be restarted, it's from here. */
restart:

     /* Check for change in active control. */
     if (dDialog->iCurrent != iOldCtrl)
     {
          /* Set new active control. */
          iActCtrl = dDialog->iCurrent;
          iOldCtrl = iActCtrl;
     }
     
     /* Check border style. */
     if (dDialog->fFlags & DF_STATIC)
          iActCtrl = 0;

     /* Check for active control. */
     if (iActCtrl)
     {
          /* Calculate real control field position. */
          iBX = iSX + dDialog->pControls[iActCtrl-1].iXOffset + 1;
          iEX = iBX + dDialog->pControls[iActCtrl-1].iWidth - 1;
          iY  = iSY + dDialog->pControls[iActCtrl-1].iYOffset + 3;
     }

     /* Draw the whole dialog. */
     NWGFXDrawDialog(dDialog, TRUE, iActCtrl);
          
     /* Main loop to poll the keyboard and handle the dialog. */
     while(TRUE)
     {
          /* Wait for keypress or timeout if needed. */
          if (dDialog->fFlags & DF_TIMER)
               wKey = NWGFXTimedWaitForKeyPress(dDialog->tTimeOut);
          else
               wKey = NWGFXWaitForKeyPress();
          
          /* Decode keycode. */
          switch(wKey)
          {
               /* Move to previous field. */
               case KEY_LEFT:
               case KEY_UP:
                    /* Check we can activate a control. */
                    if (iActCtrl)
                    {
                         /* Remember current control. */
                         if (iActCtrl == 1)
                              iTmpCtrl = dDialog->iNumCtls;
                         else
                              iTmpCtrl = iActCtrl - 1;
                         
                         /* Reset search flag. */
                         bFound = FALSE;
                         
                         /* Find next control. */
                         while (TRUE) 
                         {
                              /* Check control type. */
                              switch(dDialog->pControls[iTmpCtrl-1].iType)
                              {
                                   /* Can activate these. */
                                   case DCTL_ETEXT:
                                   case DCTL_DECVALUE:
                                   case DCTL_HEXVALUE:
                                   case DCTL_TOGGLE:
                                   case DCTL_MENU:
                                   case DCTL_DIALOG:
                                   case DCTL_CUSTOM:
                                        bFound = TRUE;
                                        break;
                    
                                   /* Ignore others. */
                                   default:
                                        break;
                              }
               
                              /* Found one? */
                              if (bFound)
                                   break;
                                   
                              /* Move on to next. */
                              iTmpCtrl--;
                              
                              /* Check for wraparound. */
                              if (iTmpCtrl <= 0)
                                   iTmpCtrl = dDialog->iNumCtls;
                         }

                         /* Check for change. */
                         if (iTmpCtrl != iActCtrl)
                         {
                              /* Remove highlight on current control. */
                              NWGFXHiliteField(iBX, iEX, iY, FALSE, FALSE);

                              /* Set new active control. */
                              iActCtrl = iTmpCtrl;
                              
                              /* Calculate real new control field position. */
                              iBX = iSX + dDialog->pControls[iActCtrl-1].iXOffset + 1;
                              iEX = iBX + dDialog->pControls[iActCtrl-1].iWidth - 1;
                              iY  = iSY + dDialog->pControls[iActCtrl-1].iYOffset + 3;

                              /* Set highlight on new control. */
                              NWGFXHiliteField(iBX, iEX, iY, FALSE, TRUE);
                         }
                    }
                    break;

               /* Move to next field. */
               case KEY_RIGHT:
               case KEY_DOWN:
                    /* Check we can activate a control. */
                    if (iActCtrl)
                    {
                         /* Remember current control. */
                         if (iActCtrl == dDialog->iNumCtls)
                              iTmpCtrl = 1;
                         else
                              iTmpCtrl = iActCtrl + 1;
                         
                         /* Reset search flag. */
                         bFound = FALSE;
                         
                         /* Find next control. */
                         while (TRUE) 
                         {
                              /* Check control type. */
                              switch(dDialog->pControls[iTmpCtrl-1].iType)
                              {
                                   /* Can activate these. */
                                   case DCTL_ETEXT:
                                   case DCTL_DECVALUE:
                                   case DCTL_HEXVALUE:
                                   case DCTL_TOGGLE:
                                   case DCTL_MENU:
                                   case DCTL_DIALOG:
                                   case DCTL_CUSTOM:
                                        bFound = TRUE;
                                        break;
                    
                                   /* Ignore others. */
                                   default:
                                        break;
                              }
               
                              /* Found one? */
                              if (bFound)
                                   break;
                                   
                              /* Move on to next. */
                              iTmpCtrl++;
                              
                              /* Check for wraparound. */
                              if (iTmpCtrl > dDialog->iNumCtls)
                                   iTmpCtrl = 1;
                         }

                         /* Check for change. */
                         if (iTmpCtrl != iActCtrl)
                         {
                              /* Remove highlight on current control. */
                              NWGFXHiliteField(iBX, iEX, iY, FALSE, FALSE);

                              /* Set new active control. */
                              iActCtrl = iTmpCtrl;
                              
                              /* Calculate real new control field position. */
                              iBX = iSX + dDialog->pControls[iActCtrl-1].iXOffset + 1;
                              iEX = iBX + dDialog->pControls[iActCtrl-1].iWidth - 1;
                              iY  = iSY + dDialog->pControls[iActCtrl-1].iYOffset + 3;

                              /* Set highlight on new control. */
                              NWGFXHiliteField(iBX, iEX, iY, FALSE, TRUE);
                         }
                    }
                    break;

               /* Select current field. */
               case KEY_RETURN:
                    /* Check we have an active control. */
                    if (iActCtrl)
                    {
                         /* Check control type. */
                         switch(dDialog->pControls[iActCtrl-1].iType)
                         {
                              /* An editable text field. */
                              case DCTL_ETEXT:
                                   break;
                              
                              /* A decimal value. */
                              case DCTL_DECVALUE:
                                   break;
                              
                              /* A hexadecimal value. */
                              case DCTL_HEXVALUE:
                                   break;
                              
                              /* A multi-state toggle. */
                              case DCTL_TOGGLE:
                                   break;
                                   
                              /* A popup menu. */
                              case DCTL_MENU:
                                   /* Check for callback function. */
                                   if (dDialog->fnDlgProc)
                                   {
                                        /* Signal menu selected. */
                                        dDialog->fnDlgProc(DIALOG_CTL_INIT, iActCtrl-1, (NWGVARPTR) NULL);
                                   
                                        /* Get menu. */
                                        pMenu = (NWGMENU *) dDialog->pControls[iActCtrl-1].pCustom;
                                        
                                        /* Draw the dialog inactive. */
                                        NWGFXDrawDialog(dDialog, FALSE, iActCtrl);

                                        /* Display the menu. */
                                        wChoice = NWGFXPopupMenu(pMenu);
                                        
                                        /* Check result. */
                                        if ( (wChoice) || (pMenu->fFlags & MF_EXTENDED) )
                                        {
                                             /* Notify with the selection. */
                                             bRestart = dDialog->fnDlgProc(DIALOG_CTL_DONE, iActCtrl-1, (NWGVARPTR)(long) wChoice);

                                             /* Check for restart. */
                                             if (bRestart)
                                                  goto restart;
                                        }

                                        /* Draw the dialog active again. */
                                        NWGFXDrawDialog(dDialog, TRUE, iActCtrl);
                                   }
                                   break;
                              
                              /* A dialog. */
                              case DCTL_DIALOG:
                                   /* Check for callback function. */
                                   if (dDialog->fnDlgProc)
                                   {
                                        /* Signal dialog selected. */
                                        dDialog->fnDlgProc(DIALOG_CTL_INIT, iActCtrl-1, (NWGVARPTR) NULL);
                                   
                                        /* Get dialog. */
                                        pDialog = (NWGDIALOG *) dDialog->pControls[iActCtrl-1].pCustom;
                                        
                                        /* Draw this dialog inactive. */
                                        NWGFXDrawDialog(dDialog, FALSE, iActCtrl);

                                        /* Display the dialog. */
                                        NWGFXDialog(pDialog);
                                        
                                        /* Notify the completion. */
                                        bRestart = dDialog->fnDlgProc(DIALOG_CTL_DONE, iActCtrl-1, (NWGVARPTR) NULL);

                                        /* Check for restart. */
                                        if (bRestart)
                                             goto restart;

                                        /* Draw this dialog active again. */
                                        NWGFXDrawDialog(dDialog, TRUE, iActCtrl);
                                   }
                                   break;
                    
                              /* A custom control. */
                              case DCTL_CUSTOM:
                                   /* Check for callback function. */
                                   if (dDialog->fnDlgProc)
                                   {
                                        /* Draw the dialog inactive. */
                                        NWGFXDrawDialog(dDialog, FALSE, iActCtrl);

                                        /* Ask callback to handle control. */
                                        bRestart = dDialog->fnDlgProc(DIALOG_CTL_EXEC, iActCtrl-1, dDialog->pControls[iActCtrl-1].pCustom);

                                        /* Check for restart. */
                                        if (bRestart)
                                             goto restart;

                                        /* Draw the dialog active again. */
                                        NWGFXDrawDialog(dDialog, TRUE, iActCtrl);
                                   }
                                   break;
                                   
                              /* Ignore others. */
                              default:
                                   break;
                         }
                    }
                    break;

               /* Timeout occurred. */
               case KEY_TIMEOUT:
                    /* Check for timer message. */
                    if ( (dDialog->fFlags & DF_TIMER) && (dDialog->fnDlgProc) )
                    {
                         /* Send message. */
                         bRestart = dDialog->fnDlgProc(DIALOG_TIMEOUT, 0, NULL);
                         
                         /* Check for restart. */
                         if (bRestart)
                              goto restart;
                    }
                    break;
                                        
               /* Exit dialog. */
               case KEY_ESCAPE:
                    /* Check for cancel message. */
                    if ( (dDialog->fFlags & DF_CANCEL) && (dDialog->fnDlgProc) )
                    {
                         /* Confirm cancel. */
                         if (dDialog->fnDlgProc(DIALOG_CANCEL, 0, NULL))
                         {
                              /* Restore the old help. */
                              NWGFXSetHelpSection(iOldHelpID);
                              
                              /* Replace the screen area under the dialog. */
                              NWGFXRestoreRect(iSX, iSY, iDX, iDY, lpDlgBuf);
                              return;
                         }
                         
                         /* Do not close. */
                         break;
                    }
                    
                    /* Restore the old help. */
                    NWGFXSetHelpSection(iOldHelpID);

                    /* Replace the screen area under the dialog. */
                    NWGFXRestoreRect(iSX, iSY, iDX, iDY, lpDlgBuf);
                    return;

               /* Safe. */                    
               default:
                    break;
          }
     }
}

/*******************************************************************************
** Draw the whole dialog as active or inactive.
*/
static VOID NWGFXDrawDialog(NWGDIALOG *dDialog, NWGBOOL bActive, NWGNUM iActCtrl)
{
     NWGPOS    iSX, iSY, iDX, iDY; /* Dialog position. */
     NWGCOLOUR cDlgBGColour;       /* The dialog background colour. */
     NWGCOLOUR cDlgFGColour;       /* Dialog border colour. */
     NWGCOLOUR cCtlColour;         /* Control colour. */
     NWGNUM    iLoop;              /* Counter. */
     NWGPOS    iX, iY;             /* Tmp position. */
     NWGPOS    iBX, iEX;           /* Beginning and end of a control field. */
     NWGTYPE   iTextJust;          /* Text justification. */

     /* Calculate dialog position in absolute co-ordinates. */
     NWGFXCalcAbsoluteFrame(dDialog->iPosType, dDialog->iSX, dDialog->iSY, dDialog->iDX, 
                              dDialog->iDY, &iSX, &iSY, &iDX, &iDY);

     /* Get the dialog display colours. */
     if (iScnType == MONO_SCREEN)
     {
          cDlgBGColour = DARK;

          if (bActive)
               cDlgFGColour = BRIGHT;
          else
               cDlgFGColour = NORMAL;
     }
     else /* iScnType == COLOUR_SCREEN */
     {
          cDlgBGColour = BLUE;

          if (bActive)
               cDlgFGColour = YELLOW;
          else
               cDlgFGColour = WHITE;
     }

     /* Draw the dialog frame and title. */
     if (dDialog->fFlags & DF_STATIC)
          NWGFXDrawFrame(iSX, iSY, iDX, iDY, TRUE, FRAME_SINGLE, cDlgFGColour, cDlgBGColour);
     else 
          NWGFXDrawFrame(iSX, iSY, iDX, iDY, TRUE, FRAME_DOUBLE, cDlgFGColour, cDlgBGColour);
     NWGFXDrawString((iDX-iSX)/2+iSX+1, iSY+1, TEXT_CENTRE, dDialog->pTitle, cDlgFGColour);

     /* Draw all the controls. */
     for(iLoop=0; iLoop < dDialog->iNumCtls; iLoop++)
     {
          switch(dDialog->pControls[iLoop].iType)
          {
               /* A static text label. */
               case DCTL_LABEL:
                    /* Get text colour. */
                    if (iScnType == MONO_SCREEN)
                         cCtlColour = NORMAL;
                    else
                         cCtlColour = YELLOW;
     
                    /* Calculate real text position. */
                    iY = iSY + dDialog->pControls[iLoop].iYOffset + 3;
                    if (dDialog->pControls[iLoop].fFlags & DCF_TEXTLEFT)
                    {
                         iTextJust = TEXT_LEFT;
                         iX = iSX + dDialog->pControls[iLoop].iXOffset + 1;
                    }
                    else if (dDialog->pControls[iLoop].fFlags & DCF_TEXTCENTRE)
                    {
                         iTextJust = TEXT_CENTRE;
                         iX = iSX + dDialog->pControls[iLoop].iXOffset + 1 + 
                              (dDialog->pControls[iLoop].iWidth / 2 );
                    }
                    else /* dDialog->pControls[wLoop].bFlags & DCF_TEXTRIGHT */
                    {
                         iTextJust = TEXT_RIGHT;
                         iX = iSX + dDialog->pControls[iLoop].iXOffset + 1 + 
                              (dDialog->pControls[iLoop].iWidth - 1);
                    }

                    /* Write out text. */
                    NWGFXDrawString(iX, iY, iTextJust, dDialog->pControls[iLoop].pValue, cCtlColour);
                    break;

               /* A static text field. */
               case DCTL_STEXT:
               /* An editable text entry field. */
               case DCTL_ETEXT:
               /* A decimal number entry field. */
               case DCTL_DECVALUE:
               /* A hex number entry field. */
               case DCTL_HEXVALUE:
               /* A two-state toggle. */
               case DCTL_TOGGLE:
               /* A popup menu. */
               case DCTL_MENU:
               /* A dialog. */
               case DCTL_DIALOG:
               /* A custom control. */
               case DCTL_CUSTOM:
                    /* Get text colour. */
                    if (iScnType == MONO_SCREEN)
                         cCtlColour = BRIGHT;
                    else
                         cCtlColour = WHITE;

                    /* Calculate real text position. */
                    iX = iSX + dDialog->pControls[iLoop].iXOffset + 1;
                    iY = iSY + dDialog->pControls[iLoop].iYOffset + 3;

                    /* Write out text. */
                    NWGFXDrawString(iX, iY, TEXT_LEFT, dDialog->pControls[iLoop].pValue, cCtlColour);
                    break;

               /* Safe. */
               default:
                    break;
          }
     }

     /* Check for active control. */
     if (iActCtrl)
     {
          /* Calculate real control field position. */
          iBX = iSX + dDialog->pControls[iActCtrl-1].iXOffset + 1;
          iEX = iBX + dDialog->pControls[iActCtrl-1].iWidth - 1;
          iY  = iSY + dDialog->pControls[iActCtrl-1].iYOffset + 3;

          /* Set highlight on first control. */
          if (bActive)
               NWGFXHiliteField(iBX, iEX, iY, FALSE, TRUE);
     }
}
