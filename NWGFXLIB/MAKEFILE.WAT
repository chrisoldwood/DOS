#==============================================================================
# (C) Chris Wood 1995.
#
# Makefile for the NetWare graphics library.
# 
# This makefile is designed for the Watcom make and compiler.
#
# This builds a DEBUG build by default, use:-
# wmake NONDEBUG=TRUE for an optimised build without debug info.
# wmake OPTDEBUG=TRUE for an optimised build with debug info.
#==============================================================================

#
# The target.
#
TARGET=nwgfx
All: $(TARGET).lib

#
# The target components and their dependencies.
#
OBJFILES=nwginit.obj nwgcore.obj nwgframe.obj nwgkeybd.obj nwghelp.obj &
         nwgmbox.obj nwgmenu.obj nwgdlg.obj nwgdebug.obj

nwginit.obj:  nwginit.c nwgtypes.h nwgpriv.h nwghelp.h nwgmenu.h nwgstyle.h &
              nwgcodes.h nwgerr.h nwgproto.h
nwgcore.obj:  nwgcore.c nwgtypes.h nwgpriv.h nwgstyle.h nwgcodes.h nwgproto.h
nwgframe.obj: nwgframe.c nwgframe.h nwgtypes.h nwgstyle.h nwgcodes.h nwgproto.h
nwgkeybd.obj: nwgkeybd.c nwgtypes.h nwgmenu.h  nwgcodes.h nwgproto.h
nwghelp.obj:  nwghelp.c nwgtypes.h nwghelp.h nwgstyle.h nwgcodes.h nwgproto.h
nwgmbox.obj:  nwgmbox.c nwgtypes.h nwgstyle.h nwgcodes.h nwgproto.h
nwgmenu.obj:  nwgmenu.c nwgtypes.h nwgmenu.h nwgstyle.h nwgcodes.h nwgproto.h
nwgdlg.obj:   nwgdlg.c nwgtypes.h nwgdlg.h nwgstyle.h nwgcodes.h nwgmenu.h nwgproto.h
nwgdebug.obj: nwgdebug.c nwgtypes.h nwgproto.h

#
# Set up basic compiler directives.
# -bt=DOS :- Target is DOS.
# -mm     :- Use Medium memory model.
# -s      :- Don't include stack checking.
# -hw     :- Watcom format debugging info.
# -wx     :- Warning level max (all errors).
# -zp1    :- Pack data structures to byte boundries.
#
CC=wcc
RM=del
MLIB=wlib
CCFLAGS=-bt=DOS -mm -s -hw -wx -zp1

#
# Set up build dependent directives.
# -od     :- No optimisation.
# -oneatx :- Maximum optimisation.
# -d2     :- Include full debugging info.
#
!ifdef NONDEBUG
# Optimised build without debug info.
DBFLAGS=-DNONDEBUG -oneatx

!else ifdef OPTDEBUG
# Optimised build with debug info.
DBFLAGS=-DDEBUG -oneatx -d1

!else
# Unoptimised build with debug info.
DBFLAGS=-DDEBUG -od -d2

!endif

#
# General compilation rule.
#
.c.obj:
     $(CC) $(CCFLAGS) $(DBFLAGS) $< >$^&.err

#
# Library building rule.
#
$(TARGET).lib: $(OBJFILES)
     $(RM) $(TARGET).lib
     $(MLIB) -n $(TARGET).lib @$(TARGET).lbc

#
# Clean up unecessary junk.
#
clean: .SYMBOLIC
     $(RM) *.bak
     $(RM) *.err
     $(RM) *.obj
