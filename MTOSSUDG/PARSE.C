/******************************************************************************
** (C) Chris Wood 1995
**
** PARSE.C - Parse a mail message into the format WinPMail can use.
**
*******************************************************************************
*/

#include "mtossudg.h"
#include "fileio.h"

/**** Global Definitions *****************************************************/
#define MAX_BUFFER_LEN	512			/* Max length of read buffer. */
#define INIT_HEADER_LEN	55			/* Initial header length. */

char szOutput[MAX_FILENAME_LEN];		/* Full path to output file. */
char szBuffer[MAX_BUFFER_LEN];		/* Read buffer. */
char szHeader[INIT_HEADER_LEN];		/* Initial mail header. */

PSTR pszTo      = "To:";					/* Recipient. */
PSTR pszSubject = "Subject:";				/* Subject. */
PSTR pszSender  = "Reply-to:";			/* Sender. */
PSTR pszCC      = "CC:";					/* Carbon copy. */
PSTR pszBCC     = "BCC:";				/* Blind carbon copy. */
PSTR pszRead    = "X-Confirm-Reading-To:";	/* Confirm reading. */
PSTR pszRecvd   = "Return-receipt-to:";		/* Confirm delivery. */
PSTR pszUrgent  = "Priority: urgent";		/* Urgent message. */

extern char szDstDir[];				/* Destination directory. */
extern char szOrg[];				/* Organisation. */
extern char szDomain[];				/* Domain name. */

/**** Internal Prototypes ****************************************************/
PSTR FindData(PSTR pszText);
void ProcessRecipients(FILE *fInput, FILE *fOutput);
PSTR SkipSpaces(PSTR pszText);

/******************************************************************************
** This is the entry point to the parser.
** It runs through the header outputing the fields in WinPmails format and
** then copies the message verbatim.
*/
BOOL ParseMessage(PSTR pszInput, PSTR pszRecipient)
{
	char szFileName[MAX_FILENAME_LEN];		/* Temporary filename. */
	FILE *fInput;						/* Source file. */
	FILE *fOutput;						/* Destination file. */
	BOOL bInHeader = TRUE;				/* Processing header. */
	
	/* Get just the filename. */
	strcpy(szFileName, pszInput);
	StripPath(szFileName);
	
	/* Now create the full output pathname. */
	CreateFullPathName(szDstDir, szFileName, szOutput);
	
	/* Open the source. */
	fInput = fopen(pszInput, "r");
	if (!fInput)
		ReportFileError("Cannot open the file", pszInput);
		
	/* Open the destination. */
	fOutput = fopen(szOutput, "w");
	if (!fOutput)
	{
		fclose(fInput);
		ReportFileError("Cannot open the file", szOutput);
	}

	/*
	** Create initial header.
	*/
	
	/* Copy up to 20 chars of the recipient. */
	strncpy(szHeader, pszRecipient, 20);
	
	/* Add a separator. */
	strcat(szHeader, ", ");
	
	/* Add on some gibberish for now. */
	strcat(szHeader, "A Dummy Subject Line.....");
	
	/* Add on five spaces and two CR-LFs. */
	strcat(szHeader, "     \r\n");
	
	/* Write out inital header. */
	fputs(szHeader, fOutput);
     
     /* Write out this field, why? */
     fputs("SY:0\n", fOutput);
     
	/*
	** Process mail header.
	*/
	while(bInHeader)
	{
		PSTR pszData = NULL;		/* Start of field data. */
		PSTR pszField = NULL;		/* New field name. */
		BOOL bWriteField = TRUE;		/* Write out field? */
		     
		/* Read a line. */
		if (!fgets(szBuffer, MAX_BUFFER_LEN, fInput))
		{    
			/* Unexpected EOF. */
			fclose(fInput);
			fclose(fOutput);
			ReportFileError("Error reading file", pszInput);
		}
		
		/* Check for end of header. */
		if (szBuffer[0] == '\n')
		{
			/* No Copy Self? 
			fputs("CS:0\n", fOutput);*/
		
			/* No signature? 
			fputs("SS:1\n", fOutput);*/
	
			/* Write out end of header marker. */
			fputs(szBuffer, fOutput);
			
			/* Stop processing header. */
			bInHeader = FALSE;
			continue;
		}
		
		/* Get start of field data. */
		pszData = FindData(szBuffer);
		
		/*
		** Process field.
		*/
		
		if (_strnicmp(pszTo, szBuffer, strlen(pszTo)) == 0)
		{
			/* We'll write out the recipients here. */
			bWriteField = FALSE;
				
			/* Write out prefix. */
			fputs("TO:", fOutput);

			/* Contract the recipients. */
			ProcessRecipients(fInput, fOutput);
		}
		else	if (_strnicmp(pszSubject, szBuffer, strlen(pszSubject)) == 0)
		{
			/* Is subject. */
			pszField = "SU:";
		}
		else if (_strnicmp(pszSender, szBuffer, strlen(pszSender)) == 0)
		{
			/* Is sender. */
			pszField = "RE:";
		}
		else if (_strnicmp(pszCC, szBuffer, strlen(pszCC)) == 0)
		{
			/* We'll write out the recipients here. */
			bWriteField = FALSE;
				
			/* Write out prefix. */
			fputs("CC:", fOutput);

			/* Contract the recipients. */
			ProcessRecipients(fInput, fOutput);
		}
		else if (_strnicmp(pszBCC, szBuffer, strlen(pszBCC)) == 0)
		{
			/* We'll write out the recipients here. */
			bWriteField = FALSE;
				
			/* Write out prefix. */
			fputs("BCC:", fOutput);

			/* Contract the recipients. */
			ProcessRecipients(fInput, fOutput);
		}
		else if (_strnicmp(pszRead, szBuffer, strlen(pszRead)) == 0)
		{
			/* Is Confirm reading. */
			pszField = "RC:";
			pszData = "1\n";
		}
		else if (_strnicmp(pszRecvd, szBuffer, strlen(pszRecvd)) == 0)
		{
			/* Is Confirm delivery. */
			pszField = "DC:";
			pszData = "1\n";
		}
		else if (_strnicmp(pszUrgent, szBuffer, strlen(pszUrgent)) == 0)
		{
			/* Is Confirm delivery. */
			pszField = "UR:";
			pszData = "1\n";
		}
		else /* Unknown. */
		{
			/* Ignore field. */
			bWriteField = FALSE;
			
			/*fputs("->", fOutput);*/
			/*fputs(szBuffer, fOutput);*/
		}

		/* Write out new field name and data. */
		if (bWriteField)
		{
			fputs(pszField, fOutput);
			fputs(pszData, fOutput);
		}
	}
	
	/* Now copy message verbatim. */
	while(fgets(szBuffer, MAX_BUFFER_LEN, fInput))
		fputs(szBuffer, fOutput);
		
	/* Close both files.*/
	fclose(fInput);
	fclose(fOutput);
	
	return TRUE;
}

