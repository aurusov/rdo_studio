# Microsoft Developer Studio Project File - Name="rdo_tracer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=rdo_tracer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rdo_tracer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rdo_tracer.mak" CFG="rdo_tracer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rdo_tracer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "rdo_tracer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "rdo_tracer - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "./src/studio_files/rdo_ctrls" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /I /src/studio_files/rdo_ctrls" " /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Version.lib Htmlhelp.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "rdo_tracer - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./src/studio_files/rdo_ctrls" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Version.lib Htmlhelp.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "rdo_tracer - Win32 Release"
# Name "rdo_tracer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\rdotracerapp.cpp
# End Source File
# Begin Source File

SOURCE=.\src\rdotracerfilereader.cpp
# End Source File
# Begin Source File

SOURCE=.\src\rdotracermainfrm.cpp
# End Source File
# Begin Source File

SOURCE=.\src\rdotraceroptions.cpp
# End Source File
# Begin Source File

SOURCE=.\src\rdotracerpane.cpp
# ADD CPP /I ".."
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Htmlhelp.h
# End Source File
# Begin Source File

SOURCE=.\src\rdotracerapp.h
# End Source File
# Begin Source File

SOURCE=.\src\rdotracerfilereader.h
# End Source File
# Begin Source File

SOURCE=.\src\rdotracermainfrm.h
# End Source File
# Begin Source File

SOURCE=.\src\rdotraceroptions.h
# End Source File
# Begin Source File

SOURCE=.\src\rdotracerpane.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\logtoolb.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logtoolb_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\rdo_tracer.ico
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer.rc
# End Source File
# Begin Source File

SOURCE=.\res\rdo_tracer.rc2
# End Source File
# Begin Source File

SOURCE=.\res\tracetoolb.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tracetoolb_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\treectrl.bmp
# End Source File
# Begin Source File

SOURCE=.\res\zoomToolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\zoomToolbar_d.bmp
# End Source File
# End Group
# Begin Group "Tracer Ctrls"

# PROP Default_Filter ""
# Begin Group "Tracer Ctrls Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\tracer_ctrls\rdologctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\tracer_ctrls\rdologstyle.cpp
# End Source File
# Begin Source File

SOURCE=.\src\tracer_ctrls\rdosplitterwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\src\tracer_ctrls\rdotracerlogctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\tracer_ctrls\rdotracerlogstyle.cpp
# End Source File
# Begin Source File

SOURCE=.\src\tracer_ctrls\rdotracertreectrl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\tracer_ctrls\rdotracertreeitem.cpp
# End Source File
# End Group
# Begin Group "Tracer Ctrls Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\tracer_ctrls\rdologctrl.h
# End Source File
# Begin Source File

SOURCE=.\src\tracer_ctrls\rdologstyle.h
# End Source File
# Begin Source File

SOURCE=.\src\tracer_ctrls\rdosplitterwnd.h
# End Source File
# Begin Source File

SOURCE=.\src\tracer_ctrls\rdotracerlogctrl.h
# End Source File
# Begin Source File

SOURCE=.\src\tracer_ctrls\rdotracerlogstyle.h
# End Source File
# Begin Source File

SOURCE=.\src\tracer_ctrls\rdotracertreectrl.h
# End Source File
# Begin Source File

SOURCE=.\src\tracer_ctrls\rdotracertreeitem.h
# End Source File
# End Group
# End Group
# Begin Group "Studio Files"

# PROP Default_Filter ""
# Begin Group "Studio Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\studio_files\rdostudiostyle.cpp
# End Source File
# Begin Source File

SOURCE=.\src\studio_files\rdostudioview.cpp
# End Source File
# End Group
# Begin Group "Studio Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\studio_files\rdostudiostyle.h
# End Source File
# Begin Source File

SOURCE=.\src\studio_files\rdostudioview.h
# End Source File
# End Group
# End Group
# Begin Group "Trace"

# PROP Default_Filter ""
# Begin Group "Trace Sourcer Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\trace_files\rdostudiochartdoc.cpp
# End Source File
# Begin Source File

SOURCE=.\src\trace_files\rdostudiochartoptions.cpp
# End Source File
# Begin Source File

SOURCE=.\src\trace_files\rdostudiochartview.cpp
# End Source File
# Begin Source File

SOURCE=.\src\trace_files\rdostudiochartviewstyle.cpp
# End Source File
# Begin Source File

SOURCE=.\src\trace_files\rdostudiodocserie.cpp
# End Source File
# Begin Source File

SOURCE=.\src\trace_files\rdotracer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\trace_files\rdotracerbase.cpp
# End Source File
# Begin Source File

SOURCE=.\src\trace_files\rdotraceroperation.cpp
# End Source File
# Begin Source File

SOURCE=.\src\trace_files\rdotracerpattern.cpp
# End Source File
# Begin Source File

SOURCE=.\src\trace_files\rdotracerresource.cpp
# End Source File
# Begin Source File

SOURCE=.\src\trace_files\rdotracerrestype.cpp
# End Source File
# Begin Source File

SOURCE=.\src\trace_files\rdotracerresult.cpp
# End Source File
# Begin Source File

SOURCE=.\src\trace_files\rdotracerserie.cpp
# End Source File
# Begin Source File

SOURCE=.\src\trace_files\rdotracervalues.cpp
# End Source File
# End Group
# Begin Group "Trace Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\trace_files\rdostudiochartdoc.h
# End Source File
# Begin Source File

SOURCE=.\src\trace_files\rdostudiochartoptions.h
# End Source File
# Begin Source File

SOURCE=.\src\trace_files\rdostudiochartview.h
# End Source File
# Begin Source File

SOURCE=.\src\trace_files\rdostudiochartviewstyle.h
# End Source File
# Begin Source File

SOURCE=.\src\trace_files\rdostudiodocserie.h
# End Source File
# Begin Source File

SOURCE=.\src\trace_files\rdotracer.h
# End Source File
# Begin Source File

SOURCE=.\src\trace_files\rdotracerbase.h
# End Source File
# Begin Source File

SOURCE=.\src\trace_files\rdotraceroperation.h
# End Source File
# Begin Source File

SOURCE=.\src\trace_files\rdotracerpattern.h
# End Source File
# Begin Source File

SOURCE=.\src\trace_files\rdotracerresource.h
# End Source File
# Begin Source File

SOURCE=.\src\trace_files\rdotracerrestype.h
# End Source File
# Begin Source File

SOURCE=.\src\trace_files\rdotracerresult.h
# End Source File
# Begin Source File

SOURCE=.\src\trace_files\rdotracerserie.h
# End Source File
# Begin Source File

SOURCE=.\src\trace_files\rdotracervalues.h
# End Source File
# End Group
# End Group
# End Target
# End Project
