#include "tmgfx.h"
#include <conio.h>

void main(void)
{
	DC		dc;
	Rect	rcRect;
	
	OpenDisplay(&dc);
	ClearDisplay(&dc);
	HideCursor(&dc);

	dc.eBgMode = Opaque;
	dc.eBgColour = Blue;
	
	HorzLine(&dc, 0, 0, 80, ' ', White);
	HorzLine(&dc, 0, 24, 80, ' ', Black);

	dc.eBgMode = Opaque;
	dc.eBgColour = LtGrey;
	
	rcRect.x1 = 0;
	rcRect.y1 = 1;
	rcRect.x2 = 79;
	rcRect.y2 = 23; 

	FillRect(&dc, &rcRect, ' ', Black);
	
	rcRect.x1 = 0;
	rcRect.y1 = 1;
	rcRect.x2 = 79;
	rcRect.y2 = 23; 

	Frame(&dc, &rcRect, DoubleLine, Black);
	VertFrameLine(&dc, 15, 1, 23, DoubleLine, Black);

	dc.eBgMode = Transparent;

	rcRect.x1 = 0;
	rcRect.y1 = 0;
	rcRect.x2 = 79;
	rcRect.y2 = 0;

	CentreText(&dc, &rcRect, "File Mangler", White);
    LeftText(&dc, 2, 1, "Directories", Black);
    LeftText(&dc, 17, 1, "Files", Black);

	rcRect.x1 = 20;
	rcRect.y1 = 5;
	rcRect.x2 = 30;
	rcRect.y2 = 15;
	Frame(&dc, &rcRect, SingleLine, Magenta);
    
	
	LeftText(&dc, 21,  6, "AAAAAAAAA", Blue);
	LeftText(&dc, 21,  7, "ABBBBBBBB", Blue);
	LeftText(&dc, 21,  8, "ABCCCCCCC", Blue);
	LeftText(&dc, 21,  9, "ABCDDDDDD", Blue);
	LeftText(&dc, 21, 10, "ABCDEEEEE", Blue);
	LeftText(&dc, 21, 11, "ABCDEFFFF", Blue);
	LeftText(&dc, 21, 12, "ABCDEFGGG", Blue);
	LeftText(&dc, 21, 13, "ABCDEFGHH", Blue);
	LeftText(&dc, 21, 14, "ABCDEFGHI", Blue);
	

	while(!kbhit());
	_getch();

	CloseDisplay();
}
