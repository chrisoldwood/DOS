#==============================================================================
# (C) Chris Wood 1995.
#
# Makefile for SENDMSG.EXE:- A NetWare message sender.
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
TARGET=sendmsg
.top: $(TARGET).exe

#
# The target components.
#
OBJFILES=sendmsg.obj nwerrs.obj fs_menu.obj fileserv.obj usgpmenu.obj usermenu.obj \
grpmenu.obj users.obj groups.obj msg.obj

#
# The components' dependencies.
#
sendmsg.obj:  sendmsg.c fileserv.h helptext.h nwerrs.h 
nwerrs.obj:   nwerrs.c
fs_menu.obj:  fs_menu.c fileserv.h helpids.h
fileserv.obj: fileserv.c fileserv.h
usgpmenu.obj: usgpmenu.c helpids.h
usermenu.obj: usermenu.c helpids.h fileserv.h users.h
grpmenu.obj:  grpmenu.c helpids.h fileserv.h groups.h
users.obj:    users.c users.h msg.h
groups.obj:   groups.c groups.h msg.h
msg.obj:      msg.c msg.h

#
# The NetWare libraries.
#
NWINC=..\include
NWLIB=..\lib

#
# My NetWare graphics libraray.
#
NWGFXINC=..\nwgfxlib
NWGFXLIB=..\nwgfxlib

#
# This application.
#
THISINC=.
THISLIB=.

#
# Set up compiler paths locally.
#
.INIT:
     %setenv include=$(THISINC);$(NWGFXINC);$(NWINC);$(INCLUDE)
     %setenv lib=$(THISLIB);$(NWGFXLIB);$(NWLIB);$(LIB)

#
# Set up basic compiler directives.
# -c      :- Compile only.
# -nologo :- Don't display MS logo.
# -AM/L   :- Use Medium/Large memory model.
# -W4     :- Warning level 4 (all errors).
# -Zp     :- Pack data structures to byte boundries.
# -DNWDOS :- Set #define NWDOS for NetWare DOS compile.
#
CC=cl
LD=link
RM=del
CCFLAGS=-c -nologo -AL -W4 -Zp -DNWDOS

#
# Set up build dependent directives.
# -Ge     :- Do include stack checking.
# -Gs     :- Don't include stack checking.
# -Od     :- No optimisation.
# -Ox     :- Maximum optimisation.
# -Zi     :- Include debugging code.
# /nologo :- Don't display MS logo.
# /noi    :- Ignore case
# /stack: :- Make stack 0x4000 bytes. (16K)
# /onerror:- Don't generate .exe if error occurred.
# /co     :- Include CodeView info.
#
%if %defined(NONDEBUG)
# Optimised build without debug info.
DBFLAGS=-DNONDEBUG -Gs -Ox
LDFLAGS=/nologo /noi /stack:0x4000 /onerror:noexe

%else
%if %defined(OPTDEBUG)
# Optimised build with debug info.
DBFLAGS=-DDEBUG -Gs -Ox
LDFLAGS=/nologo /noi /stack:0x4000 /onerror:noexe

%else
# Unoptimised build with debug info.
DBFLAGS=-DDEBUG -Ge -Od -Zi
LDFLAGS=/nologo /noi /stack:0x4000 /onerror:noexe /co
%end

%end

#
# General compilation rule.
#
.c.obj:
     $(CC) $(CCFLAGS) $(DBFLAGS) $< >$[f,,$@,err]

#
# Linking rule.
#
$(TARGET).exe: $(OBJFILES) ..\nwgfxlib\nwgfx.lib
     $(LD) $(LDFLAGS) <@<
sendmsg.obj +
nwerrs.obj +
fs_menu.obj +
fileserv.obj +
usgpmenu.obj +
usermenu.obj +
grpmenu.obj +
users.obj +
groups.obj +
msg.obj
$(TARGET).exe
$(TAGET).map /m
nwgfx.lib +
nwcalls.lib
;
<

#
# Clean up unecessary junk.
#
clean:
     $(RM) *.bak
     $(RM) *.err
     $(RM) *.obj
     $(RM) *.map
