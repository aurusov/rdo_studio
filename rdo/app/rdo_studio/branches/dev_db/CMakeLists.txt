#==============================================================================
# Copyright (c) 2013 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO  STUDIO  EXECUTABLE")

FIND_PACKAGE(Qt5Widgets REQUIRED)
INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})

FIND_PACKAGE(Boost COMPONENTS program_options thread REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${SCINTILLA_PATH}/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

CONFIGURE_FILE(${CMAKE_MODULE_PATH}/build.version.cmake ${CMAKE_CURRENT_SOURCE_DIR}/res/build_version.h)

SET(HEADER_FILES
    src/about.h
    src/action_activator/action_activator.h
    src/action_activator/action_activator_widget.h
    src/application.h
    src/dialog/file_association_dialog.h
    src/dialog/find_dialog.h
    src/dialog/find_replace_dialog.h
    src/dialog/goto_line_dialog.h
    src/dialog/new_model_dialog.h
    src/dock/dock_build.h
    src/dock/dock_chart_tree.h
    src/dock/dock_debug.h
    src/dock/dock_find.h
    src/dock/dock_focusable.h
    src/dock/dock_frame.h
    src/dock/dock_results.h
    src/dock/dock_trace.h
    src/editor/build_edit.h
    src/editor/build_edit_style.h
    src/editor/debug_edit.h
    src/editor/edit.h
    src/editor/edit_popup_menu.h
    src/editor/edit_style.h
    src/editor/find_edit.h
    src/editor/find_edit_style.h
    src/editor/log_edit.h
    src/editor/log_edit_style.h
    src/editor/model_edit.h
    src/editor/model_edit_style.h
    src/editor/parser_edit.h
    src/editor/parser_edit_style.h
    src/editor/results_edit.h
    src/editor/results_edit_style.h
    src/frame/frame_manager.h
    src/frame/frame_options_view.h
    src/frame/frame_style.h
    src/frame/frame_tree_ctrl.h
    src/frame/frame_view.h
    src/main_window.h
    src/main_window_base.h
    src/model/model.h
    src/model/model_tab_ctrl.h
    src/model/model_view.h
    src/status_bar.h
    src/style.h
    src/thread.h
    src/tracer/chart/chart_doc.h
    src/tracer/chart/chart_preferences.h
    src/tracer/chart/chart_serie.h
    src/tracer/chart/chart_tree.h
    src/tracer/chart/chart_tree_item.h
    src/tracer/chart/chart_view.h
    src/tracer/chart/chart_view_style.h
    src/tracer/logger/tracer_logger_main_wnd.h
    src/tracer/logger/tracer_logger_view.h
    src/tracer/logger/tracer_logger_view_style.h
    src/tracer/tracer.h
    src/tracer/tracer_base.h
    src/tracer/tracer_operation.h
    src/tracer/tracer_pattern.h
    src/tracer/tracer_resource.h
    src/tracer/tracer_resource_type.h
    src/tracer/tracer_result.h
    src/tracer/tracer_serie.h
    src/tracer/tracer_values.h
    src/view_preferences.h
    )

SET(HEADER_QT_FILES
    src/about.h
    src/dialog/file_association_dialog.h
    src/dialog/find_dialog.h
    src/dialog/find_replace_dialog.h
    src/dialog/goto_line_dialog.h
    src/dialog/new_model_dialog.h
    src/dock/dock_focusable.h
    src/editor/edit.h
    src/editor/log_edit.h
    src/editor/model_edit.h
    src/frame/frame_manager.h
    src/frame/frame_tree_ctrl.h
    src/frame/frame_view.h
    src/main_window.h
    src/model/model.h
    src/model/model_view.h
    src/tracer/chart/chart_preferences.h
    src/tracer/chart/chart_tree.h
    src/tracer/chart/chart_view.h
    src/tracer/logger/tracer_logger_view.h
    src/view_preferences.h
    )

SET(SOURCE_FILES
    src/about.cpp
    src/action_activator/action_activator.cpp
    src/action_activator/action_activator_widget.cpp
    src/application.cpp
    src/dialog/file_association_dialog.cpp
    src/dialog/find_dialog.cpp
    src/dialog/find_replace_dialog.cpp
    src/dialog/goto_line_dialog.cpp
    src/dialog/new_model_dialog.cpp
    src/dock/dock_build.cpp
    src/dock/dock_chart_tree.cpp
    src/dock/dock_debug.cpp
    src/dock/dock_find.cpp
    src/dock/dock_focusable.cpp
    src/dock/dock_frame.cpp
    src/dock/dock_results.cpp
    src/dock/dock_trace.cpp
    src/editor/build_edit.cpp
    src/editor/build_edit_style.cpp
    src/editor/debug_edit.cpp
    src/editor/edit.cpp
    src/editor/edit_popup_menu.cpp
    src/editor/edit_style.cpp
    src/editor/find_edit.cpp
    src/editor/find_edit_style.cpp
    src/editor/log_edit.cpp
    src/editor/log_edit_style.cpp
    src/editor/model_edit.cpp
    src/editor/model_edit_style.cpp
    src/editor/parser_edit.cpp
    src/editor/parser_edit_style.cpp
    src/editor/results_edit.cpp
    src/editor/results_edit_style.cpp
    src/frame/frame_manager.cpp
    src/frame/frame_options_view.cpp
    src/frame/frame_style.cpp
    src/frame/frame_tree_ctrl.cpp
    src/frame/frame_view.cpp
    src/main.cpp
    src/main_window.cpp
    src/model/model.cpp
    src/model/model_tab_ctrl.cpp
    src/model/model_view.cpp
    src/status_bar.cpp
    src/style.cpp
    src/thread.cpp
    src/tracer/chart/chart_doc.cpp
    src/tracer/chart/chart_preferences.cpp
    src/tracer/chart/chart_serie.cpp
    src/tracer/chart/chart_tree.cpp
    src/tracer/chart/chart_tree_item.cpp
    src/tracer/chart/chart_view.cpp
    src/tracer/chart/chart_view_style.cpp
    src/tracer/logger/tracer_logger_main_wnd.cpp
    src/tracer/logger/tracer_logger_view.cpp
    src/tracer/logger/tracer_logger_view_style.cpp
    src/tracer/tracer.cpp
    src/tracer/tracer_base.cpp
    src/tracer/tracer_operation.cpp
    src/tracer/tracer_pattern.cpp
    src/tracer/tracer_resource.cpp
    src/tracer/tracer_resource_type.cpp
    src/tracer/tracer_result.cpp
    src/tracer/tracer_serie.cpp
    src/tracer/tracer_values.cpp
    src/view_preferences.cpp
    )

SET(SCINTILLA_FILES
    res/scintilla/ac_data.xpm
    res/scintilla/ac_function.xpm
    res/scintilla/ac_trace.xpm
    )
    
SET(QT_FORMS
    res/ui/about.ui
    res/ui/chart_preferences.ui
    res/ui/file_association_dialog.ui
    res/ui/find_dialog.ui
    res/ui/find_replace_dialog.ui
    res/ui/frame_options_widget.ui
    res/ui/goto_line_dialog.ui
    res/ui/main_window.ui
    res/ui/new_model_dialog.ui
    res/ui/view_preferenses.ui
    )
    
SET(WIN32_RES
    res/win32/rdo_studio.ico
    res/win32/rdo_studio.rc
    )
    
SET(IMAGE_FILES
    res/images/about.png
    res/images/dock_build.png
    res/images/dock_chart.png
    res/images/dock_debug.png
    res/images/dock_find.png
    res/images/dock_frame.png
    res/images/dock_process.png
    res/images/dock_results.png
    res/images/dock_trace.png
    res/images/edit_copy.png
    res/images/edit_copy_d.png
    res/images/edit_cut.png
    res/images/edit_cut_d.png
    res/images/edit_paste.png
    res/images/edit_paste_d.png
    res/images/edit_redo.png
    res/images/edit_redo_d.png
    res/images/edit_undo.png
    res/images/edit_undo_d.png
    res/images/file_new.png
    res/images/file_new_d.png
    res/images/file_open.png
    res/images/file_open_d.png
    res/images/file_save.png
    res/images/file_save_all.png
    res/images/file_save_all_d.png
    res/images/file_save_d.png
    res/images/frameoptionsctrl.png
    res/images/help_context.png
    res/images/logo.png
    res/images/mdi_chart.png
    res/images/mdi_flowchart.png
    res/images/mdi_frame.png
    res/images/mdi_model.png
    res/images/model_frame_next.png
    res/images/model_frame_next_d.png
    res/images/model_frame_prev.png
    res/images/model_frame_prev_d.png
    res/images/model_run.png
    res/images/model_run_d.png
    res/images/model_runtime_jump.png
    res/images/model_runtime_jump_d.png
    res/images/model_runtime_max_speed.png
    res/images/model_runtime_max_speed_d.png
    res/images/model_runtime_pause.png
    res/images/model_runtime_pause_d.png
    res/images/model_runtime_sync.png
    res/images/model_runtime_sync_d.png
    res/images/model_show_rate_dec.png
    res/images/model_show_rate_dec_d.png
    res/images/model_show_rate_dec_four.png
    res/images/model_show_rate_dec_four_d.png
    res/images/model_show_rate_inc.png
    res/images/model_show_rate_inc_d.png
    res/images/model_show_rate_inc_four.png
    res/images/model_show_rate_inc_four_d.png
    res/images/model_stop.png
    res/images/model_stop_d.png
    res/images/search_bookmark_next.png
    res/images/search_bookmark_next_d.png
    res/images/search_bookmark_prev.png
    res/images/search_bookmark_prev_d.png
    res/images/search_find.png
    res/images/search_find_d.png
    res/images/search_replace.png
    res/images/search_replace_d.png
    res/images/tree_chart_erased.png
    res/images/tree_chart_root.png
    res/images/tree_chart_sub_root_1.png
    res/images/tree_chart_sub_root_2.png
    res/images/tree_chart_sub_root_3.png
    res/images/tree_chart_value.png
    res/images/tree_frame_item.png
    res/images/tree_frame_root.png
    res/images/view_zoom_auto.png
    res/images/view_zoom_auto_d.png
    res/images/view_zoom_dec.png
    res/images/view_zoom_dec_d.png
    res/images/view_zoom_inc.png
    res/images/view_zoom_inc_d.png
    res/images/view_zoom_reset.png
    res/images/view_zoom_reset_d.png
    )

QT5_WRAP_CPP(GENERATED_MOC_SOURCES ${HEADER_QT_FILES})
QT5_WRAP_UI (GENERATED_QT_FORMS_HEADERS ${QT_FORMS})

ADD_EXECUTABLE(rdo_studio
    ${HEADER_FILES}
    ${SOURCE_FILES}
    ${HEADER_QT_FILES}
    ${COMMON_FILES}
    ${SCRIPT_FILES}
    ${SCINTILLA_FILES}
    ${QT_FORMS}
    ${GENERATED_QT_FORMS_HEADERS}
    ${GENERATED_MOC_SOURCES}
    ${WIN32_RES}
    ${IMAGE_FILES}
    )

SET_TARGET_PROPERTIES(rdo_studio PROPERTIES FOLDER ${APPS_FOLDERS})

ADD_DEPENDENCIES(rdo_studio rdo_utils)
ADD_DEPENDENCIES(rdo_studio rdo_kernel)
ADD_DEPENDENCIES(rdo_studio rdo_repository)
ADD_DEPENDENCIES(rdo_studio rdo_simulator)
ADD_DEPENDENCIES(rdo_studio rdo_simulator_report)
ADD_DEPENDENCIES(rdo_studio rdo_ui_abstract)
ADD_DEPENDENCIES(rdo_studio rdo_ui_qt)

TARGET_LINK_LIBRARIES(rdo_studio ${Qt5Widgets_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_studio ${Qt5Core_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_studio ${Qt5Gui_LIBRARIES})

TARGET_LINK_LIBRARIES(rdo_studio ${Boost_PROGRAM_OPTIONS_LIBRARY})
TARGET_LINK_LIBRARIES(rdo_studio ${Boost_THREAD_LIBRARY})

TARGET_LINK_LIBRARIES(rdo_studio rdo_utils)
TARGET_LINK_LIBRARIES(rdo_studio rdo_kernel)
TARGET_LINK_LIBRARIES(rdo_studio rdo_repository)
TARGET_LINK_LIBRARIES(rdo_studio rdo_simulator)
TARGET_LINK_LIBRARIES(rdo_studio rdo_simulator_report)
TARGET_LINK_LIBRARIES(rdo_studio rdo_ui_abstract)
TARGET_LINK_LIBRARIES(rdo_studio rdo_ui_qt)

IF(MSVC) # lotions for windows #

SOURCE_GROUP(".code" FILES
    ${HEADER_FILES}
    ${HEADER_QT_FILES}
    ${GENERATED_MOC_SOURCES}
    ${SOURCE_FILES}
    )

SOURCE_GROUP(".scintilla" FILES
    ${SCINTILLA_FILES}
    )

SOURCE_GROUP(".qt_forms" FILES
    ${QT_FORMS}
    ${GENERATED_QT_FORMS_HEADERS}
    )

SOURCE_GROUP(".res" FILES
    ${WIN32_RES}
    )

SOURCE_GROUP(".image" FILES
    ${IMAGE_FILES}
    )

SOURCE_GROUP(".doc" FILES
    ${COMMON_FILES}
    )
    
SOURCE_GROUP(".script" FILES
    ${SCRIPT_FILES}
    )

ENDIF(MSVC)
