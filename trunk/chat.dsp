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
# ADD LINK32 winmm.lib htmlhelp.lib Version.lib /nologo /subsystem:windows /machine:I386 /out:"Release/localchat.exe"
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
# ADD LINK32 winmm.lib htmlhelp.lib Version.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/localchat.exe"
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "chat - Win32 Release"
# Name "chat - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\chatabout.cpp
# End Source File
# Begin Source File

SOURCE=.\chatapp.cpp
# End Source File
# Begin Source File

SOURCE=.\chatchildview.cpp
# End Source File
# Begin Source File

SOURCE=.\chatdock.cpp
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

SOURCE=.\chatnetwork.cpp
# End Source File
# Begin Source File

SOURCE=.\chatnetworkctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\chatoptions.cpp
# End Source File
# Begin Source File

SOURCE=.\chatpopupmessage.cpp
# End Source File
# Begin Source File

SOURCE=.\chatsmile.cpp
# End Source File
# Begin Source File

SOURCE=.\chatsmilelistctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\chatsound.cpp
# End Source File
# Begin Source File

SOURCE=.\chatsplash.cpp
# End Source File
# Begin Source File

SOURCE=.\chatstatusmode.cpp
# End Source File
# Begin Source File

SOURCE=.\chatudp.cpp
# End Source File
# Begin Source File

SOURCE=.\chatuserlist.cpp
# End Source File
# Begin Source File

SOURCE=.\chatuserlistctrl.cpp
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

SOURCE=.\chatabout.h
# End Source File
# Begin Source File

SOURCE=.\chatapp.h
# End Source File
# Begin Source File

SOURCE=.\chatchildview.h
# End Source File
# Begin Source File

SOURCE=.\chatdock.h
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

SOURCE=.\chatnetwork.h
# End Source File
# Begin Source File

SOURCE=.\chatnetworkctrl.h
# End Source File
# Begin Source File

SOURCE=.\chatoptions.h
# End Source File
# Begin Source File

SOURCE=.\chatpopupmessage.h
# End Source File
# Begin Source File

SOURCE=.\chatsmile.h
# End Source File
# Begin Source File

SOURCE=.\chatsmilelistctrl.h
# End Source File
# Begin Source File

SOURCE=.\chatsound.h
# End Source File
# Begin Source File

SOURCE=.\chatsplash.h
# End Source File
# Begin Source File

SOURCE=.\chatstatusmode.h
# End Source File
# Begin Source File

SOURCE=.\chatudp.h
# End Source File
# Begin Source File

SOURCE=.\chatuserlist.h
# End Source File
# Begin Source File

SOURCE=.\chatuserlistctrl.h
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

SOURCE=.\HTMLHELP.H
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
# Begin Group "NetSearch"

# PROP Default_Filter ""
# Begin Group "NetSearch Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\netsearch\Network.cpp
# End Source File
# End Group
# Begin Group "NetSearch Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\netsearch\Network.h
# End Source File
# End Group
# End Group
# Begin Group "PictureEx"

# PROP Default_Filter ""
# Begin Group "PictureEx Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PictureEx\PictureEx.cpp
# End Source File
# End Group
# Begin Group "PictureEx Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PictureEx\PictureEx.h
# End Source File
# End Group
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

SOURCE=.\res\dock.bmp
# End Source File
# Begin Source File

SOURCE=.\res\edit_too.bmp
# End Source File
# Begin Source File

SOURCE=.\res\gif1.bin
# End Source File
# Begin Source File

SOURCE=.\res\incoming.wav
# End Source File
# Begin Source File

SOURCE=.\res\mainToolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mainToolbar_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\net.bmp
# End Source File
# Begin Source File

SOURCE=.\res\popupmsg.wav
# End Source File
# Begin Source File

SOURCE=.\res\privatemsg.wav
# End Source File
# Begin Source File

SOURCE=.\res\smile_amazed.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_ballet.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_beer.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_beer_1.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_beer_2.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_biggrin.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_bis.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_box.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_confused.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_cool.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_crazy.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_down.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_drink.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_duel.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_eek.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_eyes.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_fart.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_fear.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_figa.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_friday.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_frown.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_gigi.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_gossip.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_hah.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_hit.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_idea.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_insane.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_invalid.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_ivan.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_joke.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_kos.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_kult.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_lamer.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_laugh.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_lol.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_mad.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_mobile.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_no.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_puke.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_redface.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_rolleyes.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_rotate.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_shuffle.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_smile.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_smirk.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_smirk2.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_spy.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_super.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_think.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_tomato.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_tongue.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_up.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_upset.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_venera.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_weep.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_wht.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_wink.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_wink2.gif
# End Source File
# Begin Source File

SOURCE=.\res\smile_yes.gif
# End Source File
# Begin Source File

SOURCE=.\res\sound.bmp
# End Source File
# Begin Source File

SOURCE=.\Splsh16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\statusModeToolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\statusModeToolbar_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_edit.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_edit_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_main.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_main_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_statusMode.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_statusMode_d.bmp
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
# End Target
# End Project
