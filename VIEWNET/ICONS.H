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
NWGSTR UnknownIcon[ICON_HEIGHT] = {               "�����Ŀ", 
                                                  "�     �",
                                                  "�  ?  �",
                                                  "�     �",
                                                  "�������",
                                                  "   �   "
                                  };

/* Workstation. */
NWGSTR WorkStationIcon[ICON_HEIGHT] = {           " ���Ŀ ", 
                                                  " �   � ",
                                                  " ����� ",
                                                  "�����Ŀ",
                                                  "�������",
                                                  "   �   "
                                      };

/* Dedicated File server. */
NWGSTR DedServerIcon[ICON_HEIGHT] = {             "��Ŀ   ",
                                                  "��ĳ   ",
                                                  "��ĳ�Ŀ",
                                                  "��ĳ���",
                                                  "�������",
                                                  "   �   "
                                    };

/* Dedciated File server & Bridge. */
NWGSTR DedServerBridgeIcon[ICON_HEIGHT] = {       "��Ŀ   ",
                                                  "��ĳ   ",
                                                  "��ĳ�Ŀ",
                                                  "��ĳ���",
                                                  "�������",
                                                  " ���ͼ "
                                        };

/* Non-Dedciated File server. */
NWGSTR NonDedServerIcon[ICON_HEIGHT] = {          "��Ŀ   ",
                                                  "��ĳ   ",
                                                  "��ĳ�Ŀ",
                                                  "��ĳ���",
                                                  "�������",
                                                  "   �   "
                                       };

/* Bridge. */
NWGSTR DedBridgeIcon[ICON_HEIGHT] = {             "       ", 
                                                  "       ",
                                                  "       ",
                                                  "�����Ŀ",
                                                  "�������",
                                                  " ���ͼ "
                                    };

/* Non-Dedciated Bridge. */
NWGSTR NonDedBridgeIcon[ICON_HEIGHT] = {          "��Ŀ   ",
                                                  "��ĳ   ",
                                                  "��ĳ�Ŀ",
                                                  "��ĳ���",
                                                  "�������",
                                                  " ���ͼ "
                                       };

/* Non-Dedciated File server & Bridge. */
NWGSTR NonDedServerBridgeIcon[ICON_HEIGHT] = {    "��Ŀ   ",
                                                  "��ĳ   ",
                                                  "��ĳ�Ŀ",
                                                  "��ĳ���",
                                                  "�������",
                                                  " ���ͼ "
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
