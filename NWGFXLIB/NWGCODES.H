/******************************************************************************
** (C) Chris Wood 1994/95.
**
** NWGCODES.H - ID's for colours, extended characters and keyboard codes.
**
******************************************************************************* 
*/

/* First check for previous inclusion. */
#ifndef _NWGCODES_H
#define _NWGCODES_H

/**** Colour-Display colours. ************************************************/
#define BLACK            (NWGCOLOUR) 0x00
#define BLUE             (NWGCOLOUR) 0x01
#define GREEN            (NWGCOLOUR) 0x02
#define CYAN             (NWGCOLOUR) 0x03
#define RED              (NWGCOLOUR) 0x04
#define MAGENTA          (NWGCOLOUR) 0x05
#define BROWN            (NWGCOLOUR) 0x06
#define LTGREY           (NWGCOLOUR) 0x07
#define DKGREY           (NWGCOLOUR) 0x08
#define LTBLUE           (NWGCOLOUR) 0x09
#define LTGREEN          (NWGCOLOUR) 0x0A
#define LTCYAN           (NWGCOLOUR) 0x0B
#define LTRED            (NWGCOLOUR) 0x0C
#define LTMAGENTA        (NWGCOLOUR) 0x0D
#define YELLOW           (NWGCOLOUR) 0x0E
#define WHITE            (NWGCOLOUR) 0x0F

/**** Mono-Display colours. **************************************************/
#define DARK             (NWGCOLOUR) 0x00
#define NORMAL           (NWGCOLOUR) 0x07
#define BRIGHT           (NWGCOLOUR) 0x0F

/**** Extended character codes for single pixel lines. ***********************/
#define S_HORZBAR        (NWGCHAR) 0xC4
#define S_VERTBAR        (NWGCHAR) 0xB3
#define S_TOPLEFT        (NWGCHAR) 0xDA
#define S_TOPRIGHT       (NWGCHAR) 0xBF
#define S_BOTRIGHT       (NWGCHAR) 0xD9
#define S_BOTLEFT        (NWGCHAR) 0xC0
#define S_SIDELEFT       (NWGCHAR) 0xC3
#define S_TOPMID         (NWGCHAR) 0xC2
#define S_SIDERIGHT      (NWGCHAR) 0xB4
#define S_BOTMID         (NWGCHAR) 0xC1
#define S_CROSS          (NWGCHAR) 0xC5

/**** Extended character codes for double pixel lines. ***********************/
#define D_HORZBAR        (NWGCHAR) 0xCD
#define D_VERTBAR        (NWGCHAR) 0xBA
#define D_TOPLEFT        (NWGCHAR) 0xC9
#define D_TOPRIGHT       (NWGCHAR) 0xBB
#define D_BOTRIGHT       (NWGCHAR) 0xBC
#define D_BOTLEFT        (NWGCHAR) 0xC8
#define D_SIDELEFT       (NWGCHAR) 0xCC
#define D_TOPMID         (NWGCHAR) 0xCB
#define D_SIDERIGHT      (NWGCHAR) 0xB9
#define D_BOTMID         (NWGCHAR) 0xCA
#define D_CROSS          (NWGCHAR) 0xCE

/**** Extended character codes for graphic blocks. ***************************/
#define SPARSEBLOCK      (NWGCHAR) 0xB0
#define QTRBLOCK         (NWGCHAR) 0xB1
#define HALFBLOCK        (NWGCHAR) 0xB2
#define FULLBLOCK        (NWGCHAR) 0xDB

/**** Extended character codes for 'proper' arrows. **************************/
#define LEFTARROW        (NWGCHAR) 0x1B
#define RIGHTARROW       (NWGCHAR) 0x1A
#define UPARROW          (NWGCHAR) 0x18
#define DOWNARROW        (NWGCHAR) 0x19
#define UDARROW          (NWGCHAR) 0x12
#define LRARROW          (NWGCHAR) 0x1D

/**** Extended character codes for 'nudge' arrows. ***************************/
#define LEFTNUDGE        (NWGCHAR) 0x11
#define RIGHTNUDGE       (NWGCHAR) 0x10
#define UPNUDGE          (NWGCHAR) 0x1E
#define DOWNNUDGE        (NWGCHAR) 0x1F

/**** Extended character codes for some other useful chars. ******************/
#define TICK             (NWGCHAR) 0xFB
#define APPROX           (NWGCHAR) 0xF7

/**** Extended & useful keycodes. ********************************************/
#define KEY_TIMEOUT      (NWGKEY) 0x0000    /* Special to denote timeout. */
#define KEY_BACKSPACE    (NWGKEY) 0x0008
#define KEY_RETURN       (NWGKEY) 0x000D
#define KEY_ESCAPE       (NWGKEY) 0x001B

