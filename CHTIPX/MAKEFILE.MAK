#==============================================================================
# (C) Chris Wood 1995.
#
# Makefile for CHATIPX.EXE:- A NetWare IPX chat for two people.
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
TARGET=chatipx
.top: $(TARGET).exe

#
# The target components.
#
OBJFILES=chatipx.obj nwerrs.obj callmenu.obj usermenu.obj users.obj callinit.obj \
talk.obj ipxcode.obj

#
# The components' dependencies.
#
chatipx.obj:  chatipx.c helptext.h nwerrs.h 
nwerrs.obj:   nwerrs.c
callmenu.obj: callmenu.c helpids.h
usermenu.obj: usermenu.c helpids.h users.h
users.obj:    users.c users.h
callinit.obj: callinit.c ipxcode.h
talk.obj:     talk.c ipxcode.h
ipxcode.obj:  ipxcode.h

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
chatipx.obj +
nwerrs.obj +
callmenu.obj +
users.obj +
usermenu.obj +
callinit.obj +
talk.obj +
ipxcode.obj
$(TARGET).exe
$(TAGET).map /m
nwgfx.lib +
nwcalls.lib +
lnwipxsp.lib
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
