# Microsoft Developer Studio Project File - Name="rdo_help_lang_rus" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=rdo_help_lang_rus - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rdo_lang_rus.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rdo_lang_rus.mak" CFG="rdo_help_lang_rus - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rdo_help_lang_rus - Win32 Release" (based on "Win32 (x86) Generic Project")
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

# Name "rdo_help_lang_rus - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "hhp"
# Begin Source File

SOURCE=.\rdo_lang.hhp
# PROP Ignore_Default_Tool 1
# Begin Custom Build
WkspDir=.
InputPath=.\rdo_lang.hhp

"RAO-language.chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	..\help_tools\Hhc.bat $(WkspDir) $(InputPath)

# End Custom Build
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Contents Files"

# PROP Default_Filter "hhc"
# Begin Source File

SOURCE=.\rdo_lang.hhc
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Index Files"

# PROP Default_Filter "hhk;stp"
# Begin Source File

SOURCE=.\rdo_lang.hhk
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\rdo_lang.stp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "HTML Files"

# PROP Default_Filter "htm;html"
# Begin Source File

SOURCE=.\html\rdo_base_alpha.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_base_equations.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_base_intro.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_base_res_types.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_base_resources.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_base_std_var_func.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_base_types.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_intro.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_lex_div_intro.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_lex_div_names.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_lex_div_numeric_const.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_lex_div_res_words.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_lex_div_spesial_symbols.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_lex_div_string_const.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_obj_const_fun_sec.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_obj_dpt.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_obj_frm.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_obj_fun.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_obj_intro.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_obj_opr.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_obj_pat.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_obj_pmd.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_obj_sec.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_obj_smr.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_res_intro.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_res_pmv.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_res_trc.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_res_trc_decisionpt.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_res_trc_events.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_res_trc_resourses.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_res_trc_results.htm
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\html\rdo_terms.htm
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Images Files"

# PROP Default_Filter "bmp;gif;jpg;jpeg;jpe"
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

SOURCE=..\shared\images\section_top.gif
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\shared\images\section_top_d.gif
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Scripts"

# PROP Default_Filter "js"
# Begin Source File

SOURCE=..\shared\scripts\navbar.js
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Styles"

# PROP Default_Filter "css"
# Begin Source File

SOURCE=..\shared\styles\styles.css
# PROP Exclude_From_Build 1
# End Source File
# End Group
# End Target
# End Project