#define KEY_UP           (NWGKEY) 0xFF48
#define KEY_DOWN         (NWGKEY) 0xFF50
#define KEY_LEFT         (NWGKEY) 0xFF4B
#define KEY_RIGHT        (NWGKEY) 0xFF4D
#define KEY_CTL_LEFT     (NWGKEY) 0xFF73
#define KEY_CTL_RIGHT    (NWGKEY) 0xFF74

#define KEY_PAGEUP       (NWGKEY) 0xFF49
#define KEY_PAGEDOWN     (NWGKEY) 0xFF51
#define KEY_CTL_PAGEUP   (NWGKEY) 0xFF84
#define KEY_CTL_PAGEDOWN (NWGKEY) 0xFF76

#define KEY_HOME         (NWGKEY) 0xFF47
#define KEY_END          (NWGKEY) 0xFF4F
#define KEY_CTL_HOME     (NWGKEY) 0xFF77
#define KEY_CTL_END      (NWGKEY) 0xFF78

#define KEY_INSERT       (NWGKEY) 0xFF52
#define KEY_DELETE       (NWGKEY) 0xFF53

#define KEY_F1           (NWGKEY) 0xFF3B
#define KEY_F2           (NWGKEY) 0xFF3C
#define KEY_F3           (NWGKEY) 0xFF3D
#define KEY_F4           (NWGKEY) 0xFF3E
#define KEY_F5           (NWGKEY) 0xFF3F
#define KEY_F6           (NWGKEY) 0xFF40
#define KEY_F7           (NWGKEY) 0xFF41
#define KEY_F8           (NWGKEY) 0xFF42
#define KEY_F9           (NWGKEY) 0xFF43
#define KEY_F10          (NWGKEY) 0xFF44
#define KEY_F11          (NWGKEY) 0xFF85
#define KEY_F12          (NWGKEY) 0xFF86

#define KEY_SFT_F1       (NWGKEY) 0xFF54
#define KEY_SFT_F2       (NWGKEY) 0xFF55
#define KEY_SFT_F3       (NWGKEY) 0xFF56
#define KEY_SFT_F4       (NWGKEY) 0xFF57
#define KEY_SFT_F5       (NWGKEY) 0xFF58
#define KEY_SFT_F6       (NWGKEY) 0xFF59
#define KEY_SFT_F7       (NWGKEY) 0xFF5A
#define KEY_SFT_F8       (NWGKEY) 0xFF5B
#define KEY_SFT_F9       (NWGKEY) 0xFF5C
#define KEY_SFT_F10      (NWGKEY) 0xFF5D
#define KEY_SFT_F11      (NWGKEY) 0xFF87
#define KEY_SFT_F12      (NWGKEY) 0xFF88

#define KEY_CTL_F1       (NWGKEY) 0xFF5E
#define KEY_CTL_F2       (NWGKEY) 0xFF5F
#define KEY_CTL_F3       (NWGKEY) 0xFF60
#define KEY_CTL_F4       (NWGKEY) 0xFF61
#define KEY_CTL_F5       (NWGKEY) 0xFF62
#define KEY_CTL_F6       (NWGKEY) 0xFF63
#define KEY_CTL_F7       (NWGKEY) 0xFF64
#define KEY_CTL_F8       (NWGKEY) 0xFF65
#define KEY_CTL_F9       (NWGKEY) 0xFF66
#define KEY_CTL_F10      (NWGKEY) 0xFF67
#define KEY_CTL_F11      (NWGKEY) 0xFF89
#define KEY_CTL_F12      (NWGKEY) 0xFF8A

#define KEY_ALT_F1       (NWGKEY) 0xFF68
#define KEY_ALT_F2       (NWGKEY) 0xFF69
#define KEY_ALT_F3       (NWGKEY) 0xFF6A
#define KEY_ALT_F4       (NWGKEY) 0xFF6B
#define KEY_ALT_F5       (NWGKEY) 0xFF6C
#define KEY_ALT_F6       (NWGKEY) 0xFF6D
#define KEY_ALT_F7       (NWGKEY) 0xFF6E
#define KEY_ALT_F8       (NWGKEY) 0xFF6F
#define KEY_ALT_F9       (NWGKEY) 0xFF70
#define KEY_ALT_F10      (NWGKEY) 0xFF71
#define KEY_ALT_F11      (NWGKEY) 0xFF8B
#define KEY_ALT_F12      (NWGKEY) 0xFF8C

/* _NWGCODES_H */
#endif
