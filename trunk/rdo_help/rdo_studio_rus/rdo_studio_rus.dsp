# Microsoft Developer Studio Project File - Name="rdo_help_studio_rus" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=rdo_help_studio_rus - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rdo_help_studio_rus.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rdo_help_studio_rus.mak" CFG="rdo_help_studio_rus - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rdo_help_studio_rus - Win32 Release" (based on "Win32 (x86) Generic Project")
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

# Name "rdo_help_studio_rus - Win32 Release"
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
# End Source File
# Begin Source File

SOURCE=.\html\intro.htm
# End Source File
# Begin Source File

SOURCE=.\html\rdo_lang_help.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_exit.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_help.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_install.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_intro.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_menu_chart.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_menu_edit.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_menu_file.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_menu_help.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_menu_insert.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_menu_intro.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_menu_model.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_menu_search.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_menu_view.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_menu_window.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_model_bookmarks.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_model_buffer.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_model_chart_main.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_model_chart_opt_c.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_model_chart_opt_s.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_model_complete.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_model_creation.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_model_edit.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_model_find_in_model.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_model_folds.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_model_frame.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_model_insert.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_model_intro.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_model_open.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_model_results.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_model_run.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_model_save.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_model_scale.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_model_search_replace.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_model_tabsheets.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_model_view.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_options.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_options_editor.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_options_general.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_options_styles_and_color.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_options_tabs.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_run.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_status_bar.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_windows.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_windows_output.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_windows_workspace.htm
# End Source File
# End Group
# Begin Group "Images Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\images\bmstu.gif
# End Source File
# Begin Source File

SOURCE=.\images\bookmark_margin.gif
# End Source File
# Begin Source File

SOURCE=.\images\bookmark_no_margin.gif
# End Source File
# Begin Source File

SOURCE=.\images\chart_main_view.gif
# End Source File
# Begin Source File

SOURCE=.\images\chart_window_chart.gif
# End Source File
# Begin Source File

SOURCE=.\images\chart_window_series.gif
# End Source File
# Begin Source File

SOURCE=.\images\code_completion.gif
# End Source File
# Begin Source File

SOURCE=.\images\context_help.gif
# End Source File
# Begin Source File

SOURCE=.\images\edit_copy.gif
# End Source File
# Begin Source File

SOURCE=.\images\edit_cut.gif
# End Source File
# Begin Source File

SOURCE=.\images\edit_paste.gif
# End Source File
# Begin Source File

SOURCE=.\images\edit_redo.gif
# End Source File
# Begin Source File

SOURCE=.\images\edit_undo.gif
# End Source File
# Begin Source File

SOURCE=.\images\exit.gif
# End Source File
# Begin Source File

SOURCE=.\images\file_new.gif
# End Source File
# Begin Source File

SOURCE=.\images\file_open.gif
# End Source File
# Begin Source File

SOURCE=.\images\file_save.gif
# End Source File
# Begin Source File

SOURCE=.\images\find_in_model_dlg.gif
# End Source File
# Begin Source File

SOURCE=.\images\find_in_model_results.gif
# End Source File
# Begin Source File

SOURCE=.\images\fold.gif
# End Source File
# Begin Source File

SOURCE=.\images\frame.gif
# End Source File
# Begin Source File

SOURCE=.\images\frame_next.gif
# End Source File
# Begin Source File

SOURCE=.\images\frame_prev.gif
# End Source File
# Begin Source File

SOURCE=.\images\insert_buffer_edit.gif
# End Source File
# Begin Source File

SOURCE=.\images\main_win.gif
# End Source File
# Begin Source File

SOURCE=.\images\next_bookmark.gif
# End Source File
# Begin Source File

SOURCE=.\images\options_editor.gif
# End Source File
# Begin Source File

SOURCE=.\images\options_general.gif
# End Source File
# Begin Source File

SOURCE=.\images\options_styles_and_color.gif
# End Source File
# Begin Source File

SOURCE=.\images\options_tabs.gif
# End Source File
# Begin Source File

SOURCE=.\images\previous_bookmark.gif
# End Source File
# Begin Source File

SOURCE=.\images\progon_animation.gif
# End Source File
# Begin Source File

SOURCE=.\images\progon_monitor.gif
# End Source File
# Begin Source File

SOURCE=.\images\progon_noshow.gif
# End Source File
# Begin Source File

SOURCE=.\images\rdo_run.gif
# End Source File
# Begin Source File

SOURCE=.\images\rdo_stop.gif
# End Source File
# Begin Source File

SOURCE=.\images\search_find.gif
# End Source File
# Begin Source File

SOURCE=.\images\search_find_dlg.gif
# End Source File
# Begin Source File

SOURCE=.\images\search_replace.gif
# End Source File
# Begin Source File

SOURCE=.\images\search_replace_dlg.gif
# End Source File
# Begin Source File

SOURCE=.\images\tempo_minus.gif
# End Source File
# Begin Source File

SOURCE=.\images\tempo_minus4.gif
# End Source File
# Begin Source File

SOURCE=.\images\tempo_plus.gif
# End Source File
# Begin Source File

SOURCE=.\images\tempo_plus4.gif
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
