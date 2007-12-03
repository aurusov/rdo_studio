# Microsoft Developer Studio Project File - Name="rdo_help_studio_rus" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=rdo_help_studio_rus - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rdo_studio_rus.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rdo_studio_rus.mak" CFG="rdo_help_studio_rus - Win32 Release"
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

SOURCE=.\html\rdo_whats_new.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_help.htm
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

SOURCE=.\html\work_model_chart.htm
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

SOURCE=.\html\work_model_search_replace.htm
# End Source File
# Begin Source File

SOURCE=.\html\work_model_tabsheets.htm
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

SOURCE=.\html\work_options_plugins.htm
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
# End Group
# Begin Group "Images Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\images\bmstu.gif
# End Source File
# Begin Source File

SOURCE=.\images\bookmark_margin.png
# End Source File
# Begin Source File

SOURCE=.\images\bookmark_no_margin.gif
# End Source File
# Begin Source File

SOURCE=.\images\chart.png
# End Source File
# Begin Source File

SOURCE=.\images\chart_main_view1.png
# End Source File
# Begin Source File

SOURCE=.\images\chart_main_view2.png
# End Source File
# Begin Source File

SOURCE=.\images\chart_main_view3.png
# End Source File
# Begin Source File

SOURCE=.\images\chart_option_chart.png
# End Source File
# Begin Source File

SOURCE=.\images\chart_option_series.png
# End Source File
# Begin Source File

SOURCE=.\images\context_help.gif
# End Source File
# Begin Source File

SOURCE=.\images\edit_case_lower.png
# End Source File
# Begin Source File

SOURCE=.\images\edit_case_normal.png
# End Source File
# Begin Source File

SOURCE=.\images\edit_case_upper.png
# End Source File
# Begin Source File

SOURCE=.\images\edit_comment1.png
# End Source File
# Begin Source File

SOURCE=.\images\edit_comment2.png
# End Source File
# Begin Source File

SOURCE=.\images\edit_copy.gif
# End Source File
# Begin Source File

SOURCE=.\images\edit_copy_as_rtf1.png
# End Source File
# Begin Source File

SOURCE=.\images\edit_copy_as_rtf2.png
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

SOURCE=.\images\edit_statusbar.gif
# End Source File
# Begin Source File

SOURCE=.\images\edit_undo.gif
# End Source File
# Begin Source File

SOURCE=.\images\example_border.psd
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

SOURCE=.\images\file_save_all.gif
# End Source File
# Begin Source File

SOURCE=.\images\find_in_model_dlg.png
# End Source File
# Begin Source File

SOURCE=.\images\find_in_model_results.png
# End Source File
# Begin Source File

SOURCE=.\images\fold_all.gif
# End Source File
# Begin Source File

SOURCE=.\images\fold_one.gif
# End Source File
# Begin Source File

SOURCE=.\images\frame.png
# End Source File
# Begin Source File

SOURCE=.\images\frame_list.png
# End Source File
# Begin Source File

SOURCE=.\images\frame_next.gif
# End Source File
# Begin Source File

SOURCE=.\images\frame_prev.gif
# End Source File
# Begin Source File

SOURCE=.\images\help_contents1.png
# End Source File
# Begin Source File

SOURCE=.\images\help_contents2.png
# End Source File
# Begin Source File

SOURCE=.\images\help_contents_options1.png
# End Source File
# Begin Source File

SOURCE=.\images\help_contents_options2.png
# End Source File
# Begin Source File

SOURCE=.\images\insert_buffer_edit.png
# End Source File
# Begin Source File

SOURCE=.\images\insert_buffer_menu.gif
# End Source File
# Begin Source File

SOURCE=.\images\insert_template1.gif
# End Source File
# Begin Source File

SOURCE=.\images\insert_template2.png
# End Source File
# Begin Source File

SOURCE=.\images\main_win.gif
# End Source File
# Begin Source File

SOURCE=.\images\main_win.png
# End Source File
# Begin Source File

SOURCE=.\images\menu.gif
# End Source File
# Begin Source File

SOURCE=.\images\menu_chart.gif
# End Source File
# Begin Source File

SOURCE=.\images\menu_edit.gif
# End Source File
# Begin Source File

SOURCE=.\images\menu_file.gif
# End Source File
# Begin Source File

SOURCE=.\images\menu_help.gif
# End Source File
# Begin Source File

SOURCE=.\images\menu_insert.gif
# End Source File
# Begin Source File

SOURCE=.\images\menu_model.gif
# End Source File
# Begin Source File

SOURCE=.\images\menu_search.gif
# End Source File
# Begin Source File

SOURCE=.\images\menu_view.gif
# End Source File
# Begin Source File

SOURCE=.\images\menu_windows.gif
# End Source File
# Begin Source File

SOURCE=.\images\model_compiled.png
# End Source File
# Begin Source File

