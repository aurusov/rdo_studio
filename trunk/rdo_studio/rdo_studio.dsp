# Microsoft Developer Studio Project File - Name="rdo_studio" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=rdo_studio - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rdo_studio.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rdo_studio.mak" CFG="rdo_studio - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rdo_studio - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "rdo_studio - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "rdo_studio - Win32 Release"

# PROP BASE Use_MFC 6
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
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../rdo_ctrls" /I "../rdo_kernel" /I "../rdo_repository" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Imm32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "rdo_studio - Win32 Debug"

# PROP BASE Use_MFC 6
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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../rdo_ctrls" /I "../rdo_kernel" /I "../rdo_repository" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Imm32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "rdo_studio - Win32 Release"
# Name "rdo_studio - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\rdostudioapp.cpp
# End Source File
# Begin Source File

SOURCE=.\rdostudiochartdoc.cpp
# End Source File
# Begin Source File

SOURCE=.\rdostudiochartview.cpp
# End Source File
# Begin Source File

SOURCE=.\rdostudiochildfrm.cpp
# End Source File
# Begin Source File

SOURCE=.\rdostudiodockwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rdostudioeditbasedoc.cpp
# End Source File
# Begin Source File

SOURCE=.\rdostudioeditbaseview.cpp
# End Source File
# Begin Source File

SOURCE=.\rdostudioeditdoc.cpp
# End Source File
# Begin Source File

SOURCE=.\rdostudioeditview.cpp
# End Source File
# Begin Source File

SOURCE=.\rdostudiomainfrm.cpp
# End Source File
# Begin Source File

SOURCE=.\rdostudiomodel.cpp
# End Source File
# Begin Source File

SOURCE=.\rdostudiomodeldoc.cpp
# End Source File
# Begin Source File

SOURCE=.\rdostudiomodelview.cpp
# End Source File
# Begin Source File

SOURCE=.\rdostudiooutput.cpp
# End Source File
# Begin Source File

SOURCE=.\rdostudioworkspace.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\rdo_repository_file\rdorepositoryfile.h
# End Source File
# Begin Source File

SOURCE=.\rdostudioapp.h
# End Source File
# Begin Source File

SOURCE=.\rdostudiochartdoc.h
# End Source File
# Begin Source File

SOURCE=.\rdostudiochartview.h
# End Source File
# Begin Source File

SOURCE=.\rdostudiochildfrm.h
# End Source File
# Begin Source File

SOURCE=.\rdostudiodockwnd.h
# End Source File
# Begin Source File

SOURCE=.\rdostudioeditbasedoc.h
# End Source File
# Begin Source File

SOURCE=.\rdostudioeditbaseview.h
# End Source File
# Begin Source File

SOURCE=.\rdostudioeditdoc.h
# End Source File
# Begin Source File

SOURCE=.\rdostudioeditview.h
# End Source File
# Begin Source File

SOURCE=.\rdostudiomainfrm.h
# End Source File
# Begin Source File

SOURCE=.\rdostudiomodel.h
# End Source File
# Begin Source File

SOURCE=.\rdostudiomodeldoc.h
# End Source File
# Begin Source File

SOURCE=.\rdostudiomodelview.h
# End Source File
# Begin Source File

SOURCE=.\rdostudiooutput.h
# End Source File
# Begin Source File

SOURCE=.\rdostudioworkspace.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "RDO-Editor"

# PROP Default_Filter ""
# Begin Group "RDO-Editor Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rdo_edit\rdoeditorapp.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\rdoeditorchildview.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\rdoeditoredit.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\rdoeditoreditstyle.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\rdoeditormainfrm.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\rdoeditoroptions.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\rdoeditorproject.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\rdoeditorsciedit.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\rdoeditorscieditstyle.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\rdoeditorscilog.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\rdoeditorscilogstyle.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\rdoeditortabctrl.cpp
# End Source File
# End Group
# Begin Group "RDO-Editor Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rdo_edit\rdoeditorapp.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\rdoeditorchildview.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\rdoeditoredit.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\rdoeditoreditstyle.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\rdoeditormainfrm.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\rdoeditoroptions.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\rdoeditorproject.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\rdoeditorsciedit.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\rdoeditorscieditstyle.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\rdoeditorscilog.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\rdoeditorscilogstyle.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\rdoeditortabctrl.h
# End Source File
# End Group
# Begin Group "Scintilla"

# PROP Default_Filter ""
# Begin Group "Scintilla Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rdo_edit\sci\AutoComplete.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\CallTip.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\CellBuffer.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\ContractionState.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\Document.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\DocumentAccessor.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\Editor.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\ExternalLexer.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\Indicator.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\KeyMap.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\KeyWords.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\LexRdo.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\LineMarker.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\PlatWin.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\PropSet.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\RESearch.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\ScintillaBase.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\ScintillaWin.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\Style.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\StyleContext.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\UniConversion.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\ViewStyle.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\WindowAccessor.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "Scintilla Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rdo_edit\sci\Accessor.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\AutoComplete.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\CallTip.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\CellBuffer.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\ContractionState.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\Document.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\DocumentAccessor.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\Editor.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\ExternalLexer.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\Indicator.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\KeyMap.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\KeyWords.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\LexRdo.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\LineMarker.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\Platform.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\PlatformRes.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\PropSet.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\RESearch.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\SciLexer.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\Scintilla.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\ScintillaBase.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\ScintillaWidget.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\SString.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\Style.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\StyleContext.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\SVector.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\UniConversion.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\ViewStyle.h
# End Source File
# Begin Source File

SOURCE=.\rdo_edit\sci\WindowAccessor.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "RDO-Tracer"

# PROP Default_Filter ""
# Begin Group "RDO-Tracer Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\rdo_tracer\rdotracerlogctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\rdotracerlogstyle.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\rdotraceroptions.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\rdotracerrdoclasses.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\rdotracertrace.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\rdotracertreectrl.cpp
# End Source File
# End Group
# Begin Group "RDO-Tracer Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\rdo_tracer\rdotracerlogctrl.h
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\rdotracerlogstyle.h
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\rdotraceroptions.h
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\rdotracerrdoclasses.h
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\rdotracertrace.h
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\rdotracertreectrl.h
# End Source File
# End Group
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\editToolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\editToolbar_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\idr_mode.ico
# End Source File
# Begin Source File

SOURCE=.\res\Margin.cur
# End Source File
# Begin Source File

SOURCE=.\res\projectToolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\projectToolbar_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\rdo_studio.ico
# End Source File
# Begin Source File

SOURCE=.\rdo_studio.rc
# End Source File
# Begin Source File

SOURCE=.\res\rdo_studio.rc2
# End Source File
# Begin Source File

SOURCE=.\rdo_studio.reg
# End Source File
# Begin Source File

SOURCE=.\res\rdostudiochartdoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\rdostudiodoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\rdostudioeditdoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\rdostudiomodeldoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tracertreectrl.bmp
# End Source File
# End Group
# Begin Group "CSizingControlBar"

# PROP Default_Filter ""
# Begin Group "CSizingControlBar Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\sizecbar\scbarg.cpp
# End Source File
# Begin Source File

SOURCE=.\sizecbar\sizecbar.cpp
# End Source File
# End Group
# Begin Group "CSizingControlBar Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\sizecbar\scbarg.h
# End Source File
# Begin Source File

SOURCE=.\sizecbar\sizecbar.h
# End Source File
# End Group
# End Group
# End Target
# End Project
