# Microsoft Developer Studio Project File - Name="rdo_parser" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=rdo_parser - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rdo_parser.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rdo_parser.mak" CFG="rdo_parser - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rdo_parser - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "rdo_parser - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /Ob2 /I "." /I ".." /I "../rdo_system" /I "../rdo_runtime" /I "../rdo_mbuilder" /I "../../rdo_common" /I "../../rdo_kernel" /I "../../rdo_repository" /D "_LIB" /D "_UNISTD_H_" /D "WIN32" /D "NDEBUG" /D "_MBCS" /Yu"pch.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GR /GX /ZI /Od /I "." /I ".." /I "../rdo_system" /I "../rdo_runtime" /I "../rdo_mbuilder" /I "../../rdo_common" /I "../../rdo_kernel" /I "../../rdo_repository" /D "RDOSIM_COMPATIBLE" /D "_LIB" /D "_UNISTD_H_" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_AFXDLL" /FR /Yu"pch.h" /FD /GZ /c
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

# Name "rdo_parser - Win32 Release"
# Name "rdo_parser - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\pch.cpp
# ADD CPP /Yc"pch.h"
# End Source File
# Begin Source File

SOURCE=.\rdodpt.cpp

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# ADD CPP /O2 /Yu"pch.h"

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdofrm.cpp

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# ADD CPP /O2 /Yu"pch.h"

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdofun.cpp

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# ADD CPP /O2 /Yu"pch.h"

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoopr.cpp

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# ADD CPP /O2 /Yu"pch.h"

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoparser.cpp

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# ADD CPP /O2 /Yu"pch.h"

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoparser_base.cpp

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# ADD CPP /O2 /Yu"pch.h"

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoparser_corba.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoparser_lexer.cpp

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# ADD CPP /O2 /Yu"pch.h"

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoparser_object.cpp
# End Source File
# Begin Source File

SOURCE=.\rdoparser_rdo.cpp

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# ADD CPP /O2 /Yu"pch.h"

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdopat.cpp

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# ADD CPP /O2 /Yu"pch.h"

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdopmd.cpp

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# ADD CPP /O2 /Yu"pch.h"

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdorss.cpp

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# ADD CPP /O2 /Yu"pch.h"

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdortp.cpp

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# ADD CPP /O2 /Yu"pch.h"

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdosmr.cpp

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# ADD CPP /O2 /Yu"pch.h"

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# ADD CPP /Yu"pch.h"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\pch.h
# End Source File
# Begin Source File

SOURCE=.\rdodpt.h
# End Source File
# Begin Source File

SOURCE=.\rdofrm.h
# End Source File
# Begin Source File

SOURCE=.\rdofun.h
# End Source File
# Begin Source File

SOURCE=.\rdoopr.h
# End Source File
# Begin Source File

SOURCE=.\rdoparser.h
# End Source File
# Begin Source File

SOURCE=.\rdoparser_base.h
# End Source File
# Begin Source File

SOURCE=.\rdoparser_corba.h
# End Source File
# Begin Source File

SOURCE=.\rdoparser_lexer.h
# End Source File
# Begin Source File

SOURCE=.\rdoparser_object.h
# End Source File
# Begin Source File

SOURCE=.\rdoparser_rdo.h
# End Source File
# Begin Source File

SOURCE=.\rdopat.h
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

SOURCE=.\rdosmr.h
# End Source File
# End Group
# Begin Group "bison"

# PROP Default_Filter ""
# Begin Group "generated"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rdogramdpt.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdogramfrm.cpp

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# SUBTRACT CPP /O<none> /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdogramfun.cpp

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# ADD CPP /GB /Zp8 /Ze /vmb /O1
# SUBTRACT CPP /Fr /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdogramma.h
# End Source File
# Begin Source File

SOURCE=.\rdogramopr.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdogrampat.cpp

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# ADD CPP /O1
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdogrampmd.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdogramproc_opr.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdogramproc_rss.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdogramproc_rtp.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdogramrss.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdogramrtp.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdogramsmr_file.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdogramsmr_sim.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\rdolex.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Source File

SOURCE=..\rdo_system\bison.simple
# End Source File
# Begin Source File

SOURCE=..\rdo_system\FlexLexer.h
# End Source File
# Begin Source File

SOURCE=.\rdo_lexer.l

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# Begin Custom Build
InputPath=.\rdo_lexer.l

"rdolex.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\rdo_system\flex.exe -ordolex.cpp -+ $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# Begin Custom Build
InputPath=.\rdo_lexer.l

"rdolex.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\rdo_system\flex.exe -ordolex.cpp -+ $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdodpt.y

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# Begin Custom Build
InputPath=.\rdodpt.y

"rdogramdpt.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\rdo_system\bison.exe -pdpt --skeleton=..\rdo_system\bison.simple -ordogramdpt.cpp $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# Begin Custom Build
InputPath=.\rdodpt.y

"rdogramdpt.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\rdo_system\bison.exe -pdpt --skeleton=..\rdo_system\bison.simple -ordogramdpt.cpp $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdofrm.y

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# Begin Custom Build
InputPath=.\rdofrm.y

"rdogramfrm.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\rdo_system\bison.exe -pfrm --skeleton=..\rdo_system\bison.simple -ordogramfrm.cpp $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# Begin Custom Build
InputPath=.\rdofrm.y

"rdogramfrm.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\rdo_system\bison.exe -pfrm --skeleton=..\rdo_system\bison.simple -ordogramfrm.cpp $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdofun.y

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# Begin Custom Build
InputPath=.\rdofun.y

