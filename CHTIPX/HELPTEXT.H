/******************************************************************************
** (C) Chris Wood 1995
**
** HELP.H - The on-line help.
**
******************************************************************************* 
*/

/* Program strings. */
#define APPNAME     "ChatIPX"
#define APPTITLE    "NetWare IPX Chat v1.0"
#define COPYRIGHT   "(C) Chris Wood 1995"

/* Help for the users menu. */
HELPSTR hCall[] =    { "Select whether you are going to be making",
                       "the initial call, or accepting one from",
                       "from another user." };

/* The main help array (width, height, title, num lines, text). */
NWGHELP hHelpText[] = { { 45, 7, "Call Type", 3, hCall }
                      };
