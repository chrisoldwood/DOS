/******************************************************************************
** (C) Chris Wood 1995.
**
** FILEIO.C - Configuration file routines.
**
******************************************************************************* 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define MAX_LINE_LEN     256            /* Max length of config line. */

/* Node name structure. */
typedef struct tagNodeName
{
     char                Address[6];    /* Node address. */
     char                szName[10];    /* Node "name". */
     struct tagNodeName  *Next;         /* Next node in list. */
} NODENAME, * PNODENAME, * NODENAMELIST;

/**** Global Vars. ***********************************************************/
char szCfgExt[]="cfg";                  /* The config file name extension. */
char NoName[]="   ?   ";                /* Unknown name. */

NODENAMELIST NameList;                  /* List of node names. */
PNODENAME    pTail;                     /* Tail of list. */

/**** Function Prototypes. ***************************************************/
void ReadCfgFile(char *);
int  ReadLine(FILE *, char *, int);
void AddNodeName(char * pAddr, char * pName);
char Str2Hex(char *pString);

/******************************************************************************
** Read the config file.
*/
void ReadCfgFile(char *ExePath)
{
     int  iExeNameLen;                  /* Exe name length. */
     FILE *fCfgFile;                    /* Config file. */
     int  iRead;                        /* Chars read. */
     char szLineBuf[MAX_LINE_LEN];      /* Line buffer. */
     int  bDone = 0;                    /* Finished? */
     char szAddr[30];                   /* Nodes address. */
     char szName[30];                   /* Nodes name. */

     /* Get config filename from exe name. */
     iExeNameLen = strlen(ExePath);
     ExePath[iExeNameLen-3] = '\0';
     strcat(ExePath, szCfgExt);
     
     /* Open config file. */
     fCfgFile = fopen(ExePath, "r");
     if (fCfgFile == NULL)
          return;

     /* Read to EOF. */
     while(!bDone)
     {
          /* Get a line of text. */
          iRead = ReadLine(fCfgFile, szLineBuf, MAX_LINE_LEN);

          /* Check for EOF. */
          if (iRead == EOF)
          {
               bDone = 1;
               continue;
          }

          /* Check for comment / null line. */
          if ( (!iRead) || (szLineBuf[0] == '#') )
               continue;

          /* Extract name & address. */
          sscanf(szLineBuf, "%s %s", szName, szAddr);
          
          /* Add it to the list. */
          AddNodeName(szAddr, szName);
     }

     /* Close config file. */
     fclose(fCfgFile);
}

/******************************************************************************
** Read a line from a file.
*/
int ReadLine(FILE* fInput, char *pBuffer, int iBufSize)
{
     int    iRead;
     char   cChar;

     /* Read until EOL or EOF. */
     iRead = 0;
     cChar = (char) fgetc(fInput);
     while( (cChar != (char) 10) && (cChar != EOF) && (iRead < iBufSize) )
     {
          pBuffer[iRead++] = cChar;
          cChar = (char) fgetc(fInput);
     }

     /* Check for EOF. */
     if ( (!iRead) && (cChar == EOF) )
          return EOF;

     /* Check for NULL string. */
     if (!iRead)
     {
          /* Set to EOS. */
          pBuffer[0] = '\0';
          return 0;
     }

     /* Check for full buffer. */
     if (iRead == iBufSize)
     {
          /* Change last char to EOS. */
          pBuffer[iRead-1] = '\0';

          /* skip to EOL or EOF. */
          while( (cChar != (char) 10) && (cChar != EOF) )
               cChar = (char) fgetc(fInput);
     }

     /* Check for EOL. */
     if (cChar == (char) 10)
     {
          /* Add EOS. */
          pBuffer[iRead] = '\0';
     }

     /* Return chars read. */
     return iRead;
}

/******************************************************************************
** Add a new node name to the list.
*/
void AddNodeName(char * pAddr, char * pName)
{
     PNODENAME pNew;
     
     /* Create new node name. */
     pNew = (PNODENAME) malloc(sizeof(NODENAME));
     if (!pNew)
          return;

     /* Convert address. */
     pNew->Address[0] = Str2Hex(pAddr);      pAddr += 2;
     pNew->Address[1] = Str2Hex(pAddr);      pAddr += 2;
     pNew->Address[2] = Str2Hex(pAddr);      pAddr += 2;
     pNew->Address[3] = Str2Hex(pAddr);      pAddr += 2;
     pNew->Address[4] = Str2Hex(pAddr);      pAddr += 2;
     pNew->Address[5] = Str2Hex(pAddr);
     
     /* Copy in name (truncated). */
     pName[7] = '\0';
     strcpy(pNew->szName, pName);
     
     /* Check for null list. */
     if (NameList == NULL)
     {
          /* Set as root. */
          pNew->Next = NULL;
          pTail      = pNew;
          NameList   = pNew;
     }
     else /* List not empty. */
     {
          /* Tag on end. */
          pNew->Next  = NULL;
          pTail->Next = pNew;
          pTail       = pNew;
     }
}

/******************************************************************************
** Convert a two byte string to a 1-byt hex value.
*/
char Str2Hex(char *pString)
{
     char szTmp[3];                /* Tmp buffer. */
     int  iValue;                  /* Hex value as int. */
 
     /* Create 2 char string. */
     szTmp[0] = *pString++;
     szTmp[1] = *pString;
     szTmp[2] = '\0';
     
     /* Get value as int. */
     sscanf(szTmp, "%x", &iValue);
     
     /* return as BYTE. */
     return (char) iValue;
}

/******************************************************************************
** Find a name in the list.
*/
char * FindNodeName(char *pAddr)
{
     PNODENAME pTmp;               /* List scanning tmp. */

     /* Check for empty list. */
     if (!NameList)
          return NoName;
     
     /* Run along the list. */
     pTmp = NameList;
     while(pTmp)
     {
          /* Compare addresses. */
          if (memcmp(pAddr, pTmp->Address, 6) == 0)
               return pTmp->szName;
               
          /* Not found. */
          pTmp = pTmp->Next;
     }

     /* Didn't find it. */
     return NoName;
}
