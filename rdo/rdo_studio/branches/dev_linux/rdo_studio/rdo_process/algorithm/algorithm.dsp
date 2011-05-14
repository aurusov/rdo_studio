# Microsoft Developer Studio Project File - Name="method_algorithm" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=method_algorithm - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "algorithm.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "algorithm.mak" CFG="method_algorithm - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "method_algorithm - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "method_algorithm - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "method_algorithm - Win32 Debug"

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
# ADD BASE MTL /nologo /D"_DEBUG" /win32
# ADD MTL /nologo /D"_DEBUG" /win32
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../rp_misc" /I "../../rp_method" /I "../../rp_ctrl" /D "_CRT_SECURE_NO_DEPRECATE" /D "_AFXDLL" /YX /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../rp_misc" /I "../../rp_method" /I "../../rp_ctrl" /D "_CRT_SECURE_NO_DEPRECATE" /D "_AFXDLL" /Yu"stdafx.h" /GZ /c
# ADD BASE RSC /l 0x409 /i "$(IntDir)" /d "_DEBUG"
# ADD RSC /l 0x409 /i "$(IntDir)" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Debug\method_algorithm.lib"

!ELSEIF  "$(CFG)" == "method_algorithm - Win32 Release"

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
# ADD BASE MTL /nologo /D"NDEBUG" /win32
# ADD MTL /nologo /D"NDEBUG" /win32
# ADD BASE CPP /nologo /MT /W3 /Gm /GX /Zi /Ob2 /I "../../rp_misc" /I "../../rp_method" /I "../../rp_ctrl" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_CRT_SECURE_NO_DEPRECATE" /D "_MBCS" /YX /c
# ADD CPP /nologo /MT /W3 /Gm /GX /Zi /Ob2 /I "../../rp_misc" /I "../../rp_method" /I "../../rp_ctrl" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /c
# ADD BASE RSC /l 0x409 /i "$(IntDir)" /d "NDEBUG"
# ADD RSC /l 0x409 /i "$(IntDir)" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Release\method_algorithm.lib"

!ENDIF 

# Begin Target

# Name "method_algorithm - Win32 Debug"
# Name "method_algorithm - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cc;cxx;def;odl;idl;hpj;bat;asm;asmx"
# Begin Source File

SOURCE=.\rdoprocess_method_algorithm.cpp
DEP_CPP_RDOPR=\
	"..\..\rp_ctrl\rdoprocess_toolbar.h"\
	"..\..\rp_method\rdoprocess_connector.h"\
	"..\..\rp_method\rdoprocess_factory.h"\
	"..\..\rp_method\rdoprocess_messages.h"\
	"..\..\rp_method\rdoprocess_method.h"\
	"..\..\rp_method\rdoprocess_object.h"\
	"..\..\rp_method\rdoprocess_object_chart.h"\
	"..\..\rp_method\rdoprocess_object_flowchart.h"\
	"..\..\rp_method\rdoprocess_object_matrix.h"\
	"..\..\rp_method\rdoprocess_project.h"\
	"..\..\rp_method\rdoprocess_shape.h"\
	"..\..\rp_misc\rdoprocess_math.h"\
	"..\..\rp_misc\rdoprocess_matrix.h"\
	"..\..\rp_misc\rdoprocess_point.h"\
	"..\..\rp_misc\rdoprocess_polyline.h"\
	"..\..\rp_misc\rdoprocess_rect.h"\
	"..\..\rp_misc\rdoprocess_string.h"\
	".\rdoprocess_method_algorithm.h"\
	".\rdoprocess_shape_action.h"\
	".\rdoprocess_shape_if.h"\
	".\res\block_action.xpm"\
	".\res\block_if.xpm"\
	".\stdafx.h"\
	
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape_action.cpp
DEP_CPP_RDOPRO=\
	"..\..\rp_method\rdoprocess_connector.h"\
	"..\..\rp_method\rdoprocess_factory.h"\
	"..\..\rp_method\rdoprocess_messages.h"\
	"..\..\rp_method\rdoprocess_object.h"\
	"..\..\rp_method\rdoprocess_object_chart.h"\
	"..\..\rp_method\rdoprocess_object_matrix.h"\
	"..\..\rp_method\rdoprocess_project.h"\
	"..\..\rp_method\rdoprocess_shape.h"\
	"..\..\rp_misc\rdoprocess_math.h"\
	"..\..\rp_misc\rdoprocess_matrix.h"\
	"..\..\rp_misc\rdoprocess_point.h"\
	"..\..\rp_misc\rdoprocess_polyline.h"\
	"..\..\rp_misc\rdoprocess_rect.h"\
	"..\..\rp_misc\rdoprocess_string.h"\
	".\rdoprocess_shape_action.h"\
	".\stdafx.h"\
	
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape_if.cpp
DEP_CPP_RDOPROC=\
	"..\..\rp_method\rdoprocess_connector.h"\
	"..\..\rp_method\rdoprocess_factory.h"\
	"..\..\rp_method\rdoprocess_flowchart.h"\
	"..\..\rp_method\rdoprocess_messages.h"\
	"..\..\rp_method\rdoprocess_object.h"\
	"..\..\rp_method\rdoprocess_object_chart.h"\
	"..\..\rp_method\rdoprocess_object_matrix.h"\
	"..\..\rp_method\rdoprocess_project.h"\
	"..\..\rp_method\rdoprocess_shape.h"\
	"..\..\rp_misc\rdoprocess_math.h"\
	"..\..\rp_misc\rdoprocess_matrix.h"\
	"..\..\rp_misc\rdoprocess_point.h"\
	"..\..\rp_misc\rdoprocess_polyline.h"\
	"..\..\rp_misc\rdoprocess_rect.h"\
	"..\..\rp_misc\rdoprocess_string.h"\
	".\rdoprocess_shape_if.h"\
	".\stdafx.h"\
	
# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
DEP_CPP_STDAF=\
	"..\..\rp_misc\rdoprocess_string.h"\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "method_algorithm - Win32 Debug"

# ADD CPP /nologo /GX /Yc"stdafx.h" /GZ

!ELSEIF  "$(CFG)" == "method_algorithm - Win32 Release"

# ADD CPP /nologo /GX /Yc"stdafx.h"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;inc;xsd"
# Begin Source File

SOURCE=.\rdoprocess_method_algorithm.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape_action.h
# End Source File
# Begin Source File

SOURCE=.\rdoprocess_shape_if.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "rc;ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe;resx;tiff;tif;png;wav"
# Begin Source File

SOURCE=.\res\block_action.xpm
# End Source File
# Begin Source File

SOURCE=.\res\block_if.xpm
# End Source File
# End Group
# End Target
# End Project
