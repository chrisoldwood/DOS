# Microsoft Visual C++ generated build script - Do not modify

PROJ = MTOSSUDG
DEBUG = 0
PROGTYPE = 6
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = C:\DEV\MTOSSUDG\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = CFGFILE.C   
FIRSTCPP =             
RC = rc
CFLAGS_D_DEXE = /nologo /G2 /W4 /WX /Gf /Zi /AM /Od /D "_DEBUG" /D "_DOS" /FR /Zn /Fd"MTOSSUDG.PDB"
CFLAGS_R_DEXE = /nologo /Gs /G2 /W4 /WX /Gf /AM /O2 /Ox /D "NDEBUG" /D "_DOS" 
LFLAGS_D_DEXE = /NOLOGO /NOI /STACK:5120 /ONERROR:NOEXE /CO 
LFLAGS_R_DEXE = /NOLOGO /NOI /STACK:5120 /ONERROR:NOEXE 
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
SBRS = CFGFILE.SBR \
		DEBUG.SBR \
		FILEIO.SBR \
		MTOSSUDG.SBR \
		PARSE.SBR


CFGFILE_DEP = c:\dev\mtossudg\mtossudg.h \
	c:\dev\mtossudg\debug.h \
	c:\dev\mtossudg\fileio.h


DEBUG_DEP = c:\dev\mtossudg\mtossudg.h \
	c:\dev\mtossudg\debug.h


FILEIO_DEP = c:\dev\mtossudg\mtossudg.h \
	c:\dev\mtossudg\debug.h \
	c:\dev\mtossudg\fileio.h


MTOSSUDG_DEP = c:\dev\mtossudg\mtossudg.h \
	c:\dev\mtossudg\debug.h \
	c:\dev\mtossudg\fileio.h


PARSE_DEP = c:\dev\mtossudg\mtossudg.h \
	c:\dev\mtossudg\debug.h \
	c:\dev\mtossudg\fileio.h


all:	$(PROJ).EXE

CFGFILE.OBJ:	CFGFILE.C $(CFGFILE_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c CFGFILE.C

DEBUG.OBJ:	DEBUG.C $(DEBUG_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c DEBUG.C

FILEIO.OBJ:	FILEIO.C $(FILEIO_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c FILEIO.C

MTOSSUDG.OBJ:	MTOSSUDG.C $(MTOSSUDG_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c MTOSSUDG.C

PARSE.OBJ:	PARSE.C $(PARSE_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c PARSE.C

$(PROJ).EXE::	CFGFILE.OBJ DEBUG.OBJ FILEIO.OBJ MTOSSUDG.OBJ PARSE.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
CFGFILE.OBJ +
DEBUG.OBJ +
FILEIO.OBJ +
MTOSSUDG.OBJ +
PARSE.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
s:\public\vc151\lib\+
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
