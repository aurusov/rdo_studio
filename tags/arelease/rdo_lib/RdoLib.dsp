# Microsoft Developer Studio Project File - Name="RdoLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=RdoLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RdoLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RdoLib.mak" CFG="RdoLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RdoLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "RdoLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RdoLib - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /D "_LIB" /D "WIN32" /D "NDEBUG" /D "_MBCS" /FD /c
# SUBTRACT CPP /Fr /YX
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "RdoLib - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /D "_LIB" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_AFXDLL" /FR /YX /FD /GZ /c
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

# Name "RdoLib - Win32 Release"
# Name "RdoLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\IETrace.cpp

!IF  "$(CFG)" == "RdoLib - Win32 Release"

!ELSEIF  "$(CFG)" == "RdoLib - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\OperationTrace.cpp

!IF  "$(CFG)" == "RdoLib - Win32 Release"

!ELSEIF  "$(CFG)" == "RdoLib - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\pch.cpp

!IF  "$(CFG)" == "RdoLib - Win32 Release"

!ELSEIF  "$(CFG)" == "RdoLib - Win32 Debug"

# ADD CPP /Yc

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Rdo.cpp

!IF  "$(CFG)" == "RdoLib - Win32 Release"

!ELSEIF  "$(CFG)" == "RdoLib - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RdoBase.cpp

!IF  "$(CFG)" == "RdoLib - Win32 Release"

!ELSEIF  "$(CFG)" == "RdoLib - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RdoTrace.cpp

!IF  "$(CFG)" == "RdoLib - Win32 Release"

!ELSEIF  "$(CFG)" == "RdoLib - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RuleTrace.cpp

!IF  "$(CFG)" == "RdoLib - Win32 Release"

!ELSEIF  "$(CFG)" == "RdoLib - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SearchTrace.cpp

!IF  "$(CFG)" == "RdoLib - Win32 Release"

!ELSEIF  "$(CFG)" == "RdoLib - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SearchTree.cpp

!IF  "$(CFG)" == "RdoLib - Win32 Release"

!ELSEIF  "$(CFG)" == "RdoLib - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SimTrace.cpp

!IF  "$(CFG)" == "RdoLib - Win32 Release"

!ELSEIF  "$(CFG)" == "RdoLib - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\IETrace.h
# End Source File
# Begin Source File

SOURCE=.\memcheck.h
# End Source File
# Begin Source File

SOURCE=.\OperationTrace.h
# End Source File
# Begin Source File

SOURCE=.\pch.h
# End Source File
# Begin Source File

SOURCE=.\Rdo.h
# End Source File
# Begin Source File

SOURCE=.\RdoBase.h
# End Source File
# Begin Source File

SOURCE=.\RdoDefines.h
# End Source File
# Begin Source File

SOURCE=.\RdoFunc.h
# End Source File
# Begin Source File

SOURCE=.\RdoTrace.h
# End Source File
# Begin Source File

SOURCE=.\RuleTrace.h
# End Source File
# Begin Source File

SOURCE=.\SearchTrace.h
# End Source File
# Begin Source File

SOURCE=.\SearchTree.h
# End Source File
# Begin Source File

SOURCE=.\SimTrace.h
# End Source File
# End Group
# End Target
# End Project
