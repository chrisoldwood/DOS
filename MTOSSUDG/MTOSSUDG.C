/******************************************************************************
** (C) Chris Wood 1995
**
** MTOSSUDG.C - Mailtoss Pmail UDG entry and exit point.
**
*******************************************************************************
*/

#include "mtossudg.h"
#include "fileio.h"

#include <conio.h>

/**** Global Definitions *****************************************************/
PSTR pszAppName     = "MTOSSUDG.EXE";		/* Application name. */
PSTR pszCfgFileName = "MTOSSUDG.CFG";		/* Configuration filename. */

char szAppPath[MAX_FILENAME_LEN];			/* Application path. */
char szCfgFile[MAX_FILENAME_LEN];			/* Full path to config file. */

/**** External Prototypes ****************************************************/
BOOL ParseMessage(PSTR pszMailFile, PSTR pszRecipient);

/******************************************************************************
** Program entry point.
** 
** This expects two command line arguments:-
** ~c The path to the mail file.
** ~t The recipient.
*/
main(int argc, char *argv[])
{
	PSTR pszPathEnd;				/* End of application path. */
	
	if(argc);
	/* Check number of command line args. 
	if (--argc != 2)
		ReportError("Invalid number of parameters. Expected:- ~c ~t"); */

	/* Copy application path to a buffer. */
	strcpy(szAppPath, argv[0]);

	/* Find last slash in path and truncate path. */
	pszPathEnd = strrchr(szAppPath, '\\');
	ASSERT(IsValidPtr(pszPathEnd));
	*(++pszPathEnd) = '\0';
	
	/* Create config file path and check it exists. */
	CreateFullPathName(szAppPath, pszCfgFileName, szCfgFile);
	if (!FileExists(szCfgFile))
		ReportFileError("Cannot locate the following file", szCfgFile);
	
	/* Read in the configuration options. */
	if (!ReadConfigFile(szCfgFile))
		exit(EXIT_ERROR);
	
	/* Parse the mail message. */
	if (!ParseMessage(argv[1], argv[2]))
		exit(EXIT_ERROR);
	
	/* Success. */
	return EXIT_OKAY;
}

/******************************************************************************
** Display an error message, wait for a key press and then exit.
*/
void ReportError(PSTR pszMsg)
{
	char szErrorMsg[256];		/* Error message buffer. */

	ASSERT(IsValidPtr(pszMsg));
	
	/* Create full message and display. */
	sprintf(szErrorMsg, "%s: %s\n", pszAppName, pszMsg);
	printf(szErrorMsg);
     printf("\nPRESS ANY KEY TO CONTINUE...");
	
	/* Wait for key press. */
	while(!kbhit());

	/* Failed! */
	exit(EXIT_ERROR);
}

/******************************************************************************
** Report a file error.
** This just calls the above function but takes an extra string parameter.
*/
void ReportFileError(PSTR pszMsg, PSTR pszFile)
{
	char szErrorMsg[256];		/* Error message buffer. */

	ASSERT(IsValidPtr(pszMsg));
	ASSERT(IsValidPtr(pszFile));
	
	/* Create full message. */
	sprintf(szErrorMsg, "%s...\n%s", pszMsg, pszFile);
	
	/* Now call the above function. */
	ReportError(szErrorMsg);
}
