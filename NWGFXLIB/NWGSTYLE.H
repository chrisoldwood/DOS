/******************************************************************************
** (C) Chris Wood 1994/95.
**
** NWGSTYLE.H - General styles relating to windows and text.
**
******************************************************************************* 
*/

/* First check for previous inclusion. */
#ifndef _NWGSTYLE_H
#define _NWGSTYLE_H

/**** Window frame styles. ***************************************************/
#define FRAME_SINGLE     0         /* Window border uses single pixel line chars. */
#define FRAME_DOUBLE     1         /* Window border uses double pixel line chars. */

#define INACTIVE         0         /* Window border drawn as inactive. */
#define ACTIVE           1         /* Window border drawn as active. */

/**** Text output styles. ****************************************************/
#define TEXT_LEFT        0         /* Point defines leftmost character of string. */
#define TEXT_RIGHT       1         /* Point defines rightmost character of string. */
#define TEXT_CENTRE      2         /* Point defines central character of string. */

/**** Screen metric interrogation codes. *************************************/
#define SCREEN_WIDTH     0         /* Screen width in characters. */
#define SCREEN_HEIGHT    1         /* Screen height in characters. */
#define SCREEN_TYPE      2         /* Screen type:- colour or mono.*/

/**** Screen type. ***********************************************************/
#define MONO_SCREEN      0         /* Display is mono. */
#define COLOUR_SCREEN    1         /* Display is colour. */

/**** Colour interrogation codes. ********************************************/
#define COLOUR_WINDOWBG  3         /* Colour of window background. */
#define COLOUR_ACTIVE    4         /* Colour of active window border. */
#define COLOUR_INACTIVE  5         /* Colour of inactive window border. */
#define COLOUR_TITLEBG   6         /* Colour of title bar background. */
#define COLOUR_TITLETEXT 7         /* Colour of title bar text. */
#define COLOUR_ERRORBG   8        /* Colour of error message background. */
#define COLOUR_ERRORTEXT 9        /* Colour of error message text. */
#define COLOUR_INFOBG    10        /* Colour of info message background. */
#define COLOUR_INFOTEXT  11        /* Colour of info message text. */
#define COLOUR_HELPBG    12        /* Colour of help window background. */
#define COLOUR_HELPTEXT  13        /* Colour of help window text. */
#define COLOUR_INITBG    14        /* Colour of app bg during initialisation. */
#define COLOUR_INITTEXT  15        /* Colour of app text during initialisation. */

/* _NWGSTYLE_H */
#endif
