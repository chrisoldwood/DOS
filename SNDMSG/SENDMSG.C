/******************************************************************************
** (C) Chris Wood 1995.
**
** SENDMSG.C - Main entry/init/exit point for application.
**
******************************************************************************* 
*/

#include <stdio.h>
#include <stdlib.h>
#include <nwcalls.h>
#include <nwgfx.h>
#include "helptext.h"
#include "fileserv.h"
#include "nwerrs.h"

/**** Global Vars. ***********************************************************/
NWGDIM    wDispWidth;                        /* The display width in chars. */
NWGDIM    wDispHeight;                       /* The display height in chars. */

/**** Function Prototypes. ***************************************************/
void Initialise(void);
void Terminate(void);

extern void FileServerMenu(void);

/******************************************************************************
** Application entry point.
** Call the initialisation procedure, attempt to read a config file and kick
** off the first menu. Finally call termination routine.
** NB: We never exit from main(); always from the terminate() function. This is
** because terminate() is our callback to exit at any time.
*/
void main()
{
     /* Screen, NetWare & App. initialisation. */
     Initialise();

     /* Show the startup menu. */
     FileServerMenu();

     /* Terminate application:- never returns. */
     Terminate();
}

/******************************************************************************
** Application initialisation.
** First try to initialise the netware library, then the screen. If okay,
** draw the init screen and build a list of connections. Finally draw the main
** application bg and setup the exit proc and help text.
*/
void Initialise(void)
{
     NWCCODE   wScnInitCode;       /* Screen init return code. */
     NWCCODE   wNWInitCode;        /* Netware init return code. */
     
     /* Initialise NetWare, must be first netware call. */
     wNWInitCode = NWCallsInit(NULL, NULL);
     if (wNWInitCode)
     {
          printf("Error (%x): Can't initialise NetWare library.\n", wNWInitCode);
          exit(EXIT_FAILURE);
     }

     /* Initialise Screen, must be first graphics call. */
     wScnInitCode = NWGFXInitScreen();
     if (wScnInitCode == NWGERR_VIDEO)
     {
          printf("Error: Unsupported video mode:-\n80 column text modes only.\n");
          exit(EXIT_FAILURE);
     }
     else if (wScnInitCode == NWGERR_ALLOC)
     {
          printf("Error: Memory allocation failure.\n");
          exit(EXIT_FAILURE);
     }

     /* Check screen dimensions. */
     wDispWidth  = NWGFXGetScreenMetric(SCREEN_WIDTH);
     wDispHeight = NWGFXGetScreenMetric(SCREEN_HEIGHT);
     if (wDispWidth < 80)
     {
          NWGFXCleanUp();
          printf("Error: 80 column text mode required.\n");
          exit(EXIT_FAILURE);
     }

     /* Draw application initialisation background. */
     NWGFXDrawInitString(0, 0, TEXT_LEFT, APPTITLE);
     NWGFXDrawInitString(0, 1, TEXT_LEFT, COPYRIGHT);
     
     /* Get a list of file server connections.*/
     wNWInitCode = GetFSConnections();
     if (wNWInitCode)
     {
          NWGFXInfoMessage(ErrToString(wNWInitCode));
          NWGFXCleanUp();
          exit(EXIT_FAILURE);
     }

     /* Check we have at least 1 connection. */
     if (!GetNumFSConnections())
     {
          NWGFXInfoMessage("You are not connected to any servers");
          NWGFXCleanUp();
          exit(EXIT_FAILURE);
     }

     /* Initialise the main application. */
     NWGFXInitApplication(3, APPNAME, Terminate, hHelpText);
     NWGFXDrawTitleString(2, 1, TEXT_LEFT, APPTITLE);
     NWGFXDrawTitleString(wDispWidth-3, 1, TEXT_RIGHT, COPYRIGHT);
}

/******************************************************************************
** Application exit.
** Close the display and exit.
*/
void Terminate(void)
{
     /* Required to restore the display. */
     NWGFXCleanUp();
     exit(EXIT_SUCCESS);
}
