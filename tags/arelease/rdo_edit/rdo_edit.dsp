# Microsoft Developer Studio Project File - Name="rdo_edit" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=rdo_edit - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rdo_edit.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rdo_edit.mak" CFG="rdo_edit - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rdo_edit - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "rdo_edit - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "rdo_edit - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Imm32.lib Htmlhelp.lib Version.lib /nologo /subsystem:windows /machine:I386 /out:"Release/RAO-editor.exe"

!ELSEIF  "$(CFG)" == "rdo_edit - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fr /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Imm32.lib Htmlhelp.lib Version.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/RAO-editor.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "rdo_edit - Win32 Release"
# Name "rdo_edit - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\rdoabout.cpp
# End Source File
# Begin Source File

SOURCE=.\src\rdocolorcombobox.cpp
# End Source File
# Begin Source File

SOURCE=.\src\rdoeditorapp.cpp
# End Source File
# Begin Source File

SOURCE=.\src\rdoeditorchildview.cpp
# End Source File
# Begin Source File

SOURCE=.\src\rdoeditoredit.cpp
# End Source File
# Begin Source File

SOURCE=.\src\rdoeditoreditstyle.cpp
# End Source File
# Begin Source File

SOURCE=.\src\rdoeditormainfrm.cpp
# End Source File
# Begin Source File

SOURCE=.\src\rdoeditoroptions.cpp
# End Source File
# Begin Source File

SOURCE=.\src\rdoeditorproject.cpp
# End Source File
# Begin Source File

SOURCE=.\src\rdoeditortabctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\rdolink.cpp
# End Source File
# Begin Source File

SOURCE=.\src\rdotabctrl.cpp
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

SOURCE=.\src\rdoabout.h
# End Source File
# Begin Source File

SOURCE=.\src\rdocolorcombobox.h
# End Source File
# Begin Source File

SOURCE=.\src\rdoeditorapp.h
# End Source File
# Begin Source File

SOURCE=.\src\rdoeditorchildview.h
# End Source File
# Begin Source File

SOURCE=.\src\rdoeditoredit.h
# End Source File
# Begin Source File

SOURCE=.\src\rdoeditoreditstyle.h
# End Source File
# Begin Source File

SOURCE=.\src\rdoeditormainfrm.h
# End Source File
# Begin Source File

SOURCE=.\src\rdoeditoroptions.h
# End Source File
# Begin Source File

SOURCE=.\src\rdoeditorproject.h
# End Source File
# Begin Source File

SOURCE=.\src\rdoeditortabctrl.h
# End Source File
# Begin Source File

SOURCE=.\src\rdolink.h
# End Source File
# Begin Source File

SOURCE=.\src\rdotabctrl.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\src\sci\Margin.cur
# End Source File
# Begin Source File

SOURCE=.\res\rdo_edit.ico
# End Source File
# Begin Source File

SOURCE=.\rdo_edit.rc
# End Source File
# Begin Source File

SOURCE=.\res\rdo_edit.rc2
# End Source File
# End Group
# Begin Group "sci"

# PROP Default_Filter ""
# Begin Group "source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\sci\AutoComplete.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\src\sci\CallTip.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\src\sci\CellBuffer.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\src\sci\ContractionState.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\src\sci\Document.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\src\sci\DocumentAccessor.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\src\sci\Editor.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\src\sci\ExternalLexer.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\src\sci\Indicator.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\src\sci\KeyMap.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\src\sci\KeyWords.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\src\sci\LexRdo.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\src\sci\LineMarker.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\src\sci\PlatWin.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\src\sci\PropSet.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\src\sci\RESearch.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\src\sci\ScintillaBase.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\src\sci\ScintillaWin.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\src\sci\Style.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\src\sci\StyleContext.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\src\sci\UniConversion.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\src\sci\ViewStyle.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\src\sci\WindowAccessor.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "header"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\src\sci\Accessor.h
# End Source File
# Begin Source File

SOURCE=.\src\sci\AutoComplete.h
# End Source File
# Begin Source File

SOURCE=.\src\sci\CallTip.h
# End Source File
# Begin Source File

SOURCE=.\src\sci\CellBuffer.h
# End Source File
# Begin Source File

SOURCE=.\src\sci\ContractionState.h
# End Source File
# Begin Source File

SOURCE=.\src\sci\Document.h
# End Source File
# Begin Source File

SOURCE=.\src\sci\DocumentAccessor.h
# End Source File
# Begin Source File

SOURCE=.\src\sci\Editor.h
# End Source File
# Begin Source File

SOURCE=.\src\sci\ExternalLexer.h
# End Source File
# Begin Source File

SOURCE=.\src\sci\Indicator.h
# End Source File
# Begin Source File

SOURCE=.\src\sci\KeyMap.h
# End Source File
# Begin Source File

SOURCE=.\src\sci\KeyWords.h
# End Source File
# Begin Source File

SOURCE=.\src\sci\LexRdo.h
# End Source File
# Begin Source File

SOURCE=.\src\sci\LineMarker.h
# End Source File
# Begin Source File

SOURCE=.\src\sci\Platform.h
# End Source File
# Begin Source File

SOURCE=.\src\sci\PlatformRes.h
# End Source File
# Begin Source File

SOURCE=.\src\sci\PropSet.h
# End Source File
# Begin Source File

SOURCE=.\src\sci\RESearch.h
# End Source File
# Begin Source File

SOURCE=.\src\sci\SciLexer.h
# End Source File
# Begin Source File

SOURCE=.\src\sci\Scintilla.h
# End Source File
# Begin Source File

SOURCE=.\src\sci\ScintillaBase.h
# End Source File
# Begin Source File

SOURCE=.\src\sci\ScintillaWidget.h
# End Source File
# Begin Source File

SOURCE=.\src\sci\SString.h
# End Source File
# Begin Source File

SOURCE=.\src\sci\Style.h
# End Source File
# Begin Source File

SOURCE=.\src\sci\StyleContext.h
# End Source File
# Begin Source File

SOURCE=.\src\sci\SVector.h
# End Source File
# Begin Source File

SOURCE=.\src\sci\UniConversion.h
# End Source File
# Begin Source File

SOURCE=.\src\sci\ViewStyle.h
# End Source File
# Begin Source File

SOURCE=.\src\sci\WindowAccessor.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\sci\License.txt
# End Source File
# End Group
# End Target
# End Project
