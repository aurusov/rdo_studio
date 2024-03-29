#==============================================================================
# Copyright (c) 2013 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

ADD_SUBDIRECTORY(help)

MESSAGE(STATUS "CREATE  RDO  STUDIO  EXECUTABLE")

FIND_PACKAGE(Qt5Widgets REQUIRED)
INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})

FIND_PACKAGE(Boost COMPONENTS program_options thread chrono system filesystem regex REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${SCINTILLA_PATH}/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

ADD_SUBDIRECTORY(plugins)

SET(MAIN_FILES
    src/main.cpp
)

SET(STUDIO_PCH_FILES
    pch/stdpch.cpp
    pch/stdpch.h
)

SET(APPLICATION_FILES
    src/application.cpp
    src/application.h
    src/shared_object_service.h
    src/main_window.cpp
    src/main_window.h
    src/main_window_base.h
    src/status_bar.cpp
    src/status_bar.h
    src/style.cpp
    src/style.h
    src/view_preferences.cpp
    src/view_preferences.h
)

SET(APPLICATION_ACTIONACTIVATOR_FILES
    src/action_activator/action_activator.cpp
    src/action_activator/action_activator.h
    src/action_activator/action_activator_widget.cpp
    src/action_activator/action_activator_widget.h
)

SET(DIALOG_FILES
    src/dialog/about_dialog.cpp
    src/dialog/about_dialog.h
    src/dialog/file_association_dialog.cpp
    src/dialog/file_association_dialog.h
    src/dialog/find_dialog.cpp
    src/dialog/find_dialog.h
    src/dialog/find_replace_dialog.cpp
    src/dialog/find_replace_dialog.h
    src/dialog/goto_line_dialog.cpp
    src/dialog/goto_line_dialog.h
    src/dialog/new_model_dialog.cpp
    src/dialog/new_model_dialog.h
)

SET(DOCK_FILES
    src/dock/dock_build.cpp
    src/dock/dock_build.h
    src/dock/dock_chart_tree.cpp
    src/dock/dock_chart_tree.h
    src/dock/dock_debug.cpp
    src/dock/dock_debug.h
    src/dock/dock_find.cpp
    src/dock/dock_find.h
    src/dock/dock_focusable.cpp
    src/dock/dock_focusable.h
    src/dock/dock_frame.cpp
    src/dock/dock_frame.h
    src/dock/dock_results.cpp
    src/dock/dock_results.h
    src/dock/dock_trace.cpp
    src/dock/dock_trace.h
)

SET(APPLICATION_PCH_FILES
    pch/application_pch.cpp
    pch/application_pch.h
)

SET(EDITOR_FILES
    src/editor/edit.cpp
    src/editor/edit.h
    src/editor/edit_popup_menu.cpp
    src/editor/edit_popup_menu.h
    src/editor/edit_style.cpp
    src/editor/edit_style.h
)

SET(EDITOR_DEBUG_FILES
    src/editor/debug_edit.cpp
    src/editor/debug_edit.h
)

SET(EDITOR_LEXER_FILES
    src/editor/lexer/lexer_find.cpp
    src/editor/lexer/lexer_find.h
    src/editor/lexer/lexer_model.cpp
    src/editor/lexer/lexer_model.h
    src/editor/lexer/word_list_util.cpp
    src/editor/lexer/word_list_util.h
)

SET(EDITOR_LOG_FILES
    src/editor/log_edit.cpp
    src/editor/log_edit.h
    src/editor/log_edit_style.cpp
    src/editor/log_edit_style.h
)

SET(EDITOR_LOG_BUILD_FILES
    src/editor/build_edit.cpp
    src/editor/build_edit.h
    src/editor/build_edit_style.cpp
    src/editor/build_edit_style.h
)

SET(EDITOR_LOG_FIND_FILES
    src/editor/find_edit.cpp
    src/editor/find_edit.h
    src/editor/find_edit_style.cpp
    src/editor/find_edit_style.h
)

SET(EDITOR_MODEL_FILES
    src/editor/model_edit.cpp
    src/editor/model_edit.h
    src/editor/model_edit_style.cpp
    src/editor/model_edit_style.h
)

SET(EDITOR_PARSER_FILES
    src/editor/parser_edit.cpp
    src/editor/parser_edit.h
    src/editor/parser_edit_style.cpp
    src/editor/parser_edit_style.h
)

SET(EDITOR_RESULTS_FILES
    src/editor/results_edit.cpp
    src/editor/results_edit.h
    src/editor/results_edit_style.cpp
    src/editor/results_edit_style.h
)

SET(EDITOR_PCH_FILES
    pch/editor_pch.cpp
    pch/editor_pch.h
)

SET(FRAME_FILES
    src/frame/frame_manager.cpp
    src/frame/frame_manager.h
    src/frame/frame_options_view.cpp
    src/frame/frame_options_view.h
    src/frame/frame_style.cpp
    src/frame/frame_style.h
    src/frame/frame_tree_ctrl.cpp
    src/frame/frame_tree_ctrl.h
    src/frame/frame_view.cpp
    src/frame/frame_view.h
)

SET(FRAME_PCH_FILES
    pch/frame_pch.cpp
    pch/frame_pch.h
)

SET(MODEL_FILES
    src/model/model.cpp
    src/model/model.h
    src/model/model_tab_ctrl.cpp
    src/model/model_tab_ctrl.h
    src/model/model_view.cpp
    src/model/model_view.h
)

SET(MODEL_PCH_FILES
    pch/model_pch.cpp
    pch/model_pch.h
)

SET(PLUGINS_FILES
    src/plugins/plugin_info.cpp
    src/plugins/plugin_info.h
    src/plugins/plugin_interface.h
    src/plugins/plugin_loader.cpp
    src/plugins/plugin_loader.h
)

SET(PLUGIN_LOADER_PCH_FILES
    pch/plugin_loader_pch.cpp
    pch/plugin_loader_pch.h
)

SET(TRACER_FILES
    src/tracer/tracer.cpp
    src/tracer/tracer.h
    src/tracer/tracer_base.cpp
    src/tracer/tracer_base.h
    src/tracer/tracer_operation.cpp
    src/tracer/tracer_operation.h
    src/tracer/tracer_pattern.cpp
    src/tracer/tracer_pattern.h
    src/tracer/tracer_resource.cpp
    src/tracer/tracer_resource.h
    src/tracer/tracer_resource_type.cpp
    src/tracer/tracer_resource_type.h
    src/tracer/tracer_result.cpp
    src/tracer/tracer_result.h
    src/tracer/tracer_serie.cpp
    src/tracer/tracer_serie.h
    src/tracer/tracer_values.cpp
    src/tracer/tracer_values.h
)

SET(TRACER_CHART_FILES
    src/tracer/chart/chart_doc.cpp
    src/tracer/chart/chart_doc.h
    src/tracer/chart/chart_preferences.cpp
    src/tracer/chart/chart_preferences.h
    src/tracer/chart/chart_serie.cpp
    src/tracer/chart/chart_serie.h
    src/tracer/chart/chart_tree.cpp
    src/tracer/chart/chart_tree.h
    src/tracer/chart/chart_tree_item.cpp
    src/tracer/chart/chart_tree_item.h
    src/tracer/chart/chart_view.cpp
    src/tracer/chart/chart_view.h
    src/tracer/chart/chart_view_style.cpp
    src/tracer/chart/chart_view_style.h
)

SET(TRACER_LOGGER_FILES
    src/tracer/logger/tracer_logger_main_wnd.cpp
    src/tracer/logger/tracer_logger_main_wnd.h
    src/tracer/logger/tracer_logger_view.cpp
    src/tracer/logger/tracer_logger_view.h
    src/tracer/logger/tracer_logger_view_style.cpp
    src/tracer/logger/tracer_logger_view_style.h
)

SET(TRACER_PCH_FILES
    pch/tracer_pch.cpp
    pch/tracer_pch.h
)

SET(QRC_FILES
    res/rdo_studio.qrc
)

QT5_ADD_RESOURCES(GENERATED_QRC_FILES ${QRC_FILES})

SET(APP_NAME RAO-studio)
CONFIGURE_FILE(${CMAKE_MODULE_PATH}/build.version.cmake ${CMAKE_CURRENT_BINARY_DIR}/build_version.h)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
SET(RESOURCES_FILES
    ${CMAKE_CURRENT_BINARY_DIR}/build_version.h
    ${QRC_FILES}
)

FILE(GLOB RESOURCES_IMAGE_FILES res/images/*.png)
FILE(GLOB RESOURCES_INSERT_MENU_TEMPLATE_FILES res/insert_menu_template/*.txt)
FILE(GLOB_RECURSE RESOURCES_NEW_MODEL_TEMPLATE_FILES res/new_model_template/*.*)
FILE(GLOB RESOURCES_SCINTILLA_FILES res/scintilla/*.xpm)

FILE(GLOB RESOURCES_UI_FILES res/ui/*.ui)
QT5_WRAP_UI(GENERATED_RESOURCES_UI_FILES ${RESOURCES_UI_FILES})

SET(RESOURCES_WIN32_FILES
    res/win32/rdo_studio.ico
)

IF (MSVC)
    SET(RESOURCES_WIN32_FILES ${RESOURCES_WIN32_FILES} res/win32/rdo_studio.rc)
ELSEIF (MINGW)
    SET(RESOURCES_WIN32_FILES res/win32/rdo_studio.rc)
    SET(CMAKE_RC_COMPILER_INIT windres)
    ENABLE_LANGUAGE(RC)
    SET(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> -i <SOURCE> -o <OBJECT>")
ENDIF()

SET(RDO_APP_MOC_FILES
    src/dialog/about_dialog.h
    src/dialog/file_association_dialog.h
    src/dialog/find_dialog.h
    src/dialog/find_replace_dialog.h
    src/dialog/goto_line_dialog.h
    src/dialog/new_model_dialog.h
    src/dock/dock_focusable.h
    src/main_window.h
    src/view_preferences.h
)

SET(RDO_EDITOR_MOC_FILES
    src/editor/edit.h
    src/editor/log_edit.h
)

SET(RDO_FRAME_MOC_FILES
    src/frame/frame_manager.h
    src/frame/frame_tree_ctrl.h
    src/frame/frame_view.h
)

SET(RDO_MODEL_MOC_FILES
    src/editor/model_edit.h
    src/model/model.h
    src/model/model_view.h
)

SET(RDO_TRACER_MOC_FILES
    src/tracer/chart/chart_preferences.h
    src/tracer/chart/chart_tree.h
    src/tracer/chart/chart_view.h
    src/tracer/logger/tracer_logger_view.h
)

SET(QT_MOC_FLAGS)
QT5_GET_MOC_FLAGS(QT_MOC_FLAGS)

SET(GENERATED_APP_MOC_FILES)
FOREACH(RDO_APP_MOC_FILES_IT ${RDO_APP_MOC_FILES})
    QT5_WRAP_CPP(GENERATED_RDO_APP_MOC_FILES_IT ${RDO_APP_MOC_FILES_IT} OPTIONS ${QT_MOC_FLAGS} "-fapp/rdo_studio/pch/application_pch.h" "-futils/src/common/warning_disable.h" "-fapp/rdo_studio/${RDO_APP_MOC_FILES_IT}")
    SET(GENERATED_APP_MOC_FILES ${GENERATED_APP_MOC_FILES} ${GENERATED_RDO_APP_MOC_FILES_IT})
ENDFOREACH()

SET(GENERATED_EDITOR_MOC_FILES)
FOREACH(RDO_EDITOR_MOC_FILES_IT ${RDO_EDITOR_MOC_FILES})
    QT5_WRAP_CPP(GENERATED_RDO_EDITOR_MOC_FILES_IT ${RDO_EDITOR_MOC_FILES_IT} OPTIONS ${QT_MOC_FLAGS} "-fapp/rdo_studio/pch/editor_pch.h" "-futils/src/common/warning_disable.h" "-fapp/rdo_studio/${RDO_EDITOR_MOC_FILES_IT}")
    SET(GENERATED_EDITOR_MOC_FILES ${GENERATED_EDITOR_MOC_FILES} ${GENERATED_RDO_EDITOR_MOC_FILES_IT})
ENDFOREACH()

SET(GENERATED_FRAME_MOC_FILES)
FOREACH(RDO_FRAME_MOC_FILES_IT ${RDO_FRAME_MOC_FILES})
    QT5_WRAP_CPP(GENERATED_RDO_FRAME_MOC_FILES_IT ${RDO_FRAME_MOC_FILES_IT} OPTIONS ${QT_MOC_FLAGS} "-fapp/rdo_studio/pch/frame_pch.h" "-futils/src/common/warning_disable.h" "-fapp/rdo_studio/${RDO_FRAME_MOC_FILES_IT}")
    SET(GENERATED_FRAME_MOC_FILES ${GENERATED_FRAME_MOC_FILES} ${GENERATED_RDO_FRAME_MOC_FILES_IT})
ENDFOREACH()

SET(GENERATED_MODEL_MOC_FILES)
FOREACH(RDO_MODEL_MOC_FILES_IT ${RDO_MODEL_MOC_FILES})
    QT5_WRAP_CPP(GENERATED_RDO_MODEL_MOC_FILES_IT ${RDO_MODEL_MOC_FILES_IT} OPTIONS ${QT_MOC_FLAGS} "-fapp/rdo_studio/pch/model_pch.h" "-futils/src/common/warning_disable.h" "-fapp/rdo_studio/${RDO_MODEL_MOC_FILES_IT}")
    SET(GENERATED_MODEL_MOC_FILES ${GENERATED_MODEL_MOC_FILES} ${GENERATED_RDO_MODEL_MOC_FILES_IT})
ENDFOREACH()

SET(GENERATED_TRACER_MOC_FILES)
FOREACH(RDO_TRACER_MOC_FILES_IT ${RDO_TRACER_MOC_FILES})
    QT5_WRAP_CPP(GENERATED_RDO_TRACER_MOC_FILES_IT ${RDO_TRACER_MOC_FILES_IT} OPTIONS ${QT_MOC_FLAGS} "-fapp/rdo_studio/pch/tracer_pch.h" "-futils/src/common/warning_disable.h" "-fapp/rdo_studio/${RDO_TRACER_MOC_FILES_IT}")
    SET(GENERATED_TRACER_MOC_FILES ${GENERATED_TRACER_MOC_FILES} ${GENERATED_RDO_TRACER_MOC_FILES_IT})
ENDFOREACH()

SET(APP_SOURCE_FILES
    ${APPLICATION_FILES}
    ${APPLICATION_ACTIONACTIVATOR_FILES}
    ${DIALOG_FILES}
    ${DOCK_FILES}
    ${RESOURCES_FILES}
    ${RESOURCES_IMAGE_FILES}
    ${RESOURCES_SCINTILLA_FILES}
    ${RESOURCES_UI_FILES}
    ${RESOURCES_INSERT_MENU_TEMPLATE_FILES}
    ${RESOURCES_NEW_MODEL_TEMPLATE_FILES}
    ${APPLICATION_PCH_FILES}
    ${GENERATED_APP_MOC_FILES}
)

SET(EDITOR_SOURCE_FILES
    ${EDITOR_FILES}
    ${EDITOR_DEBUG_FILES}
    ${EDITOR_LEXER_FILES}
    ${EDITOR_LOG_FILES}
    ${EDITOR_LOG_BUILD_FILES}
    ${EDITOR_LOG_FIND_FILES}
    ${EDITOR_PARSER_FILES}
    ${EDITOR_RESULTS_FILES}
    ${EDITOR_PCH_FILES}
    ${GENERATED_EDITOR_MOC_FILES}
)

SET(FRAME_SOURCE_FILES
    ${FRAME_FILES}
    ${FRAME_PCH_FILES}
    ${GENERATED_FRAME_MOC_FILES}
)

SET(MODEL_SOURCE_FILES
    ${EDITOR_MODEL_FILES}
    ${MODEL_FILES}
    ${MODEL_PCH_FILES}
    ${GENERATED_MODEL_MOC_FILES}
)

SET(TRACER_SOURCE_FILES
    ${TRACER_FILES}
    ${TRACER_CHART_FILES}
    ${TRACER_LOGGER_FILES}
    ${TRACER_PCH_FILES}
    ${GENERATED_TRACER_MOC_FILES}
)

SET(PLUGIN_LOADER_SOURCE_FILES
    ${PLUGINS_FILES}
    ${PLUGIN_LOADER_PCH_FILES}
)

SET(RDO_STUDIO_SOURCE_FILES
    ${MAIN_FILES}
    ${RESOURCES_WIN32_FILES}
    ${STUDIO_PCH_FILES}
)

ADD_EXECUTABLE(rdo_studio
    ${RDO_STUDIO_SOURCE_FILES}
    ${TEXT_FILES}
)

ADD_LIBRARY(rdo_application STATIC
    ${APP_SOURCE_FILES}
    ${GENERATED_RESOURCES_UI_FILES}
    ${GENERATED_QRC_FILES}
)

ADD_LIBRARY(rdo_editor STATIC
    ${EDITOR_SOURCE_FILES}
)

ADD_LIBRARY(rdo_frame STATIC
    ${FRAME_SOURCE_FILES}
)

ADD_LIBRARY(rdo_model STATIC
    ${MODEL_SOURCE_FILES}
)

ADD_LIBRARY(rdo_tracer STATIC
    ${TRACER_SOURCE_FILES}
)

ADD_LIBRARY(rdo_plugin_loader STATIC
    ${PLUGIN_LOADER_SOURCE_FILES}
)

INCLUDE(${PROJECT_SOURCE_DIR}/app/rdo_studio/projects/common/rdo_studio.cmake)
SET_TARGET_PROPERTIES(rdo_studio        PROPERTIES FOLDER ${APP_FOLDER})
SET_TARGET_PROPERTIES(rdo_application   PROPERTIES FOLDER ${APP_FOLDER})
SET_TARGET_PROPERTIES(rdo_editor        PROPERTIES FOLDER ${APP_FOLDER})
SET_TARGET_PROPERTIES(rdo_frame         PROPERTIES FOLDER ${APP_FOLDER})
SET_TARGET_PROPERTIES(rdo_model         PROPERTIES FOLDER ${APP_FOLDER})
SET_TARGET_PROPERTIES(rdo_tracer        PROPERTIES FOLDER ${APP_FOLDER})
SET_TARGET_PROPERTIES(rdo_plugin_loader PROPERTIES FOLDER ${APP_FOLDER})

SET_TARGET_PROPERTIES(rdo_studio PROPERTIES COMPILE_FLAGS "-DSCI_LEXER")

IF (WIN32)
    IF (MSVC)
        SET_TARGET_PROPERTIES(rdo_studio PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:\"mainCRTStartup\"")
        SET_TARGET_PROPERTIES(rdo_studio PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
    ELSE()
        IF (${CMAKE_BUILD_TYPE} STREQUAL "Release")
            SET_TARGET_PROPERTIES(rdo_studio PROPERTIES LINK_FLAGS -mwindows)
        ENDIF()
    ENDIF()
ENDIF()

TARGET_LINK_LIBRARIES(rdo_studio        ${Qt5Widgets_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_application   ${Qt5Widgets_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_editor        ${Qt5Widgets_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_frame         ${Qt5Widgets_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_model         ${Qt5Widgets_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_tracer        ${Qt5Widgets_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_plugin_loader ${Qt5Widgets_LIBRARIES})

TARGET_LINK_LIBRARIES(rdo_studio        ${Qt5Core_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_application   ${Qt5Core_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_editor        ${Qt5Core_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_frame         ${Qt5Core_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_model         ${Qt5Core_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_tracer        ${Qt5Core_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_plugin_loader ${Qt5Core_LIBRARIES})

TARGET_LINK_LIBRARIES(rdo_studio        ${Qt5Gui_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_application   ${Qt5Gui_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_editor        ${Qt5Gui_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_frame         ${Qt5Gui_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_model         ${Qt5Gui_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_tracer        ${Qt5Gui_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_plugin_loader ${Qt5Gui_LIBRARIES})

TARGET_LINK_LIBRARIES(rdo_studio        ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_application   ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_editor        ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_frame         ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_model         ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_tracer        ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_plugin_loader ${Boost_LIBRARIES})

ADD_DEPENDENCIES(rdo_studio rdo_application   )
ADD_DEPENDENCIES(rdo_studio rdo_editor        )
ADD_DEPENDENCIES(rdo_studio rdo_frame         )
ADD_DEPENDENCIES(rdo_studio rdo_help          )
ADD_DEPENDENCIES(rdo_studio rdo_model         )
ADD_DEPENDENCIES(rdo_studio rdo_plugin_loader )
ADD_DEPENDENCIES(rdo_studio rdo_tracer        )

TARGET_LINK_LIBRARIES(rdo_studio rdo_application   )
TARGET_LINK_LIBRARIES(rdo_studio rdo_editor        )
TARGET_LINK_LIBRARIES(rdo_studio rdo_frame         )
TARGET_LINK_LIBRARIES(rdo_studio rdo_model         )
TARGET_LINK_LIBRARIES(rdo_studio rdo_plugin_loader )
TARGET_LINK_LIBRARIES(rdo_studio rdo_tracer        )

ADD_DEPENDENCIES(rdo_application rdo_utils)
ADD_DEPENDENCIES(rdo_application rdo_kernel)
ADD_DEPENDENCIES(rdo_application rdo_repository)
ADD_DEPENDENCIES(rdo_application rdo_simulator)
ADD_DEPENDENCIES(rdo_application rdo_simulator_report)
ADD_DEPENDENCIES(rdo_application rdo_ui_abstract)
ADD_DEPENDENCIES(rdo_application rdo_ui_qt)
ADD_DEPENDENCIES(rdo_application rdo_help)
ADD_DEPENDENCIES(rdo_application scintilla)

TARGET_LINK_LIBRARIES(rdo_application rdo_utils           )
TARGET_LINK_LIBRARIES(rdo_application rdo_kernel          )
TARGET_LINK_LIBRARIES(rdo_application rdo_repository      )
TARGET_LINK_LIBRARIES(rdo_application rdo_simulator       )
TARGET_LINK_LIBRARIES(rdo_application rdo_simulator_report)
TARGET_LINK_LIBRARIES(rdo_application rdo_ui_abstract     )
TARGET_LINK_LIBRARIES(rdo_application rdo_ui_qt           )
TARGET_LINK_LIBRARIES(rdo_application scintilla           )

ADD_DEPENDENCIES     (rdo_editor rdo_application rdo_model)
TARGET_LINK_LIBRARIES(rdo_editor rdo_application rdo_model)

ADD_DEPENDENCIES     (rdo_frame rdo_application rdo_model)
TARGET_LINK_LIBRARIES(rdo_frame rdo_application rdo_model)

ADD_DEPENDENCIES     (rdo_model rdo_application)
TARGET_LINK_LIBRARIES(rdo_model rdo_application rdo_frame rdo_editor)

ADD_DEPENDENCIES     (rdo_plugin_loader rdo_application)
TARGET_LINK_LIBRARIES(rdo_plugin_loader rdo_application)

ADD_DEPENDENCIES     (rdo_tracer rdo_application)
TARGET_LINK_LIBRARIES(rdo_tracer rdo_application)

INSTALL(TARGETS rdo_studio        DESTINATION bin)
INSTALL(TARGETS rdo_application   DESTINATION lib)
INSTALL(TARGETS rdo_editor        DESTINATION lib)
INSTALL(TARGETS rdo_frame         DESTINATION lib)
INSTALL(TARGETS rdo_model         DESTINATION lib)
INSTALL(TARGETS rdo_plugin_loader DESTINATION lib)
INSTALL(TARGETS rdo_tracer        DESTINATION lib)

IF(MSVC_IDE) # options for windows #

    INCLUDE(${PROJECT_SOURCE_DIR}/cmake/pch.cmake)
    ADD_PCH("${RDO_STUDIO_SOURCE_FILES}"    "${STUDIO_PCH_FILES}"        app/rdo_studio pch stdpch)
    ADD_PCH("${APP_SOURCE_FILES}"           "${APPLICATION_PCH_FILES}"   app/rdo_studio pch application_pch)
    ADD_PCH("${EDITOR_SOURCE_FILES}"        "${EDITOR_PCH_FILES}"        app/rdo_studio pch editor_pch)
    ADD_PCH("${FRAME_SOURCE_FILES}"         "${FRAME_PCH_FILES}"         app/rdo_studio pch frame_pch)
    ADD_PCH("${MODEL_SOURCE_FILES}"         "${MODEL_PCH_FILES}"         app/rdo_studio pch model_pch)
    ADD_PCH("${PLUGIN_LOADER_SOURCE_FILES}" "${PLUGIN_LOADER_PCH_FILES}" app/rdo_studio pch plugin_loader_pch)
    ADD_PCH("${TRACER_SOURCE_FILES}"        "${TRACER_PCH_FILES}"        app/rdo_studio pch tracer_pch)
    
    SOURCE_GROUP("rdo_studio\\.doc" FILES
        ${DOC_FILES}
    )

    SOURCE_GROUP("rdo_studio\\.pch" FILES
        ${STUDIO_PCH_FILES}
    )

    SOURCE_GROUP("rdo_studio" FILES
        ${MAIN_FILES}
    )

    SOURCE_GROUP("rdo_studio\\.resources\\.win32" FILES
        ${RESOURCES_WIN32_FILES}
    )

    SOURCE_GROUP("rdo_application\\.application" FILES
        ${APPLICATION_FILES}
    )

    SOURCE_GROUP("rdo_application\\.application\\.action_activator" FILES
        ${APPLICATION_ACTIONACTIVATOR_FILES}
    )

    SOURCE_GROUP("rdo_application\\.dialog" FILES
        ${DIALOG_FILES}
    )

    SOURCE_GROUP("rdo_application\\.dock" FILES
        ${DOCK_FILES}
    )

    SOURCE_GROUP("rdo_application\\.generated\\.ui" FILES
        ${GENERATED_RESOURCES_UI_FILES}
    )

    SOURCE_GROUP("rdo_application\\.generated\\.qrc" FILES
        ${GENERATED_QRC_FILES}
    )

    SOURCE_GROUP("rdo_application\\.generated\\.moc" FILES
        ${GENERATED_APP_MOC_FILES}
    )

    SOURCE_GROUP("rdo_application\\.resources" FILES
        ${RESOURCES_FILES}
    )

    SOURCE_GROUP("rdo_application\\.resources\\.images" FILES
        ${RESOURCES_IMAGE_FILES}
    )

    SOURCE_GROUP("rdo_application\\.resources\\.insert_menu_template" FILES
        ${RESOURCES_INSERT_MENU_TEMPLATE_FILES}
    )

    SOURCE_GROUP("rdo_application\\.resources\\.new_model_template" FILES
        ${RESOURCES_NEW_MODEL_TEMPLATE_FILES}
    )

    SOURCE_GROUP("rdo_application\\.resources\\.scintilla" FILES
        ${RESOURCES_SCINTILLA_FILES}
    )

    SOURCE_GROUP("rdo_application\\.resources\\.ui" FILES
        ${RESOURCES_UI_FILES}
    )

    SOURCE_GROUP("rdo_application\\.pch" FILES
        ${APPLICATION_PCH_FILES}
    )
    
    SOURCE_GROUP("rdo_editor\\.editor" FILES
        ${EDITOR_FILES}
    )

    SOURCE_GROUP("rdo_editor\\.editor\\.debug" FILES
        ${EDITOR_DEBUG_FILES}
    )

    SOURCE_GROUP("rdo_editor\\.editor\\.lexer" FILES
        ${EDITOR_LEXER_FILES}
    )

    SOURCE_GROUP("rdo_editor\\.editor\\.log" FILES
        ${EDITOR_LOG_FILES}
    )

    SOURCE_GROUP("rdo_editor\\.editor\\.log\\.build" FILES
        ${EDITOR_LOG_BUILD_FILES}
    )

    SOURCE_GROUP("rdo_editor\\.editor\\.log\\.find" FILES
        ${EDITOR_LOG_FIND_FILES}
    )

    SOURCE_GROUP("rdo_editor\\.editor\\.parser" FILES
        ${EDITOR_PARSER_FILES}
    )

    SOURCE_GROUP("rdo_editor\\.editor\\.results" FILES
        ${EDITOR_RESULTS_FILES}
    )

    SOURCE_GROUP("rdo_editor\\.generated\\.moc" FILES
        ${GENERATED_EDITOR_MOC_FILES}
    )

    SOURCE_GROUP("rdo_editor\\.pch" FILES
        ${EDITOR_PCH_FILES}
    )
    
    SOURCE_GROUP("rdo_frame\\.frame" FILES
        ${FRAME_FILES}
    )

    SOURCE_GROUP("rdo_frame\\.generated\\.moc" FILES
        ${GENERATED_FRAME_MOC_FILES}
    )

    SOURCE_GROUP("rdo_frame\\.pch" FILES
        ${FRAME_PCH_FILES}
    )
    
    SOURCE_GROUP("rdo_model\\.editor" FILES
        ${EDITOR_MODEL_FILES}
    )

    SOURCE_GROUP("rdo_model\\.model" FILES
        ${MODEL_FILES}
    )

    SOURCE_GROUP("rdo_model\\.generated\\.moc" FILES
        ${GENERATED_MODEL_MOC_FILES}
    )

    SOURCE_GROUP("rdo_model\\.pch" FILES
        ${MODEL_PCH_FILES}
    )
    
    SOURCE_GROUP("rdo_plugin_loader\\.plugins" FILES
        ${PLUGINS_FILES}
    )

    SOURCE_GROUP("rdo_plugin_loader\\.pch" FILES
        ${PLUGIN_LOADER_PCH_FILES}
    )
    
    SOURCE_GROUP("rdo_tracer\\.tracer" FILES
        ${TRACER_FILES}
    )

    SOURCE_GROUP("rdo_tracer\\.tracer\\.chart" FILES
        ${TRACER_CHART_FILES}
    )

    SOURCE_GROUP("rdo_tracer\\.tracer\\.logger" FILES
        ${TRACER_LOGGER_FILES}
    )

    SOURCE_GROUP("rdo_tracer\\.generated\\.moc" FILES
        ${GENERATED_TRACER_MOC_FILES}
    )

    SOURCE_GROUP("rdo_tracer\\.pch" FILES
        ${TRACER_PCH_FILES}
    )    
ENDIF()
