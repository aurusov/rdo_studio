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
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../rdo_ctrls" /I "../rdo_kernel" /I "../rdo_repository" /I "../rdo_lib/simulator" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../rdo_ctrls" /I "../rdo_kernel" /I "../rdo_repository" /I "../rdo_lib/simulator" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
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
# Begin Group "RDO-Studio"

# PROP Default_Filter ""
# Begin Group "RDO-Studio Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\rdostudioapp.cpp

!IF  "$(CFG)" == "rdo_studio - Win32 Release"

!ELSEIF  "$(CFG)" == "rdo_studio - Win32 Debug"

# SUBTRACT CPP /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdostudiochartdoc.cpp

!IF  "$(CFG)" == "rdo_studio - Win32 Release"

!ELSEIF  "$(CFG)" == "rdo_studio - Win32 Debug"

# SUBTRACT CPP /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdostudiochartview.cpp

!IF  "$(CFG)" == "rdo_studio - Win32 Release"

!ELSEIF  "$(CFG)" == "rdo_studio - Win32 Debug"

# SUBTRACT CPP /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdostudiochildfrm.cpp

!IF  "$(CFG)" == "rdo_studio - Win32 Release"

!ELSEIF  "$(CFG)" == "rdo_studio - Win32 Debug"

# SUBTRACT CPP /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdostudiodockwnd.cpp

!IF  "$(CFG)" == "rdo_studio - Win32 Release"

!ELSEIF  "$(CFG)" == "rdo_studio - Win32 Debug"

# SUBTRACT CPP /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdostudioeditbasedoc.cpp

!IF  "$(CFG)" == "rdo_studio - Win32 Release"

!ELSEIF  "$(CFG)" == "rdo_studio - Win32 Debug"

# SUBTRACT CPP /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdostudioeditbaseview.cpp

!IF  "$(CFG)" == "rdo_studio - Win32 Release"

!ELSEIF  "$(CFG)" == "rdo_studio - Win32 Debug"

# SUBTRACT CPP /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdostudioeditdoc.cpp

!IF  "$(CFG)" == "rdo_studio - Win32 Release"

!ELSEIF  "$(CFG)" == "rdo_studio - Win32 Debug"

# SUBTRACT CPP /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdostudioeditview.cpp

!IF  "$(CFG)" == "rdo_studio - Win32 Release"

!ELSEIF  "$(CFG)" == "rdo_studio - Win32 Debug"

# SUBTRACT CPP /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdostudioframedoc.cpp
# End Source File
# Begin Source File

SOURCE=.\rdostudioframeview.cpp
# End Source File
# Begin Source File

SOURCE=.\rdostudiomainfrm.cpp

!IF  "$(CFG)" == "rdo_studio - Win32 Release"

!ELSEIF  "$(CFG)" == "rdo_studio - Win32 Debug"

# SUBTRACT CPP /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdostudiomodel.cpp

!IF  "$(CFG)" == "rdo_studio - Win32 Release"

!ELSEIF  "$(CFG)" == "rdo_studio - Win32 Debug"

# SUBTRACT CPP /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdostudiomodeldoc.cpp

!IF  "$(CFG)" == "rdo_studio - Win32 Release"

!ELSEIF  "$(CFG)" == "rdo_studio - Win32 Debug"

# SUBTRACT CPP /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdostudiomodelview.cpp

!IF  "$(CFG)" == "rdo_studio - Win32 Release"

!ELSEIF  "$(CFG)" == "rdo_studio - Win32 Debug"

# SUBTRACT CPP /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdostudiooutput.cpp

!IF  "$(CFG)" == "rdo_studio - Win32 Release"

!ELSEIF  "$(CFG)" == "rdo_studio - Win32 Debug"

# SUBTRACT CPP /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdostudioworkspace.cpp

!IF  "$(CFG)" == "rdo_studio - Win32 Release"

!ELSEIF  "$(CFG)" == "rdo_studio - Win32 Debug"

# SUBTRACT CPP /Fr

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "rdo_studio - Win32 Release"

# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "rdo_studio - Win32 Debug"

# ADD CPP /Yc"stdafx.h"
# SUBTRACT CPP /Fr

!ENDIF 

# End Source File
# End Group
# Begin Group "RDO-Studio Header Files"

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

SOURCE=.\rdostudioframedoc.h
# End Source File
# Begin Source File

