/******************************************************************************
** (C) Chris Wood 1995.
**
** NWGDEBUG.C - Additional debugging functions.
**
******************************************************************************* 
*/

#include <malloc.h>
#include <stdio.h>
#include "nwgfx.h"

/**** Portability. ***********************************************************/
#ifdef _MSC_VER
/* For Microsoft C. */
#define STACKAVAIL  _stackavail
#define HEAPAVAIL   _memavl
#define HEAPCHECK   _heapchk
#else
/* For everyone else. */
#define STACKAVAIL  stackavail
#define HEAPAVAIL   memavl
#define HEAPCHECK   heapchk
#endif

/******************************************************************************
** Display the currently available stack space. This is shown by pressing F11
** at any time. This is only built into a DEBUG build.
*/
VOID NWGFXShowStackSpace(VOID)
{
     NWGCHAR   Space[40];          /* Free space message. */
     
     /* Create message. */
     sprintf(Space, "Free Stack Space: %u", STACKAVAIL());

     /* Display. */
     NWGFXInfoMessage(Space);
}

/******************************************************************************
** Display the currently available heap space. This is shown by pressing F12
** at any time. This is only built into a DEBUG build.
*/
VOID NWGFXShowHeapSpace(VOID)
{
     NWGCHAR   Space[40];          /* Free space message. */
     
     /* Create message. */
     sprintf(Space, "Free Heap Space: %u", HEAPAVAIL());

     /* Display. */
     NWGFXInfoMessage(Space);
}

/******************************************************************************
** Check the heap and display the status. This is shown by pressing CTL-F12 at
** at any time. This is only built into a DEBUG build.
*/
VOID NWGFXCheckHeap(VOID)
{
     NWGCHAR   Err[40];            /* Error code as string. */
     int       iStatus;            /* Heap status. */

     /* Check heap.*/
     iStatus = HEAPCHECK();
     
     /* Decode status. */
     switch(iStatus)
     {
          case _HEAPBADBEGIN:
               NWGFXErrorMessage("Bad heap header");
               break;

          case _HEAPBADNODE:
               NWGFXErrorMessage("Damaged heap");
               break;

          case _HEAPEMPTY:
               NWGFXErrorMessage("Uninitialised heap");
               break;

          case _HEAPOK:
               NWGFXInfoMessage("Heap appears ok");
               break;

          default:
               sprintf(Err, "Unknown HeapCheck error: %d", iStatus);
               NWGFXErrorMessage(Err);
               break;
     }
}
