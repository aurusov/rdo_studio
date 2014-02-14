MESSAGE(STATUS "CREATE  RDO_PLUGIN_GAME5 LIBRARY")

IF(PROJECT_OS_LINUX)
	FIND_LIBRARY(PTHREAD pthread $ENV{LD_LIBRARY_PATH})
ENDIF(PROJECT_OS_LINUX)

FIND_PACKAGE(Boost COMPONENTS system filesystem thread REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FILE(WRITE ${PROJECT_SOURCE_DIR}/app/rdo_studio/plugins/game5/src/plugin_game5.json)

SET(PLUGIN_GAME5_MOC_FILES
	src/board.h
	src/combobox_delegate.h
	src/cost_setup_table.h
	src/graph_node.h
	src/plugin_game5.h
	src/plugin_game5_dialog.h
	src/plugin_game5_graph_dialog.h
	src/plugin_game5_graph_node_info_dialog.h
	src/plugin_game5_tiles_order_dialog.h
	src/tiles.h
)

SET(SRC_FILES
	src/plugin_game5.h
	src/plugin_game5.cpp
)

SET(COMPLETER_FILES
	src/multi_select_completer.h
	src/multi_select_completer.cpp	
)

SET(DIALOGS_FILES
	src/plugin_game5_dialog.h
	src/plugin_game5_dialog.cpp
	src/plugin_game5_graph_dialog.h
	src/plugin_game5_graph_dialog.cpp
	src/plugin_game5_graph_node_info_dialog.h
	src/plugin_game5_graph_node_info_dialog.cpp
	src/plugin_game5_tiles_order_dialog.h
	src/plugin_game5_tiles_order_dialog.cpp
)

SET(GAME_BOARD_FILES
	src/board.h
	src/board.cpp
	src/combobox_delegate.h
	src/combobox_delegate.cpp
	src/cost_setup_table.h
	src/cost_setup_table.cpp
	src/tiles.h
	src/tiles.cpp
)

SET(GRAPH_FILES
	src/graph_info.h
	src/graph_info.cpp
	src/graph_edge.h
	src/graph_edge.cpp
	src/graph_node.h
	src/graph_node.cpp
	src/graph_widget.h
	src/graph_widget.cpp
)

SET(DIALOGS_UI_FILES
	src/plugin_game5_dialog.ui
	src/plugin_game5_tiles_order_dialog.ui
	src/plugin_game5_graph_dialog.ui
	src/plugin_game5_graph_node_info_dialog.ui
)

SET(QRC_FILES
	src/res/plugin_game5.qrc
)

QT5_ADD_RESOURCES(GENERATED_QRC_FILES ${QRC_FILES})

SET(RESOURCES_IMAGE_FILES
	src/res/images/gen_sit_dialog.png
	src/res/images/graph_dialog.png
	src/res/images/graph_dialog_d.png
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
MESSAGE(STATUS ${CMAKE_CURRENT_BINARY_DIR})
QT5_WRAP_UI(GENERATED_DIALOGS_UI_FILES ${DIALOGS_UI_FILES})

SET(PLUGIN_SRC_FILES
	${SRC_FILES}
	${COMPLETER_FILES}
	${DIALOGS_FILES}
	${GAME_BOARD_FILES}
	${GRAPH_FILES}
	${DIALOGS_UI_FILES}
	${RESOURCES_IMAGE_FILES}
	${QRC_FILES}
)

SET(QT_MOC_FLAGS)
QT5_GET_MOC_FLAGS(QT_MOC_FLAGS)

SET(GENERATED_PLUGIN_GAME5_MOC_FILES)
FOREACH(PLUGIN_GAME5_MOC_FILES_IT ${PLUGIN_GAME5_MOC_FILES})
	QT5_WRAP_CPP(GENERATED_PLUGIN_GAME5_MOC_FILES_IT ${PLUGIN_GAME5_MOC_FILES_IT} OPTIONS ${QT_MOC_FLAGS} "-fapp/rdo_studio/pch/stdpch.h" "-futils/src/common/warning_disable.h" "-fapp/rdo_studio/plugins/game5/${PLUGIN_GAME5_MOC_FILES_IT}")
	SET(GENERATED_PLUGIN_GAME5_MOC_FILES ${GENERATED_PLUGIN_GAME5_MOC_FILES} ${GENERATED_PLUGIN_GAME5_MOC_FILES_IT})
ENDFOREACH()

ADD_LIBRARY(plugin_game5 SHARED
	${PLUGIN_SRC_FILES}
	${GENERATED_DIALOGS_UI_FILES}
	${GENERATED_PLUGIN_GAME5_MOC_FILES}
	${GENERATED_QRC_FILES}
)

foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
	string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
	SET_TARGET_PROPERTIES(plugin_game5 PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CONFIG_DIRECTORY_${OUTPUTCONFIG}}/plugins
	)
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

SET_TARGET_PROPERTIES(plugin_game5 PROPERTIES FOLDER ${APPS_FOLDERS}/.plugins/.game5)

ADD_DEPENDENCIES(plugin_game5 rdo_application)

TARGET_LINK_LIBRARIES(plugin_game5 ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})
TARGET_LINK_LIBRARIES(plugin_game5 rdo_application)
TARGET_LINK_LIBRARIES(plugin_game5 rdo_editor)
TARGET_LINK_LIBRARIES(plugin_game5 rdo_frame)
TARGET_LINK_LIBRARIES(plugin_game5 rdo_model)
TARGET_LINK_LIBRARIES(plugin_game5 rdo_plugin_loader)
TARGET_LINK_LIBRARIES(plugin_game5 rdo_tracer)

INSTALL(TARGETS plugin_game5 DESTINATION lib)

IF(PROJECT_OS_LINUX OR CYGWIN)
	TARGET_LINK_LIBRARIES(plugin_game5 ${PTHREAD})
ENDIF(PROJECT_OS_LINUX OR CYGWIN)

IF(MSVC) # options for windows #

	SOURCE_GROUP(".generated\\.ui" FILES
		${GENERATED_RESOURCES_UI_FILES}
	)

	SOURCE_GROUP(".generated\\.moc" FILES
		${GENERATED_PLUGIN_GAME5_MOC_FILES}
	)

	SOURCE_GROUP(".generated" FILES
		${GENERATED_QRC_FILES}
	)

	SOURCE_GROUP(".src" FILES
		${SRC_FILES}
		${QRC_FILES}
	)

	SOURCE_GROUP(".src\\.completer" FILES
		${COMPLETER_FILES}
	)

	SOURCE_GROUP(".src\\.dialogs" FILES
		${DIALOGS_FILES}
	)

	SOURCE_GROUP(".src\\.game_board" FILES
		${GAME_BOARD_FILES}
	)

	SOURCE_GROUP(".src\\.graph" FILES
		${GRAPH_FILES}
	)

	SOURCE_GROUP(".src\\.dialogs\\.ui" FILES
		${DIALOGS_UI_FILES}
	)

	SOURCE_GROUP(".src\\.resources" FILES
		${RESOURCES_IMAGE_FILES}
	)
ENDIF(MSVC)
