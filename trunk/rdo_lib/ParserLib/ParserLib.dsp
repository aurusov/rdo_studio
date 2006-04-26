# Microsoft Developer Studio Project File - Name="ParserLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ParserLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ParserLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ParserLib.mak" CFG="ParserLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ParserLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ParserLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ParserLib - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I "." /I ".." /I "..\System" /I "..\simulator" /I "../../rdo_common" /D "_LIB" /D "_UNISTD_H_" /D "WIN32" /D "NDEBUG" /D "_MBCS" /FD /c
# SUBTRACT CPP /Fr /YX
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GR /GX /ZI /Od /I "." /I ".." /I "..\System" /I "..\simulator" /I "../../rdo_common" /D "_LIB" /D "_UNISTD_H_" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_AFXDLL" /FR /YX /FD /GZ /c
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

# Name "ParserLib - Win32 Release"
# Name "ParserLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\pch.cpp

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# ADD CPP /Yc

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdodpt.cpp

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdofrm.cpp

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdofun.cpp

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdogramdpt.cpp

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdogramfrm.cpp

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdogramfun.cpp

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdogramopr.cpp

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdogrampat.cpp

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdogrampmd.cpp

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdogramrss.cpp

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdogramrtp.cpp

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdogramsmr1.cpp

!IF  "$(CFG)" == "ParserLib - Win32 Release"

# ADD CPP /Od

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdogramsmr2.cpp

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdohotkey.cpp

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdolex.cpp

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdolexinterface.cpp

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoopr.cpp

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoparser.cpp

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoparser_base.cpp

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoparser_lexer.cpp

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdopat.cpp

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdopatrtime.cpp

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdopmd.cpp

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdorss.cpp

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdortp.cpp

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoruntime.cpp

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdosmr.cpp

!IF  "$(CFG)" == "ParserLib - Win32 Release"

# ADD CPP /Od

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\System\bison.simple
# End Source File
# Begin Source File

SOURCE=..\System\FlexLexer.h
# End Source File
# Begin Source File

SOURCE=.\pch.h
# End Source File
# Begin Source File

SOURCE=.\rdocalcconst.h
# End Source File
# Begin Source File

SOURCE=.\rdodpt.h
# End Source File
# Begin Source File

SOURCE=.\rdodptrtime.h
# End Source File
# Begin Source File

SOURCE=.\rdofrm.h
# End Source File
# Begin Source File

SOURCE=.\rdofun.h
# End Source File
# Begin Source File

SOURCE=.\rdogramrtp.hpp
# End Source File
# Begin Source File

SOURCE=.\rdohotkey.h
# End Source File
# Begin Source File

SOURCE=.\rdolexinterface.h
# End Source File
# Begin Source File

SOURCE=.\rdoopr.h
# End Source File
# Begin Source File

SOURCE=.\rdoparselex.h
# End Source File
# Begin Source File

SOURCE=.\rdoparser.h
# End Source File
# Begin Source File

SOURCE=.\rdoparser_base.h
# End Source File
# Begin Source File

SOURCE=.\rdoparser_lexer.h
# End Source File
# Begin Source File

SOURCE=.\rdopat.h
# End Source File
# Begin Source File

SOURCE=.\rdopatrtime.h
# End Source File
# Begin Source File

SOURCE=.\rdopmd.h
# End Source File
# Begin Source File

SOURCE=.\rdorss.h
# End Source File
# Begin Source File

SOURCE=.\rdortp.h
# End Source File
# Begin Source File

SOURCE=.\rdoruntime.h
# End Source File
# Begin Source File

SOURCE=.\rdosmr.h
# End Source File
# Begin Source File

SOURCE=.\rdoStdFuncs.h
# End Source File
# Begin Source File

SOURCE=..\System\unistd.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\parser.l

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# Begin Custom Build
InputPath=.\parser.l

"rdolex.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\System\flex.exe -ordolex.cpp -+ $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdodpt.y

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# Begin Custom Build
InputPath=.\rdodpt.y

"rdogramdpt.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\System\bison.exe -pdpt --skeleton=..\System\bison.simple -ordogramdpt.cpp $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdofrm.y

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# Begin Custom Build
InputPath=.\rdofrm.y

"rdogramfrm.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\System\bison.exe -pfrm --skeleton=..\System\bison.simple -ordogramfrm.cpp $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdofun.y

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# Begin Custom Build
InputPath=.\rdofun.y

"rdogramfun.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\System\bison.exe -pfun --skeleton=..\System\bison.simple -ordogramfun.cpp $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoopr.y

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# Begin Custom Build
InputPath=.\rdoopr.y

"rdogramopr.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\System\bison.exe -popr --skeleton=..\System\bison.simple -ordogramopr.cpp $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdopat.y

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# Begin Custom Build
InputPath=.\rdopat.y

"rdogrampat.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\System\bison.exe -ppat --skeleton=..\System\bison.simple -ordogrampat.cpp $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdopmd.y

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# Begin Custom Build
InputPath=.\rdopmd.y

"rdogrampmd.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\System\bison.exe -ppmd --skeleton=..\System\bison.simple -ordogrampmd.cpp $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdorss.y

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# Begin Custom Build
InputPath=.\rdorss.y

"rdogramrss.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\System\bison.exe -prss --skeleton=..\System\bison.simple -ordogramrss.cpp $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdortp.y

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# Begin Custom Build
InputPath=.\rdortp.y

BuildCmds= \
	..\System\bison.exe -d -prtp --skeleton=..\System\bison.simple -ordogramrtp.cpp $(InputPath)

"rdogramrtp.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"rdogramrtp.hpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdosmr1.y

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# Begin Custom Build
InputPath=.\rdosmr1.y

"rdogramsmr1.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\System\bison.exe -psmr1 --skeleton=..\System\bison.simple -ordogramsmr1.cpp $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdosmr2.y

!IF  "$(CFG)" == "ParserLib - Win32 Release"

!ELSEIF  "$(CFG)" == "ParserLib - Win32 Debug"

# Begin Custom Build
InputPath=.\rdosmr2.y

"rdogramsmr2.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\System\bison.exe -psmr2 --skeleton=..\System\bison.simple -ordogramsmr2.cpp $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
