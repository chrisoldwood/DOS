/*
** Report the COM port section of the
** BIOS Data Area (BDA).
*/

#include <stdio.h>

typedef unsigned short	WORD;

void main(void)
{
	WORD _far *	pBDA;
	int 		iPort;
	WORD		wAddr;

	/* BDA located at 0x0040:0x0000 */
	pBDA = (WORD _far*) 0x00400000L;
    
    /* For all 4 ports. */
	for (iPort=1; iPort <= 4; iPort++)
    {
    	/* Get port address. */
    	wAddr = *pBDA;
    	
    	if (wAddr != 0x0000)
    	{
    		printf("COM%d: Found at 0x%04X\n", iPort, wAddr);
    	}
    	else
    	{
    		printf("COM%d: Not defined\n", iPort);
    	}
    
    	/* Next port. */
    	pBDA++;
    }
}
