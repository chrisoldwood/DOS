/******************************************************************************
** (C) Chris Wood 1995
**
** CFGFILE.C - Configuration file reading and passing routines.
**
*******************************************************************************
*/

#include "mtossudg.h"
#include "fileio.h"

/**** Global Definitions *****************************************************/
#define MAX_READBUF_LEN		256		/* Max length of read buffer. */
/*#define MAX_ORGNAME_LEN		100		 Max length of organisation. */
/*#define MAX_DOMAINNAME_LEN	100		 Max length of domain name. */

char szDstDir[MAX_FILENAME_LEN];		/* Destination directory. */
/*char szOrg[MAX_ORGNAME_LEN];			 Organisation. */
/*char szDomain[MAX_DOMAINNAME_LEN];		 Domain name. */

PSTR pszPathTag   = "MailPath";		/* Field name for outgoing mail path. */
/*PSTR pszOrgTag    = "Organisation";	 Field name for the organisation. */
/*PSTR pszDomainTag = "Domain";			 Field name for the domain name. */

/**** Internal Prototypes ****************************************************/
void ReadCfgData(FILE *fCfgFile, PSTR pszField, PSTR pszOutput);

/******************************************************************************
** Read and pass the config file.
** This expects the fields to be in a specific order.
*/
BOOL ReadConfigFile(PSTR pszCfgFile)
{
	FILE *fCfgFile;				/* The config file. */
	
	ASSERT(IsValidPtr(pszCfgFile));
	
	/* Open config file. */
	fCfgFile = fopen(pszCfgFile, "r");
	if (!fCfgFile)
		ReportFileError("Cannot open file", pszCfgFile);
	
	/* Read the outgoing mail path. */
	ReadCfgData(fCfgFile, pszPathTag, szDstDir);
	
	/* Read the organisation.
	ReadCfgData(fCfgFile, pszOrgTag, szOrg);
	
	/* Read the domain. 
	ReadCfgData(fCfgFile, pszDomainTag, szDomain);*/
	
	/* Close config file. */
	fclose(fCfgFile);
	
	return TRUE;
}

/******************************************************************************
** Read a field from the file. It automatically closes the file on error, and
** reports it.
*/
void ReadCfgData(FILE *fCfgFile, PSTR pszField, PSTR pszOutput)
{
	BOOL bFound = FALSE;
	char	szBuffer[MAX_READBUF_LEN];
	
	/* Read strings until found or EOF. */
	while(!bFound)
	{
		/* Read a line. */
		if (!fgets(szBuffer, MAX_READBUF_LEN, fCfgFile))
		{
			char szMsg[128];
			
			/* Read error or EOF. */
			fclose(fCfgFile);

			/* Make error string and report. */
			sprintf(szMsg, "Could not find config entry:- %s", pszField);
			ReportError(szMsg);
		}
		
		/* Check for comment or null line. */
		if ( (szBuffer[0] == ';') || (szBuffer[0] == '\r') || (szBuffer[0] == '\n') )
			continue;
			
		/* Check field name. */
		if (_strnicmp(szBuffer, pszField, strlen(pszField)) == 0)
		{
			PSTR pszData;
			int	iStrLen;
			
			/* Get start of data. */
			pszData = strchr(szBuffer, '=');
			ASSERT(IsValidPtr(pszData));
			
			/* Remove CR-LF if included. */
			iStrLen = strlen(szBuffer);
			if (szBuffer[iStrLen-1] == '\n')
				szBuffer[iStrLen-1] = '\0';
			
			/* Skip "=" and extract data. */
			pszData++;
			strcpy(pszOutput, pszData);
			
			/* break. */
			bFound = TRUE;
			continue;
		}
	}
}
