MESSAGE(STATUS "CREATE  RDO_PLUGIN_GAME5 LIBRARY")

IF(PROJECT_OS_LINUX)
	FIND_LIBRARY(PTHREAD pthread $ENV{LD_LIBRARY_PATH})
ENDIF(PROJECT_OS_LINUX)

FIND_PACKAGE(Boost COMPONENTS system filesystem thread REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(MOC_FILES
	src/board.h
    src/comboboxdelegate.h
    src/costsetuptable.h
    src/plugin_game5.h
    src/plugin_game5_dialog.h
    src/plugin_game5_tiles_order_dialog.h
    src/tiles.h
)

SET(SRC_FILES
	src/board.h
	src/board.cpp
    src/comboboxdelegate.h
	src/comboboxdelegate.cpp
	src/costsetuptable.h
	src/costsetuptable.cpp
    src/plugin_game5.h
	src/plugin_game5.cpp
    src/plugin_game5_dialog.h
	src/plugin_game5_dialog.cpp
    src/plugin_game5_tiles_order_dialog.h
	src/plugin_game5_tiles_order_dialog.cpp
    src/tiles.h
	src/tiles.cpp
)

SET(SRC_UI_FILES
	src/plugin_game5_dialog.ui
    src/plugin_game5_tiles_order_dialog.ui
)

QT5_WRAP_UI(GENERATED_SRC_UI_FILES ${SRC_UI_FILES})

SET(QT_MOC_FLAGS)
QT5_GET_MOC_FLAGS(QT_MOC_FLAGS)

SET(GENERATED_MOC_FILES)
FOREACH(MOC_FILES_IT ${MOC_FILES})
	QT5_WRAP_CPP(GENERATED_MOC_FILES_IT ${MOC_FILES_IT} OPTIONS ${QT_MOC_FLAGS})
	SET(GENERATED_MOC_FILES ${GENERATED_MOC_FILES} ${GENERATED_MOC_FILES_IT})
ENDFOREACH()

ADD_LIBRARY(plugin_game5 SHARED
    ${SRC_FILES}
    ${GENERATED_SRC_UI_FILES}
    ${GENERATED_MOC_FILES}
 )
SET_TARGET_PROPERTIES(plugin_game5 PROPERTIES FOLDER .plugins)

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
		${GENERATED_MOC_FILES}
	)

	SOURCE_GROUP(".src" FILES
		${SRC_FILES}
	)
    
    SOURCE_GROUP(".src\\.ui" FILES
		${SRC_UI_FILES}
	)

ENDIF(MSVC)
