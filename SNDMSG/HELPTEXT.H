/******************************************************************************
** (C) Chris Wood 1995
**
** HELP.H - The on-line help.
**
******************************************************************************* 
*/

/* Program strings. */
#define APPNAME     "SendMsg"
#define APPTITLE    "NetWare Message Sender v1.0"
#define COPYRIGHT   "(C) Chris Wood 1995"

/* Help for the server menu. */
HELPSTR hFServer[] = { "Choose which server the users or group, to",
                       "whom you are going to send the message, reside." };

/* Help for the target type menu. */
HELPSTR hUsrGrp[]  = { "Select whether you want to send the message to",
                       "one or more users, or a predefined group of",
                       "users." };

/* Help for the  menu. */
HELPSTR hUsers[]   = { "Select one or more users to send a message to." };

/* Help for the  menu. */
HELPSTR hGroups[]  = { "Select a group of users to send a message to." };

/* Help for the  menu. */
HELPSTR hMessage[] = { "Type the message you want to send." };

/* The main help array (width, height, title, num lines, text). */
NWGHELP hHelpText[] = { { 51, 6, "Select File Server", 2, hFServer },
                        { 50, 7, "To Whom",            3, hUsrGrp },
                        { 50, 5, "Select User(s)",     1, hUsers },
                        { 49, 5, "Select Group",       1, hGroups },
                        { 38, 5, "Message Text",       1, hMessage }
                      };
