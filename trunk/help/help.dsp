# Microsoft Developer Studio Project File - Name="help" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=help - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "help.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "help.mak" CFG="help - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "help - Win32 Release" (based on "Win32 (x86) Generic Project")
!MESSAGE "help - Win32 Debug" (based on "Win32 (x86) Generic Project")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
MTL=midl.exe

!IF  "$(CFG)" == "help - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "help - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "help - Win32 Release"
# Name "help - Win32 Debug"
# Begin Group "Source File"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\localchat.hhp

!IF  "$(CFG)" == "help - Win32 Release"

# Begin Custom Build
WkspDir=.
InputPath=.\localchat.hhp

"localchat.chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(WkspDir)\help\tools\hhc.bat $(WkspDir)\help\tools\ $(WkspDir)\help$(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "help - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
WkspDir=.
InputPath=.\localchat.hhp

"localchat.chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(WkspDir)\help\tools\hhc.bat $(WkspDir)\help\tools\ $(WkspDir)\help$(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "HTML File"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\html\forum.htm
# End Source File
# End Group
# Begin Group "Style File"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\styles\styles.css
# End Source File
# End Group
# End Target
# End Project
