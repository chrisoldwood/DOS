/******************************************************************************
** (C) Chris Wood 1995
**
** MTOSSUDG.H - Mailtoss Pmail UDG header file.
**
*******************************************************************************
*/

/* Check for previous include. */
#ifndef _MTOSSUDG_H_
#define _MTOSSUDG_H_

/**** Standard headers ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**** Global Definitions *****************************************************/
#ifndef TRUE
#define TRUE	1
#define FALSE	0
#endif

#ifndef OKAY
#define OKAY	1
#define ERROR	0
#endif

#ifndef EXIT_OKAY
#define EXIT_OKAY	0
#define EXIT_ERROR	1
#endif

/**** Global Data Types ******************************************************/

typedef void *			PVOID;
typedef char *			PSTR;
typedef int 			BOOL;
typedef unsigned int 	UINT;
typedef unsigned long	ULONG;

/**** Application headers ****************************************************/

#include "debug.h"

/**** Prototypes *************************************************************/

/* MTOSSUDG.C */
extern void ReportError(PSTR pszMsg);
extern void ReportFileError(PSTR pszMsg, PSTR pszFile);

/* CFGFILE.C */
extern BOOL ReadConfigFile(PSTR pszCfgFile);

#endif /* _MTOSSUDG_H_ */
