/******************************************************************************
** (C) Chris Wood 1995.
**
** FIFOLIST.H - Circular FIFO list header.
**
******************************************************************************* 
*/

/**** Definitions. ***********************************************************/

/* Pointer to a block of data. */
typedef struct ECB * PECB;

/**** Structures. ************************************************************/

/* FIFO list structure. */
typedef struct tagFIFO
{
     int  iSize;              /* Size of list. */
     int  iHead;              /* Head of list. */
     int  iTail;              /* Tail of list. */
     int  iNumItems;          /* Number of items in list. */
     PECB *pItems;           /* Array of ECBs in the list. */
} FIFOLIST, *PFIFOLIST;

/**** Function Prototypes. ***************************************************/
int GetNumItems(PFIFOLIST pList);
void AddItem(PFIFOLIST pList, PECB pECB);
PECB GetItem(PFIFOLIST pList);