"rdogramfun.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\rdo_system\bison.exe -pfun --skeleton=..\rdo_system\bison.simple -ordogramfun.cpp $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# Begin Custom Build
InputPath=.\rdofun.y

"rdogramfun.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\rdo_system\bison.exe -pfun --skeleton=..\rdo_system\bison.simple -ordogramfun.cpp $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoopr.y

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# Begin Custom Build
InputPath=.\rdoopr.y

"rdogramopr.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\rdo_system\bison.exe -popr --skeleton=..\rdo_system\bison.simple -ordogramopr.cpp $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# Begin Custom Build
InputPath=.\rdoopr.y

"rdogramopr.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\rdo_system\bison.exe -popr --skeleton=..\rdo_system\bison.simple -ordogramopr.cpp $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdopat.y

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# Begin Custom Build
InputPath=.\rdopat.y

"rdogrampat.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\rdo_system\bison.exe -ppat --skeleton=..\rdo_system\bison.simple -ordogrampat.cpp $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# Begin Custom Build
InputPath=.\rdopat.y

"rdogrampat.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\rdo_system\bison.exe -ppat --skeleton=..\rdo_system\bison.simple -ordogrampat.cpp $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdopmd.y

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# Begin Custom Build
InputPath=.\rdopmd.y

"rdogrampmd.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\rdo_system\bison.exe -ppmd --skeleton=..\rdo_system\bison.simple -ordogrampmd.cpp $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# Begin Custom Build
InputPath=.\rdopmd.y

"rdogrampmd.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\rdo_system\bison.exe -ppmd --skeleton=..\rdo_system\bison.simple -ordogrampmd.cpp $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoproc_opr.y

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# Begin Custom Build
InputPath=.\rdoproc_opr.y

"rdogramproc_opr.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\rdo_system\bison.exe -pproc_opr_ --skeleton=..\rdo_system\bison.simple -ordogramproc_opr.cpp $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# Begin Custom Build
InputPath=.\rdoproc_opr.y

"rdogramproc_opr.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\rdo_system\bison.exe -pproc_opr_ --skeleton=..\rdo_system\bison.simple -ordogramproc_opr.cpp $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoproc_rss.y

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# Begin Custom Build
InputPath=.\rdoproc_rss.y

"rdogramproc_rss.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\rdo_system\bison.exe -pproc_rss_ --skeleton=..\rdo_system\bison.simple -ordogramproc_rss.cpp $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# Begin Custom Build
InputPath=.\rdoproc_rss.y

"rdogramproc_rss.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\rdo_system\bison.exe -pproc_rss_ --skeleton=..\rdo_system\bison.simple -ordogramproc_rss.cpp $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdoproc_rtp.y

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# Begin Custom Build
InputPath=.\rdoproc_rtp.y

"rdogramproc_rtp.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\rdo_system\bison.exe -pproc_rtp_ --skeleton=..\rdo_system\bison.simple -ordogramproc_rtp.cpp $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# Begin Custom Build
InputPath=.\rdoproc_rtp.y

"rdogramproc_rtp.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\rdo_system\bison.exe -pproc_rtp_ --skeleton=..\rdo_system\bison.simple -ordogramproc_rtp.cpp $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdorss.y

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# Begin Custom Build
InputPath=.\rdorss.y

"rdogramrss.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\rdo_system\bison.exe -prss --skeleton=..\rdo_system\bison.simple -ordogramrss.cpp $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# Begin Custom Build
InputPath=.\rdorss.y

"rdogramrss.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\rdo_system\bison.exe -v -prss --skeleton=..\rdo_system\bison.simple -ordogramrss.cpp $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdortp.y

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# Begin Custom Build
InputPath=.\rdortp.y

BuildCmds= \
	..\rdo_system\bison.exe -v --defines=rdogramma.h -prtp --skeleton=..\rdo_system\bison.simple -ordogramrtp.cpp $(InputPath)

"rdogramrtp.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"rdogramma.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# Begin Custom Build
InputPath=.\rdortp.y

BuildCmds= \
	..\rdo_system\bison.exe -v --defines=rdogramma.h -prtp --skeleton=..\rdo_system\bison.simple -ordogramrtp.cpp $(InputPath)

"rdogramrtp.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"rdogramma.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdosmr_file.y

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# Begin Custom Build
InputPath=.\rdosmr_file.y

"rdogramsmr_file.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\rdo_system\bison.exe -psmr_file_ --skeleton=..\rdo_system\bison.simple -ordogramsmr_file.cpp $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# Begin Custom Build
InputPath=.\rdosmr_file.y

"rdogramsmr_file.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\rdo_system\bison.exe -psmr_file_ --skeleton=..\rdo_system\bison.simple -ordogramsmr_file.cpp $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rdosmr_sim.y

!IF  "$(CFG)" == "rdo_parser - Win32 Release"

# Begin Custom Build
InputPath=.\rdosmr_sim.y

"rdogramsmr_sim.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\rdo_system\bison.exe -psmr_sim_ --skeleton=..\rdo_system\bison.simple -ordogramsmr_sim.cpp $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "rdo_parser - Win32 Debug"

# Begin Custom Build
InputPath=.\rdosmr_sim.y

"rdogramsmr_sim.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\rdo_system\bison.exe -psmr_sim_ --skeleton=..\rdo_system\bison.simple -ordogramsmr_sim.cpp $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\rdo_system\unistd.h
# End Source File
# End Group
# End Target
# End Project
