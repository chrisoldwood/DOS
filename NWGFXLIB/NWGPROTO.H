/******************************************************************************
** (C) Chris Wood 1994/95.
**
** NWGPROTO.H - All function prototypes.
**
******************************************************************************* 
*/

/* First check for previous inclusion. */
#ifndef _NWGPROTO_H
#define _NWGPROTO_H

/* Check for C++ compiler. */
#ifdef __cplusplus
extern "C" {
#endif

/* From NWGINIT.C */
extern NWCCODE NWGFXInitScreen(VOID);
extern VOID NWGFXCleanUp(VOID);
extern VOID NWGFXDrawInitString(NWGPOS, NWGPOS, NWGTYPE, NWGSTR);
extern VOID NWGFXInitApplication(NWGDIM, NWGSTR, NWGABORTPROC, NWGHELP*);
extern VOID NWGFXDrawTitleString(NWGPOS, NWGPOS, NWGTYPE, NWGSTR);

/* From NWGCORE.C */
extern VOID NWGFXGetCursorPos(NWGPOS *, NWGPOS *);
extern VOID NWGFXSetCursorPos(NWGPOS, NWGPOS);
extern VOID NWGFXHideCursor(VOID);
extern VOID NWGFXPlotChar(NWGPOS, NWGPOS, NWGCHAR, NWGCOLOUR);
extern VOID NWGFXDrawHorizLine(NWGPOS, NWGPOS, NWGPOS, NWGCHAR, NWGCOLOUR);
extern VOID NWGFXDrawVertLine(NWGPOS, NWGPOS, NWGPOS, NWGCHAR, NWGCOLOUR);
extern VOID NWGFXDrawString(NWGPOS, NWGPOS, NWGTYPE, NWGSTR, NWGCOLOUR);
extern NWGFARBUF NWGFXSaveRect(NWGPOS, NWGPOS, NWGPOS, NWGPOS);
extern VOID NWGFXRestoreRect(NWGPOS, NWGPOS, NWGPOS, NWGPOS, NWGFARBUF);
extern VOID NWGFXFillRect(NWGPOS, NWGPOS, NWGPOS, NWGPOS, NWGCHAR, NWGCOLOUR, NWGCOLOUR);
extern VOID NWGFXClearRect(NWGPOS, NWGPOS, NWGPOS, NWGPOS);
extern VOID NWGFXScrollRectUp(NWGPOS, NWGPOS, NWGPOS, NWGPOS);
extern VOID NWGFXScrollRectDown(NWGPOS, NWGPOS, NWGPOS, NWGPOS);
extern VOID NWGFXHiliteField(NWGPOS, NWGPOS, NWGPOS, NWGBOOL, NWGBOOL);

/* From NWGFRAME.C */
extern VOID NWGFXDrawFrame(NWGPOS, NWGPOS, NWGPOS, NWGPOS, NWGBOOL, NWGTYPE, NWGCOLOUR, NWGCOLOUR);
extern VOID NWGFXCalcCentredFrame(NWGDIM, NWGDIM, NWGPOS *, NWGPOS *, NWGPOS *, NWGPOS *);
extern VOID NWGFXCreateWindow(NWGFRAMEPTR);
extern VOID NWGFXDestroyWindow(VOID);
extern VOID NWGFXDrawWindow(NWGTYPE);
extern NWGNUM NWGFXGetScreenMetric(NWGNUM);

/* From NWGKEYBD.C */
extern NWGBOOL NWGFXIsKeyPressed(VOID);
extern NWGKEY NWGFXGetKeyPressed(VOID);
extern NWGKEY NWGFXWaitForKeyPress(VOID);
extern NWGKEY NWGFXTimedWaitForKeyPress(NWGTIME);

/* From NWGHELP.C */
extern VOID NWGFXSetHelpSection(NWGNUM);
extern NWGNUM NWGFXGetHelpSection(VOID);
extern VOID NWGFXCallUpHelp(VOID);
extern VOID NWGFXShowHelpSection(VOID);
extern VOID NWGFXListKeys(VOID);
extern VOID NWGFXAllowHelp(NWGBOOL);

/* From NWGMBOX.C */
extern VOID NWGFXMessageBox(NWGSTR, NWGCOLOUR, NWGCOLOUR);
extern VOID NWGFXHelpMessage(NWGSTR);
extern VOID NWGFXInfoMessage(NWGSTR);
extern VOID NWGFXErrorMessage(NWGSTR);
extern VOID NWGFXWaitMessage(NWGBOOL);

/* From NWGMENU.C */
extern NWGNUM NWGFXPopupMenu(NWGMENU *); 
extern NWGBOOL NWGFXQueryMenu(NWGSTR, NWGBOOL);
extern VOID NWGFXAllowExit(NWGBOOL);
extern NWGBOOL NWGFXExitMenu(VOID);

/* From NWGDLG.C */
extern VOID NWGFXDialog(NWGDIALOG *); 

/* From NWGDEBUG.C */
extern VOID NWGFXShowStackSpace(VOID);
extern VOID NWGFXShowHeapSpace(VOID);
extern VOID NWGFXCheckHeap(VOID);

/* Check for C++ compiler. */
#ifdef __cplusplus
}
#endif

/* _NWGPROTO_H */
#endif
