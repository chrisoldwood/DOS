#==============================================================================
# (C) Chris Wood 1995.
#
# Makefile for VIEWNET.EXE:- A NetWare network viewer.
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
TARGET=viewnet
.top: $(TARGET).exe

#
# The target components.
#
OBJFILES=$(TARGET).obj nwerrs.obj scanmenu.obj shownet.obj ipxcode.obj fileio.obj\
         wsdlg.obj brdgmenu.obj

#
# The components' dependencies.
#
$(TARGET).obj: $(TARGET).c helptext.h nwerrs.h 
nwerrs.obj:    nwerrs.c
scanmenu.obj:  scanmenu.c helpids.h
shownet.obj:   shownet.c helpids.h icons.h nodes.h ipxcode.h
ipxcode.obj:   ipxcode.c nodes.h ipxcode.h
fileio.obj:    fileio.c
wsdlg.obj:     wsdlg.c wsdlg.h helpids.h nodes.h
brdgmenu.obj:  brdgmenu.c helpids.h nodes.h

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
# Set up assembly directives.
# /mx     :- Leave symbols as mixed upper & lower case.
ASM=masm
ASMFLAGS=/mx

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
# General assembly rule.
#
.asm.obj:
    $(ASM) $(ASMFLAGS) $< ; >$[f,,$@,err]

#
# Linking rule.
#
$(TARGET).exe: $(OBJFILES) ..\nwgfxlib\nwgfx.lib
     $(LD) $(LDFLAGS) <@<
$(TARGET).obj +
nwerrs.obj +
scanmenu.obj +
shownet.obj +
ipxcode.obj +
fileio.obj +
wsdlg.obj +
brdgmenu.obj
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
