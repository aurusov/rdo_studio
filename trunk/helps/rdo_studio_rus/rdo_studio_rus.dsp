# Microsoft Developer Studio Project File - Name="rdo_studio_rus" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=rdo_studio_rus - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rdo_studio_rus.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rdo_studio_rus.mak" CFG="rdo_studio_rus - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rdo_studio_rus - Win32 Release" (based on "Win32 (x86) Generic Project")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
MTL=midl.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
# PROP Target_Dir ""
# Begin Target

# Name "rdo_studio_rus - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rdo_studio.hhp
# PROP Ignore_Default_Tool 1
# Begin Custom Build
WkspDir=.
InputPath=.\rdo_studio.hhp

"RAO-studio.chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\help_tools\Hhc.bat $(WkspDir) $(InputPath)

# End Custom Build
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\aliases.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\dummy.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\resource.hm
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Contents Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rdo_studio.hhc
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Index Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rdo_studio.hhk
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\rdo_studio.stp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "HTML Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\html\about.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\intro.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_lang_help.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_exit.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_help.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_install.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_intro.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_menu_chart.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_menu_edit.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_menu_file.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_menu_help.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_menu_insert.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_menu_intro.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_menu_model.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_menu_search.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_menu_view.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_menu_window.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_model_bookmarks.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_model_buffer.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_model_complete.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_model_creation.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_model_edit.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_model_folds.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_model_insert.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_model_intro.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_model_open.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_model_results.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_model_run.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_model_save.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_model_scale.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_model_search_replace.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_model_tabsheets.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_model_view.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_options.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_run.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\work_status_bar.htm
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Images Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\images\bmstu.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\bookmark_margin.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\bookmark_no_margin.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\code_completion.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\context_help.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\edit_copy.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\edit_cut.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\edit_paste.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\edit_redo.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\edit_undo.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\exit.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\file_new.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\file_open.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\file_save.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\fold.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\insert_buffer_edit.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\main_win.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\next_bookmark.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\shared\images\next_section.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\shared\images\next_section_d.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\shared\images\next_topic.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\shared\images\next_topic_d.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\options_color.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\options_edit.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\shared\images\prev_section.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\shared\images\prev_section_d.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\shared\images\prev_topic.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\shared\images\prev_topic_d.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\previous_bookmark.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\rdo_path.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\rdo_reload_pmv.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\rdo_reload_trc.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\rdo_run.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\rdo_stop.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\search_find.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\search_find_dlg.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\search_replace.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\images\search_replace_dlg.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\shared\images\section_top.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\shared\images\section_top_d.gif
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Scripts"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\shared\scripts\navbar.js
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\sections.js
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Styles"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\shared\styles\styles.css
# PROP Exclude_From_Build 1
# End Source File
# End Group
# End Target
# End Project
