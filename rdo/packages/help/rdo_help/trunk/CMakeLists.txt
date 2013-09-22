#==============================================================================
# Copyright (c) 2013 Andrey Urusov <drobus@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO  HELP")

SET(QT_DIR $ENV{QTDIR})
SET(QT_BIN_DIR ${QT_DIR}/bin)
SET(QT_PLUGIN_PATH ${QT_DIR}/plugins)

FIND_PROGRAM(
	QCOLLECTIONGENERATOR_EXECUTABLE
	NAME qcollectiongenerator
	PATHS ${QT_BIN_DIR}
)

IF(NOT QCOLLECTIONGENERATOR_EXECUTABLE)
	MESSAGE(FATAL_ERROR "Unable to find 'qcollectiongenerator' program") 
ENDIF()

SET(HELP_COLLECTION_FILES
	RAO-help-collection.qhcp
)

SET(HELP_PROJECT_FILES
	rdo_lang_rus.qhp
	rdo_studio_rus.qhp
)

FILE(GLOB RDOLANGRUS_HTML_FILES "rdo_lang_rus/html/*.htm")
FILE(GLOB RDOLANGRUS_HTML_RDO_BASE_FILES "rdo_lang_rus/html/rdo_base/*.htm")
FILE(GLOB RDOLANGRUS_HTML_RDO_DPT_FILES "rdo_lang_rus/html/rdo_dpt/*.htm")
FILE(GLOB RDOLANGRUS_HTML_RDO_LEX_DIV_FILES "rdo_lang_rus/html/rdo_lex_div/*.htm")
FILE(GLOB RDOLANGRUS_HTML_RDO_OBJ_FILES "rdo_lang_rus/html/rdo_obj/*.htm")
FILE(GLOB RDOLANGRUS_HTML_RDO_OBJ_SEC_FILES "rdo_lang_rus/html/rdo_obj/rdo_obj_sec/*.htm")
FILE(GLOB RDOLANGRUS_HTML_RDO_OBJ_PAT_TYPE_FILES "rdo_lang_rus/html/rdo_obj/rdo_pat_type/*.htm")
FILE(GLOB RDOLANGRUS_HTML_RDO_RES_FILES "rdo_lang_rus/html/rdo_res/*.htm")
FILE(GLOB RDOLANGRUS_HTML_RDO_THEORY_FILES "rdo_lang_rus/html/rdo_theory/*.htm")
FILE(GLOB RDOLANGRUS_IMAGES_FILES "rdo_lang_rus/images/*")

FILE(GLOB RDOSTUDIORUS_HTML_FILES "rdo_studio_rus/html/*.htm")
FILE(GLOB RDOSTUDIORUS_HTML_WORK_MODEL_FILES "rdo_studio_rus/html/work_model/*.htm")
FILE(GLOB RDOSTUDIORUS_HTML_WORK_OPTIONS_FILES "rdo_studio_rus/html/work_options/*.htm")
FILE(GLOB RDOSTUDIORUS_IMAGES_FILES "rdo_studio_rus/images/*")

SET(SOURCES_FILES
	${HELP_COLLECTION_FILES}
	${HELP_PROJECT_FILES}
	${RDOLANGRUS_HTML_FILES}
	${RDOLANGRUS_HTML_RDO_BASE_FILES}
	${RDOLANGRUS_HTML_RDO_DPT_FILES}
	${RDOLANGRUS_HTML_RDO_LEX_DIV_FILES}
	${RDOLANGRUS_HTML_RDO_OBJ_FILES}
	${RDOLANGRUS_HTML_RDO_OBJ_SEC_FILES}
	${RDOLANGRUS_HTML_RDO_OBJ_PAT_TYPE_FILES}
	${RDOLANGRUS_HTML_RDO_RES_FILES}
	${RDOLANGRUS_HTML_RDO_THEORY_FILES}
	${RDOLANGRUS_IMAGES_FILES}
	${RDOSTUDIORUS_HTML_FILES}
	${RDOSTUDIORUS_HTML_WORK_MODEL_FILES}
	${RDOSTUDIORUS_HTML_WORK_OPTIONS_FILES}
	${RDOSTUDIORUS_IMAGES_FILES}
)