SOURCE=.\rdostudioframeview.h
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

SOURCE=.\rdo_edit\rdoeditortabctrl.h
# End Source File
# End Group
# End Group
# Begin Group "RDO-Tracer"

# PROP Default_Filter ""
# Begin Group "RDO-Tracer Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\rdo_tracer\rdotraceroperation.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\rdotraceroptions.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\rdotracerpattern.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\rdotracerresource.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\rdotracerrestype.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\rdotracerresult.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\rdotracerserie.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\rdotracertrace.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\rdotracervalues.cpp
# End Source File
# End Group
# Begin Group "RDO-Tracer Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\rdo_tracer\rdotraceroperation.h
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\rdotraceroptions.h
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\rdotracerpattern.h
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\rdotracerresource.h
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\rdotracerrestype.h
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\rdotracerresult.h
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\rdotracerserie.h
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\rdotracertrace.h
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\rdotracervalues.h
# End Source File
# End Group
# Begin Group "Tracer Ctrls"

# PROP Default_Filter ""
# Begin Group "Tracer Ctrls Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\rdo_tracer\tracer_ctrls\rdologctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\tracer_ctrls\rdologstyle.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\tracer_ctrls\rdotracerlogctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\tracer_ctrls\rdotracerlogstyle.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\tracer_ctrls\rdotracertreectrl.cpp
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\tracer_ctrls\rdotracertreeitem.cpp
# End Source File
# End Group
# Begin Group "Tracer Ctrls Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\rdo_tracer\tracer_ctrls\rdologctrl.h
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\tracer_ctrls\rdologstyle.h
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\tracer_ctrls\rdotracerlogctrl.h
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\tracer_ctrls\rdotracerlogstyle.h
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\tracer_ctrls\rdotracertreectrl.h
# End Source File
# Begin Source File

SOURCE=.\rdo_tracer\tracer_ctrls\rdotracertreeitem.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Edit Ctrls"

# PROP Default_Filter ""
# Begin Group "Edit Ctrls Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\edit_ctrls\rdobaseedit.cpp
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\rdobaseeditstyle.cpp
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\rdobuildedit.cpp
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\rdodebugedit.cpp
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\rdofindedit.cpp
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\rdofindeditstyle.cpp
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\rdologedit.cpp
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\rdologeditstyle.cpp
# End Source File
# End Group
# Begin Group "Edit Ctrls Headers Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\edit_ctrls\rdobaseedit.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\rdobaseeditstyle.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\rdobuildedit.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\rdodebugedit.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\rdofindedit.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\rdofindeditstyle.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\rdologedit.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\rdologeditstyle.h
# End Source File
# End Group
# Begin Group "Scintilla"

# PROP Default_Filter ""
# Begin Group "Scintilla Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\edit_ctrls\sci\AutoComplete.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\CallTip.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\CellBuffer.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\ContractionState.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\Document.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\DocumentAccessor.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\Editor.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\ExternalLexer.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\Indicator.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\KeyMap.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\KeyWords.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\LexFind.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\LexRdo.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\LineMarker.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\PlatWin.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\PropSet.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\RESearch.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\ScintillaBase.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\ScintillaWin.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\Style.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\StyleContext.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\UniConversion.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\ViewStyle.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\WindowAccessor.cxx
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "Scintilla Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\edit_ctrls\sci\Accessor.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\AutoComplete.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\CallTip.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\CellBuffer.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\ContractionState.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\Document.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\DocumentAccessor.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\Editor.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\ExternalLexer.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\Indicator.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\KeyMap.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\KeyWords.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\LexFind.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\LexRdo.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\LineMarker.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\Platform.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\PlatformRes.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\PropSet.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\RESearch.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\SciLexer.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\Scintilla.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\ScintillaBase.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\ScintillaWidget.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\SString.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\Style.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\StyleContext.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\SVector.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\UniConversion.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\ViewStyle.h
# End Source File
# Begin Source File

SOURCE=.\edit_ctrls\sci\WindowAccessor.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\edit_ctrls\sci\License.txt
# End Source File
# End Group
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\buildToolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\editToolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\editToolbar_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\fileToolbar.bmp
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

SOURCE=.\res\rdostudioeditdoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\rdostudioframedoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\rdostudiomodeldoc.ico
# End Source File
# End Group
# End Target
# End Project
