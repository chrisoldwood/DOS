/******************************************************************************
** (C) Chris Wood 1995.
**
** PRTFORMS.H - Structures, defs & prototypes for accessing print forms.
**
******************************************************************************* 
*/

/**** Global Defs ************************************************************/ 
#define MAX_PRINT_FORMS  50        /* Maximum supported forms, up to 255 really! */
#define MAX_FMNAME_LEN   50        /* Maximum form name length. */

/**** Global Structures ******************************************************/ 

/* A print form. */
typedef struct tagPrintForm {
     char      Name[MAX_FMNAME_LEN+1];       /* The servers' name. */
     WORD      Number;                       /* The forms' name. */
     WORD      Length;                       /* The forms' length. */
     WORD      Width;                        /* The forms' width. */
} PRINTFORM, *PPRINTFORM, *PF_LIST;          /* Aliases. */

/**** Prototypes *************************************************************/ 
void GetForms(PPRINTQUEUE pQueue);
int GetNumForms(void);
char * GetFormName(int iIndex);
WORD GetFormNumber(int iIndex);
int GetFormMaxNameLen(void);
