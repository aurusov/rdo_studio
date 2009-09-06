# Microsoft Developer Studio Project File - Name="rp_ctrl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=rp_ctrl - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rp_ctrl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rp_ctrl.mak" CFG="rp_ctrl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rp_ctrl - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "rp_ctrl - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "rp_ctrl - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "$(SolutionDir)$(ConfigurationName)"
# PROP BASE Intermediate_Dir "$(ConfigurationName)"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /MD /W3 /GX /ZI /Od /I "../rp_misc" /D "_CRT_SECURE_NO_DEPRECATE" /D "_AFXDLL" /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I "../rp_misc" /D "_CRT_SECURE_NO_DEPRECATE" /D "_AFXDLL" /GZ /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Debug\rp_ctrl.lib"

!ELSEIF  "$(CFG)" == "rp_ctrl - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "$(SolutionDir)$(ConfigurationName)"
# PROP BASE Intermediate_Dir "$(ConfigurationName)"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /GX /I "../rp_misc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_CRT_SECURE_NO_DEPRECATE" /D "_MBCS" /c
# ADD CPP /nologo /MT /GX /I "../rp_misc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Release\rp_ctrl.lib"

!ENDIF 

# Begin Target

# Name "rp_ctrl - Win32 Debug"
# Name "rp_ctrl - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cc;cxx;def;odl;idl;hpj;bat;asm;asmx"
# Begin Source File

SOURCE=.\rdoprocess_toolbar.cpp
DEP_CPP_RDOPR=\
	"..\rp_misc\rdoprocess_string.h"\
	".\rdoprocess_toolbar.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "rp_ctrl - Win32 Debug"

# ADD CPP /nologo /GX /Yu"stdafx.h" /GZ

!ELSEIF  "$(CFG)" == "rp_ctrl - Win32 Release"

# ADD CPP /nologo /GX /Yu"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
DEP_CPP_STDAF=\
	"..\rp_misc\rdoprocess_string.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "rp_ctrl - Win32 Debug"

# ADD CPP /nologo /GX /Yc"stdafx.h" /GZ

!ELSEIF  "$(CFG)" == "rp_ctrl - Win32 Release"

# ADD CPP /nologo /GX /Yc"stdafx.h"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;inc;xsd"
# Begin Source File

SOURCE=.\rdoprocess_toolbar.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# End Group
# End Target
# End Project
