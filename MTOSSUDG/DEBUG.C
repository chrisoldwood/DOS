/*****************************************************************************
** (C) Chris Wood 1995.
**
** DEBUG.C - Debugging functions.
**
******************************************************************************
*/

#include "mtossudg.h"

#include <conio.h>

/* Check build type. */
#ifdef _DEBUG

#pragma message ( "DEBUG BUILD" )

/**** Defs. ******************************************************************/
#define DEBUG_MSG_LEN    128       /* Max length of debug message. */

/******************************************************************************
** My simple assert function. This checks the value and displays a message and
** exits if it fails.
*/
void MyAssert(PSTR pszModule, int iLineNum, BOOL bExpr)
{
     char szMsg[DEBUG_MSG_LEN];    /* The full message. */

     /* Check the expression. */
     if(!bExpr)
     {
          /* Copy the module and line number into the message. */
          sprintf(szMsg, "ASSERT Failed: Module %s Line %d\n", pszModule, iLineNum);

          /* Display message. */
          printf(szMsg);
          printf("\nPRESS ANY KEY TO CONTINUE...");
          
		/* Wait for key press. */
		while(!kbhit());
		
		/* Exit. */
		exit(EXIT_ERROR);
     }
}

#else /* NDEBUG. */

#pragma message ( "NON-DEBUG BUILD" )

#endif /* DEBUG. */
