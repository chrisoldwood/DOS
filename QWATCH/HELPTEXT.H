/******************************************************************************
** (C) Chris Wood 1995
**
** HELP.H - The on-line help.
**
******************************************************************************* 
*/

/* Program strings. */
#define APPNAME     "Q-WATCHER"
#define APPTITLE    "NetWare Print Queue Monitor v1.3"
#define COPYRIGHT   "(C) Chris Wood 1994/95"

/* Help for the server selection menu. */
HELPSTR hFServer[] = {"ALL SERVERS:" ,
                      "Watch all queues on all the servers.",
                      "SPECIFIC SERVER:",
                      "Watch all the queues only on the server selected"  };

/* Help for queue monitor screen. */
HELPSTR hQueues[] = { "This displays basic information about the queues",
                      "chosen. If you select a queue further information",
                      "will be displayed about all the jobs in it." };

/* Help for queue job list menu. */
HELPSTR hJobList[] = { "This displays information about all the jobs in",
                       "the selected queue. From here you can select a job",
                       "to display more information about it, or delete",
                       "it. You can select multiple jobs for deletion." };

/* Help for the job information dialog. */
HELPSTR hJobInfo[] = { "This displays all the relevant information about", 
                       "the job selected. From here you can change the",
                       "jobs attributes or check out the print server",
                       "servicing the job. This assumes that you have",
                       "enough rights.",
                       "FORM: This will bring up a menu to allow you to",
                       "change the jobs' form.",
                       "STATUS: This will also bring up a menu which",
                       "will let you put a job on hold, either a user",
                       "or an operator hold, or make it ready.",
                       "SERVER: This displays the server servicing the",
                       "job. If the job is being serviced you can see",
                       "information about the server by selecting it.",
                       "POSITION: From here you can move the job to",
                       "the top or bottom of the queue by a menu." };

/* Help for print server menu. */
HELPSTR hPServer[] = { "Select the print server which you want to",
                       "monitor." };

/* Help for print server info dialog. */
HELPSTR hPSInfo[] = { "This displays information about the printer",
                      "serving the queue selected. It will display",
                      "the progress of the current job." };

/* The main help array (width, height, title, num lines, text). */
NWGHELP hHelpText[] = { { 52, 8, "Select File Server",   4, hFServer },
                        { 53, 7, "Queue Monitor",        3, hQueues },
                        { 54, 8, "Queue Jobs",           4, hJobList },
                        { 52, 9, "Job Information",     15, hJobInfo },
                        { 45, 6, "Select Print Server",  2, hPServer },
                        { 47, 7, "Server Information",   3, hPSInfo } };
