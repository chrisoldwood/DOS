/******************************************************************************
** (C) Chris Wood 1995
**
** HELPTEXT.H - The on-line help.
**
******************************************************************************* 
*/

/* Program strings. */
#define APPNAME     "PrintCap "
#define APPTITLE    "NetWare Print Capture Setup v1.0"
#define COPYRIGHT   "(C) Chris Wood 1995"

/* Help for the server menu. */
HELPSTR hLPTPort[] = { "Choose the printer port which you wish",
                       "to change the settings for." };

/* Help for the settings dialog menu. */
HELPSTR hLPTDlg[] =  { "From here you can change the way that",
                       "the printer port is captured." };

/* Help for the queue menu. */
HELPSTR hQueues[] =  { "Select the queue where you want your",
                       "printing to be sent." };

/* Help for the form menu. */
HELPSTR hForms[] =   { "Select the form type for the queue that",
                       "you will be sending to." };

/* Help for the tabs menu. */
HELPSTR hTabs[] =    { "This allows you to indicate whether you",
                       "want tabs to be expanded to space or not." };

/* Help for the banner menu. */
HELPSTR hBanner[] =  { "This allows you to print a banner page",
                       "that precedes the job." };

/* Help for the form feed menu. */
HELPSTR hFeed[] =    { "This allows you to send a form feed after",
                       "the job has finished" };

/* The main help array (width, height, title, num lines, text). */
NWGHELP hHelpText[] = { { 44, 6, "Select Port",      2, hLPTPort },
                        { 43, 6, "Capture Settings", 2, hLPTDlg },
                        { 42, 6, "Select Queue",     2, hQueues },
                        { 45, 6, "Select Form",      2, hForms },
                        { 47, 6, "Expand Tabs",      2, hTabs },
                        { 44, 6, "Print Banner",     2, hBanner },
                        { 47, 6, "Send Form Feed",   2, hFeed }
                      };
