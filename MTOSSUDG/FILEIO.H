/******************************************************************************
** (C) Chris Wood 1995
**
** FILEIO.H - Useful file functions header.
**
*******************************************************************************
*/

/* Check for previous include. */
#ifndef _FILEIO_H_
#define _FILEIO_H_

/**** Headers ****************************************************************/

#include <io.h>
#include <dos.h>
#include <direct.h>

/**** Global Definitions *****************************************************/
#define MAX_PATH_LEN		128		/* Max length of file path. */
#define FILENAME_LEN		12		/* Length of filename. */
#define FILEEXT_LEN			4		/* Length of file extension. */

#define MAX_FILENAME_LEN		(MAX_PATH_LEN + FILENAME_LEN)

/* _access() flags. */
#define FILE_HAS_MODE		0		/* File has this mode. */

#define FILE_EXISTS			0		/* File exists. */
#define FILE_WRITABLE		2		/* File is writable. */
#define FILE_READABLE		4		/* File is readable. */
#define FILE_READWRITE		6		/* File is R/W. */

/* _dos_findfirst() flags. */
#define FIND_NEXT			0xFFFF	/* Find next file. */
#define FILE_FOUND			0		/* File was found. */

/**** Prototypes *************************************************************/
extern BOOL FileExists(PSTR pszFile);
extern BOOL FileReadOnly(PSTR pszFile);
extern BOOL FindFile(PSTR pszPath, UINT *iType);
extern void CreateFullPathName(PSTR pszDir, PSTR pszFile, PSTR pszOutput);
extern void StripPath(PSTR pszPath);
extern void StripFileName(PSTR pszPath);
extern void StripExtension(PSTR pszFileName);
extern void GetPathFileName(PSTR pszPath, PSTR pszFileName);
extern BOOL MakeDir(PSTR pszDir);
extern BOOL DelDir(PSTR pszDir);
extern BOOL DelFile(PSTR pszFile);

#endif /* _FILEIO_H_ */
