# Microsoft Visual C++ generated build script - Do not modify

PROJ = MTMGFX
DEBUG = 1
PROGTYPE = 6
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = K:\PROJECTS\DOS\TMLIBS\TMGFXLIB\MTMGFX\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = TMGASSRT.C  
FIRSTCPP =             
RC = rc
CFLAGS_D_DEXE = /nologo /W4 /WX /Zi /AM /YX"TMGFX.H" /Od /D "_DEBUG" /D "_DOS" /FR /Zn /Fd"MTMGFX.PDB" /Fp"MTMGFX.PCH"
CFLAGS_R_DEXE = /nologo /Gs /W4 /WX /AM /YX"TMGFX.H" /Ox /D "NDEBUG" /D "_DOS" /FR /Zn /Fp"MTMGFX.PCH"
LFLAGS_D_DEXE = /NOLOGO /ONERROR:NOEXE /NOI /CO /STACK:5120
LFLAGS_R_DEXE = /NOLOGO /ONERROR:NOEXE /NOI /STACK:5120
LIBS_D_DEXE = oldnames mlibce
LIBS_R_DEXE = oldnames mlibce
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_DEXE)
LFLAGS = $(LFLAGS_D_DEXE)
LIBS = $(LIBS_D_DEXE)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_DEXE)
LFLAGS = $(LFLAGS_R_DEXE)
LIBS = $(LIBS_R_DEXE)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = CLIPRECT.SBR \
		CURSOR.SBR \
		DISPLAY.SBR \
		FRAME.SBR \
		GFXASSRT.SBR \
		LINE.SBR \
		MAIN.SBR \
		PIXEL.SBR \
		RECT.SBR \
		TEXT.SBR


CLIPRECT_DEP = k:\projects\dos\tmlibs\tmgfxlib\libsrc\tmgfx.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\pragmas.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\gfxtypes.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\gfxassrt.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\point.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\cliprect.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\display.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\cursor.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\pixel.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\line.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\rect.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\frame.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\text.h


CURSOR_DEP = k:\projects\dos\tmlibs\tmgfxlib\libsrc\tmgfx.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\pragmas.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\gfxtypes.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\gfxassrt.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\point.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\cliprect.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\display.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\cursor.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\pixel.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\line.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\rect.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\frame.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\text.h


DISPLAY_DEP = k:\projects\dos\tmlibs\tmgfxlib\libsrc\tmgfx.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\pragmas.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\gfxtypes.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\gfxassrt.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\point.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\cliprect.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\display.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\cursor.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\pixel.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\line.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\rect.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\frame.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\text.h


FRAME_DEP = k:\projects\dos\tmlibs\tmgfxlib\libsrc\tmgfx.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\pragmas.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\gfxtypes.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\gfxassrt.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\point.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\cliprect.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\display.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\cursor.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\pixel.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\line.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\rect.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\frame.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\text.h


GFXASSRT_DEP = k:\projects\dos\tmlibs\tmgfxlib\libsrc\tmgfx.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\pragmas.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\gfxtypes.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\gfxassrt.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\point.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\cliprect.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\display.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\cursor.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\pixel.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\line.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\rect.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\frame.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\text.h


LINE_DEP = k:\projects\dos\tmlibs\tmgfxlib\libsrc\tmgfx.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\pragmas.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\gfxtypes.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\gfxassrt.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\point.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\cliprect.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\display.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\cursor.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\pixel.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\line.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\rect.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\frame.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\text.h


MAIN_DEP = k:\projects\dos\tmlibs\tmgfxlib\libsrc\tmgfx.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\pragmas.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\gfxtypes.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\gfxassrt.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\point.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\cliprect.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\display.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\cursor.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\pixel.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\line.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\rect.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\frame.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\text.h


PIXEL_DEP = k:\projects\dos\tmlibs\tmgfxlib\libsrc\tmgfx.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\pragmas.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\gfxtypes.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\gfxassrt.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\point.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\cliprect.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\display.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\cursor.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\pixel.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\line.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\rect.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\frame.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\text.h


RECT_DEP = k:\projects\dos\tmlibs\tmgfxlib\libsrc\tmgfx.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\pragmas.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\gfxtypes.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\gfxassrt.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\point.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\cliprect.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\display.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\cursor.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\pixel.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\line.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\rect.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\frame.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\text.h


TEXT_DEP = k:\projects\dos\tmlibs\tmgfxlib\libsrc\tmgfx.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\pragmas.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\gfxtypes.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\gfxassrt.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\point.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\cliprect.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\display.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\cursor.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\pixel.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\line.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\rect.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\frame.h \
	k:\projects\dos\tmlibs\tmgfxlib\libsrc\text.h


all:	$(PROJ).EXE $(PROJ).BSC

CLIPRECT.OBJ:	..\LIBSRC\CLIPRECT.C $(CLIPRECT_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIBSRC\CLIPRECT.C

CURSOR.OBJ:	..\LIBSRC\CURSOR.C $(CURSOR_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIBSRC\CURSOR.C

DISPLAY.OBJ:	..\LIBSRC\DISPLAY.C $(DISPLAY_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIBSRC\DISPLAY.C

FRAME.OBJ:	..\LIBSRC\FRAME.C $(FRAME_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIBSRC\FRAME.C

GFXASSRT.OBJ:	..\LIBSRC\GFXASSRT.C $(GFXASSRT_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIBSRC\GFXASSRT.C

LINE.OBJ:	..\LIBSRC\LINE.C $(LINE_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIBSRC\LINE.C

MAIN.OBJ:	..\LIBSRC\MAIN.C $(MAIN_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIBSRC\MAIN.C

PIXEL.OBJ:	..\LIBSRC\PIXEL.C $(PIXEL_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIBSRC\PIXEL.C

RECT.OBJ:	..\LIBSRC\RECT.C $(RECT_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIBSRC\RECT.C

TEXT.OBJ:	..\LIBSRC\TEXT.C $(TEXT_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LIBSRC\TEXT.C

$(PROJ).EXE::	CLIPRECT.OBJ CURSOR.OBJ DISPLAY.OBJ FRAME.OBJ GFXASSRT.OBJ LINE.OBJ \
	MAIN.OBJ PIXEL.OBJ RECT.OBJ TEXT.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
CLIPRECT.OBJ +
CURSOR.OBJ +
DISPLAY.OBJ +
FRAME.OBJ +
GFXASSRT.OBJ +
LINE.OBJ +
MAIN.OBJ +
PIXEL.OBJ +
RECT.OBJ +
TEXT.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
d:\msvc15\lib\+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