SOURCE=.\images\model_compiled_error.png
# End Source File
# Begin Source File

SOURCE=.\images\model_compiled_warning1.png
# End Source File
# Begin Source File

SOURCE=.\images\model_compiled_warning2.png
# End Source File
# Begin Source File

SOURCE=.\images\model_open1.png
# End Source File
# Begin Source File

SOURCE=.\images\model_open2.png
# End Source File
# Begin Source File

SOURCE=.\images\model_open_error.png
# End Source File
# Begin Source File

SOURCE=.\images\model_reopen.gif
# End Source File
# Begin Source File

SOURCE=.\images\model_run.gif
# End Source File
# Begin Source File

SOURCE=.\images\model_save.png
# End Source File
# Begin Source File

SOURCE=.\images\model_stop.gif
# End Source File
# Begin Source File

SOURCE=.\images\model_tabsheets.png
# End Source File
# Begin Source File

SOURCE=.\images\model_wizard.png
# End Source File
# Begin Source File

SOURCE=.\images\model_wizard_directory.gif
# End Source File
# Begin Source File

SOURCE=.\images\model_wizard_directory.png
# End Source File
# Begin Source File

SOURCE=.\images\model_wizard_sample.png
# End Source File
# Begin Source File

SOURCE=.\images\options_editor.png
# End Source File
# Begin Source File

SOURCE=.\images\options_editor_autocomp_full.png
# End Source File
# Begin Source File

SOURCE=.\images\options_editor_autocomp_nearest.png
# End Source File
# Begin Source File

SOURCE=.\images\options_editor_bookmark_off.png
# End Source File
# Begin Source File

SOURCE=.\images\options_editor_bookmark_on.png
# End Source File
# Begin Source File

SOURCE=.\images\options_editor_fold_off.png
# End Source File
# Begin Source File

SOURCE=.\images\options_editor_fold_on.png
# End Source File
# Begin Source File

SOURCE=.\images\options_editor_linenumber_on.png
# End Source File
# Begin Source File

SOURCE=.\images\options_general.png
# End Source File
# Begin Source File

SOURCE=.\images\options_plugins.png
# End Source File
# Begin Source File

SOURCE=.\images\options_styles_and_color.png
# End Source File
# Begin Source File

SOURCE=.\images\options_tabs.png
# End Source File
# Begin Source File

SOURCE=.\images\panel_output_compiler.gif
# End Source File
# Begin Source File

SOURCE=.\images\panel_output_debug.gif
# End Source File
# Begin Source File

SOURCE=.\images\panel_output_find.gif
# End Source File
# Begin Source File

SOURCE=.\images\panel_output_results.gif
# End Source File
# Begin Source File

SOURCE=.\images\panel_output_trace.gif
# End Source File
# Begin Source File

SOURCE=.\images\panel_workspace_chart.png
# End Source File
# Begin Source File

SOURCE=.\images\panel_workspace_frame.png
# End Source File
# Begin Source File

SOURCE=.\images\progon_jump.gif
# End Source File
# Begin Source File

SOURCE=.\images\progon_maxspeed.gif
# End Source File
# Begin Source File

SOURCE=.\images\progon_pause.gif
# End Source File
# Begin Source File

SOURCE=.\images\progon_speed.gif
# End Source File
# Begin Source File

SOURCE=.\images\progon_statusbar.png
# End Source File
# Begin Source File

SOURCE=.\images\progon_sync.gif
# End Source File
# Begin Source File

SOURCE=.\images\results.png
# End Source File
# Begin Source File

SOURCE=.\images\search_find.gif
# End Source File
# Begin Source File

SOURCE=.\images\search_find_dlg.png
# End Source File
# Begin Source File

SOURCE=.\images\search_next_bookmark.gif
# End Source File
# Begin Source File

SOURCE=.\images\search_previous_bookmark.gif
# End Source File
# Begin Source File

SOURCE=.\images\search_replace.gif
# End Source File
# Begin Source File

SOURCE=.\images\search_replace_dlg.png
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
# Begin Source File

SOURCE=.\images\trace.png
# End Source File
# Begin Source File

SOURCE=.\images\view_endofline.gif
# End Source File
# Begin Source File

SOURCE=.\images\view_panel_object.gif
# End Source File
# Begin Source File

SOURCE=.\images\view_panel_output.gif
# End Source File
# Begin Source File

SOURCE=.\images\view_whitespace.gif
# End Source File
# Begin Source File

SOURCE=.\images\view_zoom_auto.gif
# End Source File
# Begin Source File

SOURCE=.\images\view_zoom_in.gif
# End Source File
# Begin Source File

SOURCE=.\images\view_zoom_out.gif
# End Source File
# Begin Source File

SOURCE=.\images\view_zoom_reset.gif
# End Source File
# End Group
# Begin Group "Scripts"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\shared\scripts\footer_raostudio.js
# End Source File
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
