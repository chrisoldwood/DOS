/******************************************************************************
** (C) Chris Wood 1995
**
** ICONS.H - Node icons. (These are arrays of extended characters).
**
******************************************************************************* 
*/

/* Icon dimensions. */
#define ICON_WIDTH  7
#define ICON_HEIGHT 6

/* Unknown. */
NWGSTR UnknownIcon[ICON_HEIGHT] = {               "зддддд©", 
                                                  "Ё     Ё",
                                                  "Ё  ?  Ё",
                                                  "Ё     Ё",
                                                  "юддддды",
                                                  "   ╨   "
                                  };

/* Workstation. */
NWGSTR WorkStationIcon[ICON_HEIGHT] = {           " зддд© ", 
                                                  " Ё   Ё ",
                                                  " юбдбы ",
                                                  "здадад©",
                                                  "юддддды",
                                                  "   ╨   "
                                      };

/* Dedicated File server. */
NWGSTR DedServerIcon[ICON_HEIGHT] = {             "здд©   ",
                                                  "ЁддЁ   ",
                                                  "ЁддЁзд©",
                                                  "ЁддЁюбы",
                                                  "юддыдад",
                                                  "   ╨   "
                                    };

/* Dedciated File server & Bridge. */
NWGSTR DedServerBridgeIcon[ICON_HEIGHT] = {       "здд©   ",
                                                  "ЁддЁ   ",
                                                  "ЁддЁзд©",
                                                  "ЁддЁюбы",
                                                  "юддыдад",
                                                  " хмнм╪ "
                                        };

/* Non-Dedciated File server. */
NWGSTR NonDedServerIcon[ICON_HEIGHT] = {          "здд©   ",
                                                  "ЁддЁ   ",
                                                  "ЁддЁзд©",
                                                  "ЁддЁюбы",
                                                  "юддыдад",
                                                  "   ╨   "
                                       };

/* Bridge. */
NWGSTR DedBridgeIcon[ICON_HEIGHT] = {             "       ", 
                                                  "       ",
                                                  "       ",
                                                  "зддддд©",
                                                  "юддддды",
                                                  " хмнм╪ "
                                    };

/* Non-Dedciated Bridge. */
NWGSTR NonDedBridgeIcon[ICON_HEIGHT] = {          "здд©   ",
                                                  "ЁддЁ   ",
                                                  "ЁддЁзд©",
                                                  "ЁддЁюбы",
                                                  "юддыдад",
                                                  " хмнм╪ "
                                       };

/* Non-Dedciated File server & Bridge. */
NWGSTR NonDedServerBridgeIcon[ICON_HEIGHT] = {    "здд©   ",
                                                  "ЁддЁ   ",
                                                  "ЁддЁзд©",
                                                  "ЁддЁюбы",
                                                  "юддыдад",
                                                  " хмнм╪ "
                                             };

/* List of icons. */
NWGSTR * Icons[8] = { UnknownIcon,                /* NODE_UNKNOWN. */
                      WorkStationIcon,            /* NODE_WORKSTATION. */
                      DedServerIcon,              /* NODE_FILESERVER. */
                      DedServerBridgeIcon,        /* NODE_FILESERVERBRIDGE. */
                      NonDedServerIcon,           /* NODE_NONDEDFILESERVER. */
                      DedBridgeIcon,              /* NODE_BRIDGE. */
                      NonDedBridgeIcon,           /* NODE_NONDEDBRIDGE. */
                      NonDedServerBridgeIcon,     /* NODE_NONDEDSERVBRIDGE. */
                    };    