ADD_CUSTOM_COMMAND(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/RAO-help.qhc ${CMAKE_CURRENT_BINARY_DIR}/rdo_lang_rus.qch ${CMAKE_CURRENT_BINARY_DIR}/rdo_studio_rus.qch
	COMMAND ${QCOLLECTIONGENERATOR_EXECUTABLE} ARGS RAO-help-collection.qhcp -o ${CMAKE_CURRENT_BINARY_DIR}/RAO-help.qhc
	COMMAND ${CMAKE_COMMAND} -E copy   ${CMAKE_CURRENT_SOURCE_DIR}/rdo_lang_rus.qch ${CMAKE_CURRENT_BINARY_DIR}/rdo_lang_rus.qch
	COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_CURRENT_SOURCE_DIR}/rdo_lang_rus.qch
	COMMAND ${CMAKE_COMMAND} -E copy   ${CMAKE_CURRENT_SOURCE_DIR}/rdo_studio_rus.qch ${CMAKE_CURRENT_BINARY_DIR}/rdo_studio_rus.qch
	COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_CURRENT_SOURCE_DIR}/rdo_studio_rus.qch
	DEPENDS ${SOURCES_FILES}
	COMMENT "generate help files"
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_CUSTOM_TARGET(rdo_help
	SOURCES ${SOURCES_FILES}
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/RAO-help.qhc
)

INCLUDE(${PROJECT_SOURCE_DIR}/app/rdo_studio/projects/common/rdo_studio.cmake)
SET_TARGET_PROPERTIES(rdo_help PROPERTIES FOLDER ${APP_FOLDER}/.help)

IF(MSVC) # options for windows #

	SOURCE_GROUP(".collection" FILES
		${HELP_COLLECTION_FILES}
	)

	SOURCE_GROUP(".project" FILES
		${HELP_PROJECT_FILES}
	)

	SOURCE_GROUP(".rdo_lang_rus\\.html" FILES
		${RDOLANGRUS_HTML_FILES}
	)

	SOURCE_GROUP(".rdo_lang_rus\\.html\\rdo_base" FILES
		${RDOLANGRUS_HTML_RDO_BASE_FILES}
	)

	SOURCE_GROUP(".rdo_lang_rus\\.html\\rdo_dpt" FILES
		${RDOLANGRUS_HTML_RDO_DPT_FILES}
	)

	SOURCE_GROUP(".rdo_lang_rus\\.html\\rdo_lex_div" FILES
		${RDOLANGRUS_HTML_RDO_LEX_DIV_FILES}
	)

	SOURCE_GROUP(".rdo_lang_rus\\.html\\rdo_obj" FILES
		${RDOLANGRUS_HTML_RDO_OBJ_FILES}
	)

	SOURCE_GROUP(".rdo_lang_rus\\.html\\rdo_obj\\rdo_obj_sec" FILES
		${RDOLANGRUS_HTML_RDO_OBJ_SEC_FILES}
	)

	SOURCE_GROUP(".rdo_lang_rus\\.html\\rdo_obj\\rdo_pat_type" FILES
		${RDOLANGRUS_HTML_RDO_OBJ_PAT_TYPE_FILES}
	)

	SOURCE_GROUP(".rdo_lang_rus\\.html\\rdo_res" FILES
		${RDOLANGRUS_HTML_RDO_RES_FILES}
	)

	SOURCE_GROUP(".rdo_lang_rus\\.html\\rdo_theory" FILES
		${RDOLANGRUS_HTML_RDO_THEORY_FILES}
	)

	SOURCE_GROUP(".rdo_lang_rus\\.images" FILES
		${RDOLANGRUS_IMAGES_FILES}
	)

	SOURCE_GROUP(".rdo_studio_rus\\.html" FILES
		${RDOSTUDIORUS_HTML_FILES}
	)

	SOURCE_GROUP(".rdo_studio_rus\\.html\\work_model" FILES
		${RDOSTUDIORUS_HTML_WORK_MODEL_FILES}
	)

	SOURCE_GROUP(".rdo_studio_rus\\.html\\work_options" FILES
		${RDOSTUDIORUS_HTML_WORK_OPTIONS_FILES}
	)

	SOURCE_GROUP(".rdo_studio_rus\\.images" FILES
		${RDOSTUDIORUS_IMAGES_FILES}
	)

ENDIF()
