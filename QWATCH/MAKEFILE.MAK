#==============================================================================
# (C) Chris Wood 1994/95.
#
# Makefile for QWATCH.EXE:- A NetWare print queue monitor.
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
TARGET=qwatch
.top: $(TARGET).exe

#
# The target components.
#
OBJFILES=qwatch.obj nwerrs.obj fs_menu.obj pq_menu.obj pj_menu.obj pj_dlg.obj \
ps_menu.obj ps_dlg.obj fileserv.obj prntserv.obj prtqueue.obj printjob.obj \
prtforms.obj

#
# The components' dependencies.
#
qwatch.obj:   qwatch.c  fileserv.h prtqueue.h printjob.h prntserv.h helptext.h nwerrs.h 
nwerrs.obj:   nwerrs.c

fs_menu.obj:  fs_menu.c fileserv.h helpids.h
pq_menu.obj:  pq_menu.c fileserv.h prtqueue.h printjob.h helpids.h
pj_menu.obj:  pj_menu.c fileserv.h prtqueue.h printjob.h nwerrs.h helpids.h
pj_dlg.obj:   pj_dlg.c  fileserv.h prtqueue.h printjob.h nwerrs.h pj_dlg.h helpids.h
ps_menu.obj:  ps_menu.c fileserv.h prtqueue.h printjob.h nwerrs.h helpids.h
ps_dlg.obj:   ps_dlg.c  ps_dlg.h

fileserv.obj: fileserv.c fileserv.h
prntserv.obj: prntserv.c prntserv.h fileserv.h
prtqueue.obj: prtqueue.c prtqueue.h printjob.h prntserv.h
printjob.obj: printjob.c printjob.h prtqueue.h prntserv.h
prtforms.obj: prtforms.c prtforms.h prtqueue.h printjob.h prntserv.h

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
qwatch.obj +
nwerrs.obj +
fs_menu.obj +
pq_menu.obj +
pj_menu.obj +
pj_dlg.obj +
ps_menu.obj +
ps_dlg.obj +
fileserv.obj +
prntserv.obj +
prtqueue.obj +
printjob.obj +
prtforms.obj
qwatch.exe
qwatch.map /m
nwgfx.lib +
nwcalls.lib +
nwpsrv.lib +
nwnet.lib +
nwlocale.lib +
lnwipxspx.lib
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
