/******************************************************************************
** (C) Chris Wood 1995
**
** HELP.H - The on-line help.
**
******************************************************************************* 
*/

/* Program strings. */
#define APPNAME     "Q-WATCHER"
#define APPTITLE    "NetWare Print Queue Monitor v1.1 (Alpha-7)"
#define COPYRIGHT   "(C) Chris Wood 1994/95"

/* Help for the server selection menu. */
HELPSTR hServer[] = {"ALL SERVERS:" ,
                     "Watch all queues on all the servers.",
                     "SPECIFIC SERVER:",
                     "Watch all the queues only on the server selected"  };

/* Help for queue monitor screen. */
HELPSTR hQueues[] = { "This displays basic information about the queues",
                      "chosen. If you select a queue further information",
                      "will be displayed." };

/* Help for queue job list menu. */
HELPSTR hJobList[] = { "This displays information about all the jobs in",
                       "the selected queue. From here you can select a job",
                       "to display more information about it, or delete",
                       "it. You can select multiple jobs for deletion." };

HELPSTR hJobInfo[] = { "This displays all the relevant information about", 
                       "the job selected. From here you can change the",
                       "jobs attributes or check out the print server",
                       "servicing the job" };

/* The main help array (width, height, title, num lines, text). */
NWGHELP hHelpText[] = { { 52, 8, "Select Server",   4, hServer },
                        { 53, 7, "Queue Monitor",   3, hQueues },
                        { 54, 8, "Queue Jobs",      4, hJobList },
                        { 52, 8, "Job Information", 4, hJobInfo } };