/******************************************************************************
** Find the start of the fields data.
*/
PSTR FindData(PSTR pszText)
{
	/* Skip up to ':' */
	while (*pszText != ':')
		pszText++;
	
	/* Skip ':'. */
	pszText++;
	
	/* Skip leading spaces. */
	while ( (*pszText == ' ') || (*pszText == '\t'))
		pszText++;
	
	/* Start of data. */
	return pszText;
}

/******************************************************************************
** Contract the recipients to one line of the output.
*/
void ProcessRecipients(FILE *fInput, FILE *fOutput)
{
	BOOL bDone = FALSE;
	PSTR pszData;				/* Start of field data. */

	/* Get start of field data. */
	pszData = FindData(szBuffer);

	/* Check for multiple recipients. */
	if (szBuffer[strlen(szBuffer)-2] != ',')
	{
		/* Write the single one out. */
		fputs(pszData, fOutput);
		return;
	}
			
	/* For each recipient. */
	while(!bDone)
	{
		/* Check for last. */
		if (szBuffer[strlen(szBuffer)-2] != ',')
		{
			/* Write out as is. */
			bDone = TRUE;
		}
		else /* Not last. */
		{
			/* Strip trailing '\n' */
			szBuffer[strlen(szBuffer)-1] = '\0';
		}
					
		/* Write out recipient. */
		fputs(pszData, fOutput);
					
		/* Only read next if not done. */
		if (!bDone)
		{
			/* Read next. */
			if (!fgets(szBuffer, MAX_BUFFER_LEN, fInput))
			{    
				/* Unexpected EOF. */
				fclose(fInput);
				fclose(fOutput);
				ReportError("Error reading input file");
			}
			
			/* Find the start of the recipient. */
			pszData = SkipSpaces(szBuffer);
		}
	}
}

/******************************************************************************
** Skip the spaces to the start of the data.
*/
PSTR SkipSpaces(PSTR pszText)
{
	/* Skip leading spaces. */
	while ( (*pszText == ' ') || (*pszText == '\t'))
		pszText++;
	
	/* Start of data. */
	return pszText;
}
