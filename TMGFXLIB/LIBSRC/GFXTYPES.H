/******************************************************************************
** (C) Chris Wood 1997
**
** MODULE:		GFXTYPES.H
** COMPONENT:	Text Mode Graphics Library.
** DESCRIPTION:	Common data types, defines and macros.
**
*******************************************************************************
*/

/* Check for previous inclusion */
#ifndef GFXTYPES_H
#define GFXTYPES_H

/******************************************************************************
**
** Standard types.
**
*******************************************************************************
*/

typedef unsigned char	uchar;
typedef unsigned short	ushort;
typedef unsigned int	uint;
typedef unsigned long	ulong;
typedef signed   int	bool;

/******************************************************************************
**
** Standard defines.
**
*******************************************************************************
*/

#ifndef TRUE
#define TRUE	1
#define FALSE	0
#endif

/******************************************************************************
**
** Size specific types.
**
*******************************************************************************
*/

typedef unsigned char	byte;
typedef unsigned short	word;
typedef unsigned long	dword;

/******************************************************************************
**
** Library specific types.
**
*******************************************************************************
*/

/******************************************************************************
**
** The screen colours.
**
** This type represents all the colours that can be displayed on both colour
** and mono screens.
**
** Note: Only the first 8 colours can be used as a background colour.
*/

typedef enum tagColour
{
	/* Mono and colour */
	Black     = 0,

	/* Mono only */
	Normal    = 7,
	Bright    = 15,

	/* Colour only */
	Blue      = 1,
	Green     = 2,
	Cyan      = 3,
	Red       = 4,
	Magenta   = 5,
	Brown     = 6,
	LtGrey    = 7,
	DkGrey    = 8,
	LtBlue    = 9,
	LtGreen   = 10,
	LtCyan    = 11,
	LtRed     = 12,
	LtMagenta = 13,
	Yellow    = 14,
	White     = 15
} Colour;

/******************************************************************************
**
** Extended character code aliases.
**
*******************************************************************************
*/

/* Single pixel lines */
#define S_HORZBAR		0xC4
#define S_VERTBAR		0xB3
#define S_TOPLEFT		0xDA
#define S_TOPRIGHT		0xBF
#define S_BOTRIGHT		0xD9
#define S_BOTLEFT		0xC0
#define S_SIDELEFT		0xC3
#define S_TOPMID		0xC2
#define S_SIDERIGHT		0xB4
#define S_BOTMID		0xC1
#define S_CROSS			0xC5

/* Double pixel lines */
#define D_HORZBAR		0xCD
#define D_VERTBAR		0xBA
#define D_TOPLEFT		0xC9
#define D_TOPRIGHT		0xBB
#define D_BOTRIGHT		0xBC
#define D_BOTLEFT		0xC8
#define D_SIDELEFT		0xCC
#define D_TOPMID		0xCB
#define D_SIDERIGHT		0xB9
#define D_BOTMID		0xCA
#define D_CROSS			0xCE

/* Graphic blocks */
#define SPARSEBLOCK		0xB0
#define QTRBLOCK		0xB1
#define HALFBLOCK		0xB2
#define FULLBLOCK		0xDB

/* 'proper' arrows */
#define LEFTARROW		0x1B
#define RIGHTARROW		0x1A
#define UPARROW			0x18
#define DOWNARROW		0x19
#define UDARROW			0x12
#define LRARROW			0x1D

/* 'nudge' arrows */
#define LEFTNUDGE		0x11
#define RIGHTNUDGE		0x10
#define UPNUDGE			0x1E
#define DOWNNUDGE		0x1F

/* Other useful chars */
#define TICK			0xFB
#define APPROX			0xF7

#endif /* GFXTYPES_H */
