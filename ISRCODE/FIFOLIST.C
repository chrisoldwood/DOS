/******************************************************************************
** (C) Chris Wood 1995.
**
** FIFOLIST.C - Circular FIFO list code for storing ECBs.
**
******************************************************************************* 
*/

#include <nwipxspx.h>
#include "fifolist.h"

/******************************************************************************
** Get the number of items in the list.
*/
int GetNumItems(PFIFOLIST pList)
{
     return pList->iNumItems;
}

/******************************************************************************
** Stack checking must be removed for the following routine as it is called 
** with the temporary stack set up by the ESR handlers.
*/
#pragma check_stack ( off )

/******************************************************************************
** Add an item to end of the list.
*/
void AddItem(PFIFOLIST pList, PECB pECB)
{
     /* Check for room. */
     if (pList->iNumItems == pList->iSize)
          return;
          
     /* Add onto end. */
     pList->pItems[pList->iTail] = pECB;
     
     /* Update list. */
     pList->iNumItems++;
     pList->iTail = (pList->iTail + 1) % (pList->iSize);
}

#pragma check_stack ( on )

/******************************************************************************
** Get the next item in the list.
*/
PECB GetItem(PFIFOLIST pList)
{
     PECB pECB;
     
     /* Check if empty. */
     if (pList->iNumItems == 0)
          return (PECB) 0;
          
     /* Get from head. */
     pECB = pList->pItems[pList->iHead];
     
     /* Update list. */
     pList->iNumItems--;
     pList->iHead = (pList->iHead + 1) % (pList->iSize);
}
