/******************************************************************************
** (C) Chris Wood 1995
**
** HELPTEXT.H - The on-line help.
**
******************************************************************************* 
*/

/* Program strings. */
#define APPNAME     "ViewNet"
#define APPTITLE    "NetWare Network Viewer v1.0"
#define COPYRIGHT   "(C) Chris Wood 1995"

/* Help for the initial menu. */
HELPSTR hNetOpts[] = { "SCAN: Scans the immediate network this node",
                       "      is on." };

/* The main help array (width, height, title, num lines, text). */
NWGHELP hHelpText[] = { { 47, 6, "Network Options", 2, hNetOpts }
                      };
