/******************************************************************************
** (C) Chris Wood 1995
**
** FILEIO.C - Useful file routines.
**
*******************************************************************************
*/

#include "mtossudg.h"
#include "fileio.h"

/******************************************************************************
** Check if a file exists.
*/
BOOL FileExists(PSTR pszFile)
{
	ASSERT(IsValidPtr(pszFile));

	/* Check access mode. */
	if (_access(pszFile, FILE_EXISTS) == FILE_HAS_MODE)
		return TRUE;
	
	return FALSE;
}

/******************************************************************************
** Check if a file is read-only.
*/
BOOL FileReadOnly(PSTR pszFile)
{
	ASSERT(IsValidPtr(pszFile));

	/* Check access mode. */
	if (_access(pszFile, FILE_WRITABLE) != FILE_HAS_MODE)
		return TRUE;
	
	return FALSE;
}

/******************************************************************************
** Find files in a path. This wraps up the calls to find-first and find-next by
** using the path as the return buffer and the file type as a flag to the first
** or next call.
*/
BOOL FindFile(PSTR pszPath, UINT *iType)
{
static struct _find_t	FindData;
static char			szFilePath[MAX_FILENAME_LEN+1];

	ASSERT(IsValidPtr(pszPath));

	/* Check for first or next call. */
	if (*iType != FIND_NEXT)
	{
		/* First copy to NEAR buffer. */
		strcpy(szFilePath, pszPath);
		
		/* Find first file. */
		if (_dos_findfirst(szFilePath, *iType, &FindData) == FILE_FOUND)
		{
			/* Strip search name from path. */
			StripFileName(szFilePath);

			/* Copy filename into buffer. */
			StripFileName(pszPath);
			strcat(pszPath, FindData.name);
			
			*iType = FIND_NEXT;
			return TRUE;
		}
	}
	else /* *iType == FIND_NEXT */
	{
		/* Find next file. */
		if (_dos_findnext(&FindData) == FILE_FOUND)
		{
			/* Copy filename into buffer. */
			strcpy(pszPath, szFilePath);
			strcat(pszPath, FindData.name);
			return TRUE;
		}
	}
	
	return FALSE;
}

/******************************************************************************
** Create a full pathname from a path and a filename.
*/
void CreateFullPathName(PSTR pszDir, PSTR pszFile, PSTR pszOutput)
{
	UINT iStrLen;

	ASSERT(IsValidPtr(pszDir));
	ASSERT(IsValidPtr(pszFile));
	ASSERT(IsValidPtr(pszOutput));
	
	/* Copy in directory. */
	strcpy(pszOutput, pszDir);
	
	/* Check for trailing \. */
	iStrLen = strlen(pszDir);
	if (pszDir[iStrLen-1] != '\\')
		strcat(pszOutput, "\\");
		
	/* Add in file name. */
	strcat(pszOutput, pszFile);
}

/******************************************************************************
** Strip the path to leave the filename.
*/
void StripPath(PSTR pszPath)
{
	PSTR 	pszFileName;
	int		iStrLen;
	
	ASSERT(IsValidPtr(pszPath));

	/* Go to end of string. */
	iStrLen = strlen(pszPath);
	pszFileName = &pszPath[iStrLen-1];
	
	/* Search back to first \ or : */
	while ( (*pszFileName != '\\') && (*pszFileName != ':') )
		pszFileName--;
	
	/* Move back to start of filename. */
	ASSERT((pszFileName >= pszPath));
	pszFileName++;
	
	/* Copy filename to start of buffer. */
	while (*pszFileName != '\0')
		*pszPath++ = *pszFileName++;
		
	/* Copy in EOL. */
	*pszPath = '\0';
}

/******************************************************************************
** Strip the filename to leave the path.
*/
void StripFileName(PSTR pszPath)
{
	PSTR 	pszFileName;
	int		iStrLen;
	
	ASSERT(IsValidPtr(pszPath));
	
	/* Go to end of string. */
	iStrLen = strlen(pszPath);
	pszFileName = &pszPath[iStrLen-1];
	
	/* Search back to first \ or : replacing chars with EOL. */
	while ( (*pszFileName != '\\') && (*pszFileName != ':') )
		*pszFileName-- = '\0';

	ASSERT((pszFileName >= pszPath));
}

/******************************************************************************
** Strip the extension off a file.
*/
void StripExtension(PSTR pszFileName)
{
	ASSERT(IsValidPtr(pszFileName));

	/* Find the last . or EOS. */
	while ( (*pszFileName != '\0') && (*pszFileName != '.') )
		pszFileName++;
	
	/* EOS. */
	if (*pszFileName == '.')
		*pszFileName = '\0';
}

/******************************************************************************
** Get the filename only from the full path.
*/
void GetPathFileName(PSTR pszPath, PSTR pszFileName)
{
	PSTR	pszName;
	int		iStrLen;

	ASSERT(IsValidPtr(pszPath));
	ASSERT(IsValidPtr(pszFileName));

	/* Go to end of string. */
	iStrLen  = strlen(pszPath);
	pszName = &pszPath[iStrLen-1];
	
	/* Search back to first \ or : */
	while ( (*pszName != '\\') && (*pszName != ':') )
		pszName--;
	
	/* Move back to start of filename. */
	ASSERT((pszName >= pszPath));
	pszName++;
	
	/* Copy filename to output buffer. */
	while (*pszName != '\0')
		*pszFileName++ = *pszName++;
		
	/* Copy in EOL. */
	*pszFileName = '\0';
}

/******************************************************************************
** Make a new directory.
*/
BOOL MakeDir(PSTR pszDir)
{
	ASSERT(IsValidPtr(pszDir));

	/* Make. */
	if (_mkdir(pszDir) == 0)
		return TRUE;
	
	return FALSE;
}

/******************************************************************************
** Delete a directory.
*/
BOOL DelDir(PSTR pszDir)
{
	ASSERT(IsValidPtr(pszDir));

	/* Delete. */
	if (_rmdir(pszDir) == 0)
		return TRUE;
	
	return FALSE;
}

/******************************************************************************
** Delete a file.
*/
BOOL DelFile(PSTR pszFile)
{
	ASSERT(IsValidPtr(pszFile));

	/* Delete. */
	if (remove(pszFile) == 0)
		return TRUE;
	
	return FALSE;
}
