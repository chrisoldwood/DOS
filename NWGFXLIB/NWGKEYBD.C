/******************************************************************************
** (C) Chris Wood 1994/95.
**
** NWGKEYBD.C - Keyboard handling code.
**
******************************************************************************* 
*/

#include <conio.h>
#include <time.h>
#include "nwgfx.h"

/**** Portability. ***********************************************************/
#ifdef _MSC_VER
/* For Microsoft C. */
#define KBHIT  _kbhit
#define GETCH  _getch
#else
/* For everyone else. */
#define KBHIT  kbhit
#define GETCH  getch
#endif

/**** Global Variables ********************************************************/
NWGBOOL      bAllowExit=TRUE;      /* Allow the exit menu to be shown. */
NWGBOOL      bAllowHelp=TRUE;      /* Allow help to be shown. */
NWGBOOL      bExitShown=FALSE;     /* The exit menu is being shown. */
NWGBOOL      bStackShown=FALSE;    /* The stack space is being shown. */
NWGBOOL      bHeapShown=FALSE;     /* The heap space is being shown. */
NWGBOOL      bCheckShown=FALSE;    /* The stack check is being shown. */
NWGABORTPROC fnExit;               /* The exit callback function. */

/******************************************************************************
** Check to see if a key has been pressed. The keycode is not removed from the
** buffer.
*/
NWGBOOL NWGFXIsKeyPressed(VOID)
{
     return KBHIT();
}

/******************************************************************************
** Get a keycode from the keyboard buffer. If it is F1 launch the help handler,
** if help is available. If it is ALT+F10 call up the exit menu, if an exit
** callback has been given.
** If the key is not a regular key, then 0xFF is placed in the hi-byte.
*/
NWGKEY NWGFXGetKeyPressed(VOID)
{
     NWGKEY    wKey=0;        /* Last key pressed. */
    
     /* Get a key, and check for an extended code. */
     wKey = (NWGKEY) GETCH();
     if ( (wKey == 0) || (wKey == 0xE0) )
     {
          /* Get extended key code. */
          wKey = ((NWGKEY) 0xFF00) | ((NWGKEY) GETCH());
     }

     /* Check for exit request. */
     if ( (wKey == KEY_ALT_F10) && (fnExit) && (!bExitShown) && (bAllowExit) )
     {
          /* Set flag to stop repeats. */
          bExitShown = TRUE;
          
          /* Show exit menu and call terminate funcn if true. */
          if (NWGFXExitMenu())
               fnExit();

          /* Unset flag. */
          bExitShown = FALSE;
     }

     /* Check for help request. */
     if ( (wKey == KEY_F1) && (bAllowHelp) )
     {
          /* Launch the help handler. */
          NWGFXCallUpHelp();
     }

#ifdef DEBUG
     /* Check for max stack display. */
     if ( (wKey == KEY_F11) && (!bStackShown) )
     {
          /* Set flag to stop repeats. */
          bStackShown = TRUE;

          /* Show available stack space. */
          NWGFXShowStackSpace();

          /* Unset flag. */
          bStackShown = FALSE;
     }
     
     /* Check for max heap display. */
     if ( (wKey == KEY_F12) && (!bHeapShown) )
     {
          /* Set flag to stop repeats. */
          bHeapShown = TRUE;

          /* Show available heap space. */
          NWGFXShowHeapSpace();

          /* Unset flag. */
          bHeapShown = FALSE;
     }

     /* Check for heap check. */
     if ( (wKey == KEY_CTL_F12) && (!bCheckShown) )
     {
          /* Set flag to stop repeats. */
          bCheckShown = TRUE;

          /* Check heap. */
          NWGFXCheckHeap();

          /* Unset flag. */
          bCheckShown = FALSE;
     }
#endif
     
     return wKey;
}

/******************************************************************************
** Wait until a key is pressed and return the keycode.
*/
NWGKEY NWGFXWaitForKeyPress(VOID)
{
     /* Poll keyboard. */
     while(!KBHIT());

     /* Get KeyCode. */
     return NWGFXGetKeyPressed();
}

/******************************************************************************
** Wait until a key is pressed or a timeout occurs and return either the
** keycode or null. The length before a timeout is given in seconds.
*/
NWGKEY NWGFXTimedWaitForKeyPress(NWGTIME tPause)
{
     time_t    tCurrent, tTimeOut;      /* The current and timeout times. */
     
     /* Calculate timeout time. */
     time(&tCurrent);
     tTimeOut = tCurrent + (time_t) tPause;
     
     /* Check until timeout. */
     while(tCurrent <= tTimeOut)
     {
          /* Check for keypress and process. */
          if (KBHIT())
               return NWGFXGetKeyPressed();

          /* Get current time.*/
          time(&tCurrent);
     }
     
     /* Timeout occurred. */
     return KEY_TIMEOUT;
}
