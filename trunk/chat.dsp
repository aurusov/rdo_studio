# Microsoft Developer Studio Project File - Name="chat" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=chat - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "chat.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "chat.mak" CFG="chat - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "chat - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "chat - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "chat - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /I ".\rdo_ctrls" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib /nologo /subsystem:windows /machine:I386 /out:"Release/localchat.exe"
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "chat - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".\rdo_ctrls" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/localchat.exe"
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "chat - Win32 Release"
# Name "chat - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\chatapp.cpp
# End Source File
# Begin Source File

SOURCE=.\chatchildview.cpp
# End Source File
# Begin Source File

SOURCE=.\chatdockwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\chatedit.cpp
# End Source File
# Begin Source File

SOURCE=.\chatlistboxctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\chatmainfrm.cpp
# End Source File
# Begin Source File

SOURCE=.\chatoptions.cpp
# End Source File
# Begin Source File

SOURCE=.\chatpopupmessage.cpp
# End Source File
# Begin Source File

SOURCE=.\chatsound.cpp
# End Source File
# Begin Source File

SOURCE=.\chatstatusmode.cpp
# End Source File
# Begin Source File

SOURCE=.\chatudp.cpp
# End Source File
# Begin Source File

SOURCE=.\chatusers.cpp
# End Source File
# Begin Source File

SOURCE=.\chatuserstreectrl.cpp
# End Source File
# Begin Source File

SOURCE=.\chatviewer.cpp
# End Source File
# Begin Source File

SOURCE=.\chatviewerstring.cpp
# End Source File
# Begin Source File

SOURCE=.\chatviewerstyle.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\chatapp.h
# End Source File
# Begin Source File

SOURCE=.\chatchildview.h
# End Source File
# Begin Source File

SOURCE=.\chatdockwnd.h
# End Source File
# Begin Source File

SOURCE=.\chatedit.h
# End Source File
# Begin Source File

SOURCE=.\chatlistboxctrl.h
# End Source File
# Begin Source File

SOURCE=.\chatmainfrm.h
# End Source File
# Begin Source File

SOURCE=.\chatoptions.h
# End Source File
# Begin Source File

SOURCE=.\chatpopupmessage.h
# End Source File
# Begin Source File

SOURCE=.\chatsound.h
# End Source File
# Begin Source File

SOURCE=.\chatstatusmode.h
# End Source File
# Begin Source File

SOURCE=.\chatudp.h
# End Source File
# Begin Source File

SOURCE=.\chatusers.h
# End Source File
# Begin Source File

SOURCE=.\chatuserstreectrl.h
# End Source File
# Begin Source File

SOURCE=.\chatviewer.h
# End Source File
# Begin Source File

SOURCE=.\chatviewerstring.h
# End Source File
# Begin Source File

SOURCE=.\chatviewerstyle.h
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

SOURCE=.\res\changestatus.wav
# End Source File
# Begin Source File

SOURCE=.\res\chat.ico
# End Source File
# Begin Source File

SOURCE=.\chat.rc
# End Source File
# Begin Source File

SOURCE=.\res\chatback.wav
# End Source File
# Begin Source File

SOURCE=.\res\chatret.wav
# End Source File
# Begin Source File

SOURCE=.\res\chattype.wav
# End Source File
# Begin Source File

SOURCE=.\res\connect.wav
# End Source File
# Begin Source File

SOURCE=.\res\disconnect.wav
# End Source File
# Begin Source File

SOURCE=.\res\incoming.wav
# End Source File
# Begin Source File

SOURCE=.\res\sound.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tray_app.ico
# End Source File
# Begin Source File

SOURCE=.\res\tray_message.ico
# End Source File
# Begin Source File

SOURCE=.\res\userstreectrl.bmp
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
