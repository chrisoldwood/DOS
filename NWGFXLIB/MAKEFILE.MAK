#==============================================================================
# (C) Chris Wood 1995.
#
# Makefile for the NetWare graphics library.
# 
# This makefile is designed for Polytron smake and the Microsoft compiler.
#
# This builds a DEBUG build by default, use:-
# smake NONDEBUG=TRUE for an optimised build without debug info.
# smake OPTDEBUG=TRUE for an optimised build with debug info.
#==============================================================================

#
# The target.
#
TARGET=nwgfx
.top: $(TARGET).lib

#
# The target components and their dependencies.
#
OBJFILES=nwginit.obj nwgcore.obj nwgframe.obj nwgkeybd.obj nwghelp.obj\
         nwgmbox.obj nwgmenu.obj nwgdlg.obj nwgdebug.obj

nwginit.obj:  nwginit.c nwgtypes.h nwgpriv.h nwghelp.h nwgmenu.h nwgstyle.h\
              nwgcodes.h nwgerr.h nwgproto.h
nwgcore.obj:  nwgcore.c nwgtypes.h nwgpriv.h nwgstyle.h nwgcodes.h nwgproto.h
nwgframe.obj: nwgframe.c nwgtypes.h nwgframe.h nwgstyle.h nwgcodes.h nwgproto.h
nwgkeybd.obj: nwgkeybd.c nwgtypes.h nwgmenu.h  nwgcodes.h nwgproto.h
nwghelp.obj:  nwghelp.c nwgtypes.h nwghelp.h nwgstyle.h nwgcodes.h nwgproto.h
nwgmbox.obj:  nwgmbox.c nwgtypes.h nwgstyle.h nwgcodes.h nwgproto.h
nwgmenu.obj:  nwgmenu.c nwgtypes.h nwgmenu.h nwgstyle.h nwgcodes.h nwgproto.h
nwgdlg.obj:   nwgdlg.c nwgtypes.h nwgdlg.h nwgstyle.h nwgcodes.h nwgmenu.h nwgproto.h
nwgdebug.obj: nwgdebug.c nwgtypes.h nwgproto.h

#
# Set up basic compiler directives.
# -c      :- Compile only.
# -nologo :- Don't display MS logo.
# -AM/L   :- Use Medium/Large memory model.
# -W4     :- Warning level 4 (all errors).
# -Zp     :- Pack data structures to byte boundries.
#
CC=cl
RM=del
MLIB=lib
CCFLAGS=-c -nologo -AL -W4 -Zp

#
# Set up build dependent directives.
# -Ge     :- Do include stack checking.
# -Gs     :- Don't include stack checking.
# -Od     :- No optimisation.
# -Ox     :- Maximum optimisation.
# -Zi     :- Include debugging code.
#
%if %defined(NONDEBUG)
# Optimised build without debug info.
DBFLAGS=-DNONDEBUG -Gs -Ox

%else
%if %defined(OPTDEBUG)
# Optimised build with debug info.
DBFLAGS=-DDEBUG -Gs -Ox -Zi

%else
# Unoptimised build with debug info.
DBFLAGS=-DDEBUG -Ge -Od -Zi

%end
%end

#
# General compilation rule.
#
.c.obj:
     $(CC) $(CCFLAGS) $(DBFLAGS) $< >$[f,,$@,err]

#
# Library building rule.
#
$(TARGET).lib: $(OBJFILES)
     $(RM) $(TARGET).lib
     $(MLIB) <@<
$(TARGET).lib
y
+nwginit.obj &
+nwgcore.obj &
+nwgframe.obj &
+nwgkeybd.obj &
+nwghelp.obj &
+nwgmbox.obj &
+nwgmenu.obj &
+nwgdlg.obj &
+nwgdebug.obj
;
<

#
# Clean up unecessary junk.
#
clean:
     $(RM) *.bak
     $(RM) *.err
     $(RM) *.obj
