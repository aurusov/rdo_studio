# Microsoft Developer Studio Project File - Name="rdo_runtime" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=rdo_runtime - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rdo_runtime.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rdo_runtime.mak" CFG="rdo_runtime - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rdo_runtime - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "rdo_runtime - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "rdo_runtime - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /Ob2 /I "." /I "../rdo_parser" /I "../../rdo_common" /D "_LIB" /D "WIN32" /D "NDEBUG" /D "_MBCS" /Yu"pch.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "rdo_runtime - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GR /GX /ZI /Od /I "." /I "../rdo_parser" /I "../../rdo_common" /D "RDOSIM_COMPATIBLE" /D "_LIB" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_AFXDLL" /Fr /Yu"pch.h" /FD /GZ /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "rdo_runtime - Win32 Release"
# Name "rdo_runtime - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ietrace.cpp

!IF  "$(CFG)" == "rdo_runtime - Win32 Release"

# ADD CPP /GR /Yu"pch.h"

!ELSEIF  "$(CFG)" == "rdo_runtime - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\operationtrace.cpp

!IF  "$(CFG)" == "rdo_runtime - Win32 Release"

# ADD CPP /GR /Yu"pch.h"

!ELSEIF  "$(CFG)" == "rdo_runtime - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\pch.cpp

!IF  "$(CFG)" == "rdo_runtime - Win32 Release"

# ADD CPP /GR /Yc"pch.h"

!ELSEIF  "$(CFG)" == "rdo_runtime - Win32 Debug"

# ADD CPP /Yc"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdo.cpp

!IF  "$(CFG)" == "rdo_runtime - Win32 Release"

# ADD CPP /GR /Yu"pch.h"

!ELSEIF  "$(CFG)" == "rdo_runtime - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdo_activity.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_pattern.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_runtime.cpp
# End Source File
# Begin Source File

SOURCE=.\rdobase.cpp

!IF  "$(CFG)" == "rdo_runtime - Win32 Release"

# ADD CPP /GR /Yu"pch.h"

!ELSEIF  "$(CFG)" == "rdo_runtime - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdocalc.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoframe.cpp

!IF  "$(CFG)" == "rdo_runtime - Win32 Release"

# ADD CPP /O2

!ELSEIF  "$(CFG)" == "rdo_runtime - Win32 Debug"

# ADD CPP /Ze

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdopokaz.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoprocess.cpp

!IF  "$(CFG)" == "rdo_runtime - Win32 Release"

# ADD CPP /GR /Yu"pch.h"

!ELSEIF  "$(CFG)" == "rdo_runtime - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoruntime_object.cpp
# End Source File
# Begin Source File

SOURCE=.\rdotrace.cpp

!IF  "$(CFG)" == "rdo_runtime - Win32 Release"

# ADD CPP /GR /Yu"pch.h"

!ELSEIF  "$(CFG)" == "rdo_runtime - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ruletrace.cpp

!IF  "$(CFG)" == "rdo_runtime - Win32 Release"

# ADD CPP /GR /Yu"pch.h"

!ELSEIF  "$(CFG)" == "rdo_runtime - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\searchtrace.cpp

!IF  "$(CFG)" == "rdo_runtime - Win32 Release"

# ADD CPP /GR /Yu"pch.h"

!ELSEIF  "$(CFG)" == "rdo_runtime - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\searchtree.cpp

!IF  "$(CFG)" == "rdo_runtime - Win32 Release"

# ADD CPP /GR /Yu"pch.h"

!ELSEIF  "$(CFG)" == "rdo_runtime - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\simtrace.cpp

!IF  "$(CFG)" == "rdo_runtime - Win32 Release"

# ADD CPP /GR /Yu"pch.h"

!ELSEIF  "$(CFG)" == "rdo_runtime - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ietrace.h
# End Source File
# Begin Source File

SOURCE=.\memcheck.h
# End Source File
# Begin Source File

SOURCE=.\operationtrace.h
# End Source File
# Begin Source File

SOURCE=.\pch.h
# End Source File
# Begin Source File

SOURCE=.\rdo.h
# End Source File
# Begin Source File

SOURCE=.\rdo_activity.h
# End Source File
# Begin Source File

SOURCE=.\rdo_pattern.h
# End Source File
# Begin Source File

SOURCE=.\rdo_random_distribution.h
# End Source File
# Begin Source File

SOURCE=.\rdo_runtime.h
# End Source File
# Begin Source File

SOURCE=.\rdobase.h
# End Source File
# Begin Source File

SOURCE=.\rdocalc.h
# End Source File
# Begin Source File

SOURCE=.\rdodptrtime.h
# End Source File
# Begin Source File

SOURCE=.\rdoframe.h
# End Source File
# Begin Source File

SOURCE=.\rdopokaz.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess.h
# End Source File
# Begin Source File

SOURCE=.\rdoruntime_object.h
# End Source File
# Begin Source File

SOURCE=.\rdotrace.h
# End Source File
# Begin Source File

SOURCE=.\ruletrace.h
# End Source File
# Begin Source File

SOURCE=.\searchtrace.h
# End Source File
# Begin Source File

SOURCE=.\searchtree.h
# End Source File
# Begin Source File

SOURCE=.\simtrace.h
# End Source File
# End Group
# End Target
# End Project
