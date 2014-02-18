#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>,
#               2013 Andrey Urusov <drobus@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  CONVERTER  SMR2RDOX  LIBRARY")

INCLUDE_DIRECTORIES(${BISON_FLEX_DIRECTORY})
INCLUDE_DIRECTORIES(${BISON_FLEX_DIRECTORY}/include)

FIND_PACKAGE(Boost COMPONENTS system filesystem thread date_time REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

FIND_PACKAGE(BISON)
FIND_PACKAGE(FLEX)

IF(NOT BISON_FOUND)
	MESSAGE(FATAL_ERROR "Bison can not be found")
ENDIF()

IF(NOT FLEX_FOUND)
	MESSAGE(FATAL_ERROR "Flex can not be found")
ENDIF()

GET_FILENAME_COMPONENT(BISON_INCLUDE ${BISON_EXECUTABLE} PATH)
SET(BISON_INCLUDE ${BISON_INCLUDE}/../include)
INCLUDE_DIRECTORIES(${BISON_INCLUDE})

SET(GRAMMA_INPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/grammar)
SET(GRAMMA_OUTPUT_PATH ${GRAMMA_INPUT_PATH}/generated)
SET(GRAMMA_H ${GRAMMA_OUTPUT_PATH}/rdogramma.h)

INCLUDE(${PROJECT_SOURCE_DIR}/cmake/bison.cmake)

RDO_BISON_TARGET(rtp cnv_rtp define)
RDO_BISON_TARGET(rss cnv_rss)
RDO_BISON_TARGET(opr cnv_opr)
RDO_BISON_TARGET(pat cnv_pat)
RDO_BISON_TARGET(dpt cnv_dpt)
RDO_BISON_TARGET(frm cnv_frm)
RDO_BISON_TARGET(fun cnv_fun)
RDO_BISON_TARGET(pmd cnv_pmd)
RDO_BISON_TARGET(proc_rtp cnv_proc_rtp_)
RDO_BISON_TARGET(proc_rss cnv_proc_rss_)
RDO_BISON_TARGET(proc_opr cnv_proc_opr_)
RDO_BISON_TARGET(smr_file cnv_smr_file_)
RDO_BISON_TARGET(smr_sim cnv_smr_sim_)

FLEX_TARGET(lexer_scaner ${GRAMMA_INPUT_PATH}/rdo_lexer.l ${GRAMMA_OUTPUT_PATH}/rdolex.cpp COMPILE_FLAGS "-Pcnv -+")
SET_SOURCE_FILES_PROPERTIES(${GRAMMA_OUTPUT_PATH}/rdolex.cpp PROPERTIES COMPILE_FLAGS "/Od")

ADD_FLEX_BISON_DEPENDENCY(lexer_scaner rtp)

SET(ANIMATION_FILES
	rdofrm.cpp
	rdofrm.h
)

SET(COMMON_FILES
	namespace.h
	rdo_object.cpp
	rdo_object.h
)

SET(CONVERTER_FILES
	rdoparser.cpp
	rdoparser.h
	rdoparser_base.cpp
	rdoparser_base.h
	rdoparser_rdo.cpp
	rdoparser_rdo.h
)

SET(CONVERTER_ERROR_FILES
	rdoparser_error.cpp
	rdoparser_error.h
)

SET(CONVERTER_LEXER_FILES
	rdoparser_lexer.cpp
	rdoparser_lexer.inl
	rdoparser_lexer.h
)

SET(DPT_FILES
	rdo_logic.cpp
	rdo_logic.h
	rdodpt.cpp
	rdodpt.h
)

SET(FUNCTION_FILES
	rdofun.cpp
	rdofun.h
)

SET(GRAMMAR_FILES
	${GRAMMA_INPUT_PATH}/rdobison.h
	${GRAMMA_INPUT_PATH}/rdo_lexer.l
	${GRAMMA_INPUT_PATH}/rdortp.y
	${GRAMMA_INPUT_PATH}/rdorss.y
	${GRAMMA_INPUT_PATH}/rdoopr.y
	${GRAMMA_INPUT_PATH}/rdopat.y
	${GRAMMA_INPUT_PATH}/rdodpt.y
	${GRAMMA_INPUT_PATH}/rdofrm.y
	${GRAMMA_INPUT_PATH}/rdofun.y
	${GRAMMA_INPUT_PATH}/rdopmd.y
	${GRAMMA_INPUT_PATH}/rdoproc_rtp.y
	${GRAMMA_INPUT_PATH}/rdoproc_rss.y
	${GRAMMA_INPUT_PATH}/rdoproc_opr.y
	${GRAMMA_INPUT_PATH}/rdosmr_file.y
	${GRAMMA_INPUT_PATH}/rdosmr_sim.y
)

SET(GRAMMAR_GENERATED_NO_PCH
	${FLEX_lexer_scaner_OUTPUTS}
	${BISON_rtp_OUTPUTS}
	${BISON_rss_OUTPUTS}
	${BISON_opr_OUTPUTS}
	${BISON_pat_OUTPUTS}
	${BISON_dpt_OUTPUTS}
	${BISON_frm_OUTPUTS}
	${BISON_fun_OUTPUTS}
	${BISON_pmd_OUTPUTS}
	${BISON_proc_rtp_OUTPUTS}
	${BISON_proc_rss_OUTPUTS}
	${BISON_proc_opr_OUTPUTS}
	${BISON_smr_file_OUTPUTS}
	${BISON_smr_sim_OUTPUTS}
)

SET(GRAMMAR_GENERATED_FILES
	${GRAMMA_H}
	${GRAMMAR_GENERATED_NO_PCH}
)

SET(OPR_FILES
	rdoopr.cpp
	rdoopr.h
)

SET(PATTERN_FILES
	rdopat.cpp
	rdopat.h
)

SET(PCH_FILES
	pch.cpp
	pch.h
)

SET(PROJECT_FILES
	rdosmr.cpp
	rdosmr.h
)

SET(RDO_COMMON_FILES
	rdo_common/model_objects_convertor.h
)

SET(RESORCES_FILES
	rdorss.cpp
	rdorss.h
	rdortp.cpp
	rdortp.h
	rdortp_param.cpp
	rdortp_param.h
)

SET(RESORCES_PARAM_FILES
	param.h
	param.cpp
	rdo_type_param.cpp
	rdo_type_param.h
	rdo_type_param_suchas.h
	rdo_type_param_suchas.cpp
)

SET(RUNTIME_FILES
	runtime/rdo_ie.cpp
	runtime/rdo_ie.h
	runtime/rdo_logic_dptfree.cpp
	runtime/rdo_logic_dptfree.h
	runtime/rdo_logic_dptfree.inl
)

SET(TYPE_FILES
	rdo_type.cpp
	rdo_type.h
)

SET(TYPE_ENUM_FILES
	rdo_enum.cpp
	rdo_enum.h
)

SET(TYPE_RANGE_FILES
	rdo_type_range.h
	rdo_type_range.inl
)

SET(UPDATE_FILES
	update/document.cpp
	update/document.h
	update/document_i.h
	update/update.cpp
	update/update.h
	update/update_i.cpp
	update/update_i.h
)

SET(VALUE_FILES
	rdo_value.cpp
	rdo_value.h
	rdo_value.inl
)

SET(VALUE_ARRAY_FILES
	rdo_array.cpp
	rdo_array.h
)

SET(WATCH_FILES
	rdopmd.cpp
	rdopmd.h
)

SET(SOURCE_FILES
	${PCH_FILES}
	${ANIMATION_FILES}
	${COMMON_FILES}
	${CONVERTER_FILES}
	${CONVERTER_ERROR_FILES}
	${CONVERTER_LEXER_FILES}
	${DPT_FILES}
	${FUNCTION_FILES}
	${GRAMMAR_FILES}
	${GRAMMAR_GENERATED_FILES}
	${OPR_FILES}
	${PATTERN_FILES}
	${PROJECT_FILES}
	${RDO_COMMON_FILES}
	${RESORCES_FILES}
	${RESORCES_PARAM_FILES}
	${RUNTIME_FILES}
	${TYPE_FILES}
	${TYPE_ENUM_FILES}
	${TYPE_RANGE_FILES}
	${UPDATE_FILES}
	${VALUE_FILES}
	${WATCH_FILES}
)

SET(NO_PCH_FILES
	${GRAMMAR_GENERATED_NO_PCH}
	${PCH_FILES}
)

ADD_LIBRARY(rdo_converter_smr2rdox STATIC ${HEADER_FILES} ${INLINE_FILES} ${SOURCE_FILES} ${GENERATED_FILES} ${GRAMMA_FILES})
SET_TARGET_PROPERTIES(rdo_converter_smr2rdox PROPERTIES LINKER_LANGUAGE CXX)
SET_TARGET_PROPERTIES(rdo_converter_smr2rdox PROPERTIES FOLDER .converter)

ADD_DEPENDENCIES(rdo_converter_smr2rdox rdo_utils)
ADD_DEPENDENCIES(rdo_converter_smr2rdox rdo_runtime)

TARGET_LINK_LIBRARIES(rdo_converter_smr2rdox ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_DATE_TIME_LIBRARY})
TARGET_LINK_LIBRARIES(rdo_converter_smr2rdox rdo_utils)
TARGET_LINK_LIBRARIES(rdo_converter_smr2rdox rdo_runtime)

