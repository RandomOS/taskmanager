# Microsoft Developer Studio Generated NMAKE File, Based on TaskManager.dsp
!IF "$(CFG)" == ""
CFG=TaskManager - Win32 Release
!MESSAGE No configuration specified. Defaulting to TaskManager - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "TaskManager - Win32 Release" && "$(CFG)" != "TaskManager - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TaskManager.mak" CFG="TaskManager - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TaskManager - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TaskManager - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TaskManager - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\TaskManager.exe"


CLEAN :
	-@erase "$(INTDIR)\auxfuction.obj"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\dialogs.res"
	-@erase "$(INTDIR)\listview.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\oncommand.obj"
	-@erase "$(INTDIR)\process.obj"
	-@erase "$(INTDIR)\statusbar.obj"
	-@erase "$(INTDIR)\uifunction.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\TaskManager.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "_WINDOWS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\dialogs.res"
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib advapi32.lib shell32.lib comctl32.lib psapi.lib shlwapi.lib uxtheme.lib /nologo /subsystem:windows /pdb:none /machine:I386 /out:"$(OUTDIR)\TaskManager.exe" 
LINK32_OBJS= \
	"$(INTDIR)\auxfuction.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\listview.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\oncommand.obj" \
	"$(INTDIR)\process.obj" \
	"$(INTDIR)\statusbar.obj" \
	"$(INTDIR)\uifunction.obj" \
	"$(INTDIR)\dialogs.res"

"$(OUTDIR)\TaskManager.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TaskManager - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\TaskManager.exe"


CLEAN :
	-@erase "$(INTDIR)\auxfuction.obj"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\dialogs.res"
	-@erase "$(INTDIR)\listview.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\oncommand.obj"
	-@erase "$(INTDIR)\process.obj"
	-@erase "$(INTDIR)\statusbar.obj"
	-@erase "$(INTDIR)\uifunction.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\TaskManager.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\dialogs.res" /d "_DEBUG" 
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib advapi32.lib shell32.lib comctl32.lib psapi.lib shlwapi.lib uxtheme.lib /nologo /subsystem:windows /pdb:none /debug /machine:I386 /out:"$(OUTDIR)\TaskManager.exe" 
LINK32_OBJS= \
	"$(INTDIR)\auxfuction.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\listview.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\oncommand.obj" \
	"$(INTDIR)\process.obj" \
	"$(INTDIR)\statusbar.obj" \
	"$(INTDIR)\uifunction.obj" \
	"$(INTDIR)\dialogs.res"

"$(OUTDIR)\TaskManager.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("TaskManager.dep")
!INCLUDE "TaskManager.dep"
!ELSE 
!MESSAGE Warning: cannot find "TaskManager.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "TaskManager - Win32 Release" || "$(CFG)" == "TaskManager - Win32 Debug"
SOURCE=.\src\auxfuction.c

"$(INTDIR)\auxfuction.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\src\debug.c

"$(INTDIR)\debug.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\src\listview.c

"$(INTDIR)\listview.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\src\main.c

"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\src\oncommand.c

"$(INTDIR)\oncommand.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\src\process.c

"$(INTDIR)\process.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\src\statusbar.c

"$(INTDIR)\statusbar.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\src\uifunction.c

"$(INTDIR)\uifunction.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=.\dialogs.rc

"$(INTDIR)\dialogs.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)

!ENDIF 
