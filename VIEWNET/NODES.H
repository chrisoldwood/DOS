/******************************************************************************
** (C) Chris Wood 1995
**
** NODES.H - Nodes structures and definitions.
**
******************************************************************************* 
*/

/* Node types. */
#define NODE_UNKNOWN          0
#define NODE_WORKSTATION      1
#define NODE_FILESERVER       2
#define NODE_FILESERVERBRIDGE 3
#define NODE_NONDEDFILESERVER 4
#define NODE_BRIDGE           5
#define NODE_NONDEDBRIDGE     6
#define NODE_NONDEDSERVBRIDGE 7

/* Component flags. */
#define COMPONENT_IPXSPX           0x00000001
#define COMPONENT_BRIDGEDRIVER     0x00000002
#define COMPONENT_SHELLDRIVER      0x00000004
#define COMPONENT_SHELL            0x00000008
#define COMPONENT_VAPSHELL         0x00000010
#define COMPONENT_BRIDGE           0x00000020
#define COMPONENT_FILESERVER       0x00000040
#define COMPONENT_IPXSPX_NONDED    0x00000080
#define COMPONENT_IPXONLY          0x00000100

/* Node structure. */
typedef struct tagNetNode
{
     int                 iType;         /* Node type. */
     unsigned long       lFlags;        /* Node components. */
     BYTE                Address[6];    /* Node address. */
     BYTE                Network[4];    /* Nodes' network. */
     BYTE                Socket[2];     /* Nodes' diagnostic socket. */
     int                 iNumComps;     /* Number of components. */
     char                szName[10];    /* Node "name". */
     struct tagNetNode   *Prev;         /* Previous node in list. */
     struct tagNetNode   *Next;         /* Next node in list. */
} NETNODE, * PNETNODE, *NODELIST;

/* Pointer to no node. */
#define NONE   (PNETNODE) 0