INSTALL(TARGETS rdo_converter_smr2rdox DESTINATION lib)

IF(MSVC) # options for windows #

	SET_TARGET_PROPERTIES(rdo_converter_smr2rdox PROPERTIES COMPILE_FLAGS "-D_UNISTD_H_")

	INCLUDE(${PROJECT_SOURCE_DIR}/cmake/pch.cmake)
	ADD_PCH("${SOURCE_FILES}" "${NO_PCH_FILES}" converter/smr2rdox ./ pch)

	SOURCE_GROUP(".animation" FILES 
		${ANIMATION_FILES}
	)
	
	SOURCE_GROUP(".common" FILES 
		${COMMON_FILES}
	)

	SOURCE_GROUP(".converter" FILES 
		${CONVERTER_FILES}
	)

	SOURCE_GROUP(".converter\\.error" FILES 
		${CONVERTER_ERROR_FILES}
	)

	SOURCE_GROUP(".converter\\.lexer" FILES
		${CONVERTER_LEXER_FILES}
	)

	SOURCE_GROUP(".dpt" FILES
		${DPT_FILES}
	)

	SOURCE_GROUP(".function" FILES
		${FUNCTION_FILES}
	)

	SOURCE_GROUP(".grammar" FILES
		${GRAMMAR_FILES}
	)

	SOURCE_GROUP(".grammar\\generated" FILES
		${GRAMMAR_GENERATED_FILES}
	)

	SOURCE_GROUP(".opr" FILES
		${OPR_FILES}
	)

	SOURCE_GROUP(".pattern" FILES
		${PATTERN_FILES}
	)

	SOURCE_GROUP(".pch" FILES
		${PCH_FILES}
	)

	SOURCE_GROUP(".project" FILES
		${PROJECT_FILES}
	)

	SOURCE_GROUP(".rdo_common" FILES
		${RDO_COMMON_FILES}
	)

	SOURCE_GROUP(".resorces" FILES
		${RESORCES_FILES}
	)

	SOURCE_GROUP(".resorces\\.param" FILES
		${RESORCES_PARAM_FILES}
	)

	SOURCE_GROUP(".runtime" FILES
		${RUNTIME_FILES}
	)

	SOURCE_GROUP(".type" FILES
		${TYPE_FILES}
	)

	SOURCE_GROUP(".type\\.enum" FILES
		${TYPE_ENUM_FILES}
	)

	SOURCE_GROUP(".type\\.range" FILES
		${TYPE_RANGE_FILES}
	)

	SOURCE_GROUP(".update" FILES
		${UPDATE_FILES}
	)

	SOURCE_GROUP(".value" FILES
		${VALUE_FILES}
	)

	SOURCE_GROUP(".watch" FILES
		${WATCH_FILES}
	)

ENDIF(MSVC)
