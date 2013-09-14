#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
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
ENDIF(NOT BISON_FOUND)
IF(NOT FLEX_FOUND)
    MESSAGE(FATAL_ERROR "Flex can not be found")
ENDIF(NOT FLEX_FOUND)

SET(GRAMMA_PATH ${CMAKE_CURRENT_SOURCE_DIR}/grammar)

CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/cmake/rdogramma.h.cmake ${GRAMMA_PATH}/rdogramma.h)

INCLUDE(${PROJECT_SOURCE_DIR}/cmake/bison.cmake)

RDO_BISON_TARGET(smr_sim cnv_smr_sim_)
RDO_BISON_TARGET(smr_file cnv_smr_file_)
RDO_BISON_TARGET(proc_rtp cnv_proc_rtp_)
RDO_BISON_TARGET(proc_rss cnv_proc_rss_)
RDO_BISON_TARGET(proc_opr cnv_proc_opr_)
RDO_BISON_TARGET(rtp cnv_rtp define)
RDO_BISON_TARGET(rss cnv_rss)
RDO_BISON_TARGET(pmd cnv_pmd)
RDO_BISON_TARGET(pat cnv_pat)
RDO_BISON_TARGET(opr cnv_opr)
RDO_BISON_TARGET(fun cnv_fun)
RDO_BISON_TARGET(frm cnv_frm)
RDO_BISON_TARGET(dpt cnv_dpt)

FLEX_TARGET(lexer_scaner ${GRAMMA_PATH}/rdo_lexer.l ${GRAMMA_PATH}/rdolex.cpp COMPILE_FLAGS "-Pcnv -+")

ADD_FLEX_BISON_DEPENDENCY(lexer_scaner rtp)

SET(GENERATED_FILES
    ${GRAMMA_PATH}/rdogramma.h
    ${FLEX_lexer_scaner_OUTPUTS}
    ${GRAMMA_PATH}/rdogramrtp.cpp
    ${GRAMMA_PATH}/rdogramrtp.hpp
    ${GRAMMA_PATH}/rdogramdpt.cpp
    ${GRAMMA_PATH}/rdogramfrm.cpp
    ${GRAMMA_PATH}/rdogramfun.cpp
    ${GRAMMA_PATH}/rdogramopr.cpp
    ${GRAMMA_PATH}/rdogrampat.cpp
    ${GRAMMA_PATH}/rdogrampmd.cpp
    ${GRAMMA_PATH}/rdogramproc_opr.cpp
    ${GRAMMA_PATH}/rdogramproc_rss.cpp
    ${GRAMMA_PATH}/rdogramproc_rtp.cpp
    ${GRAMMA_PATH}/rdogramrss.cpp
    ${GRAMMA_PATH}/rdogramsmr_file.cpp
    ${GRAMMA_PATH}/rdogramsmr_sim.cpp
    )

SET(HEADER_FILES
    namespace.h
    param.h
    pch.h
    ${GRAMMA_PATH}/rdobison.h
    rdodpt.h
    rdo_enum.h
    rdofrm.h
    rdofun.h
    rdo_logic.h
    rdo_object.h
    rdoopr.h
    rdoparser_base.h
    rdoparser_error.h
    rdoparser.h
    rdoparser_lexer.h
    rdoparser_rdo.h
    rdopat.h
    rdopmd.h
    rdorss.h
    rdortp.h
    rdortp_param.h
    rdosmr.h
    rdo_type.h
    rdo_type_param.h
    rdo_type_param_suchas.h
    rdo_type_range.h
    rdo_value.h
    runtime/rdo_ie.h
    runtime/rdo_logic_dptfree.h
    update/document.h
    update/document_i.h
    update/update.h
    update/update_i.h
	)

SET(INLINE_FILES
    rdoparser_lexer.inl
    rdo_type_range.inl
    rdo_value.inl
    runtime/rdo_logic_dptfree.inl
	)

SET(SOURCE_FILES
    param.cpp
    pch.cpp
    rdodpt.cpp
    rdo_enum.cpp
    rdofrm.cpp
    rdofun.cpp
    rdo_logic.cpp
    rdo_object.cpp
    rdoopr.cpp
    rdoparser_base.cpp
    rdoparser.cpp
    rdoparser_error.cpp
    rdoparser_lexer.cpp
    rdoparser_rdo.cpp
    rdopat.cpp
    rdopmd.cpp
    rdorss.cpp
    rdortp.cpp
    rdortp_param.cpp
    rdosmr.cpp
    rdo_type.cpp
    rdo_type_param.cpp
    rdo_type_param_suchas.cpp
    rdo_value.cpp
    runtime/rdo_ie.cpp
    runtime/rdo_logic_dptfree.cpp
    update/document.cpp
    update/update.cpp
    update/update_i.cpp
	)

SET(GRAMMA_FILES
    ${GRAMMA_PATH}/rdo_lexer.l
    ${GRAMMA_PATH}/rdoproc_rtp.y
    ${GRAMMA_PATH}/rdoproc_rss.y
    ${GRAMMA_PATH}/rdoproc_opr.y
    ${GRAMMA_PATH}/rdosmr_file.y
    ${GRAMMA_PATH}/rdosmr_sim.y
    ${GRAMMA_PATH}/rdortp.y
    ${GRAMMA_PATH}/rdorss.y
    ${GRAMMA_PATH}/rdopmd.y
    ${GRAMMA_PATH}/rdopat.y
    ${GRAMMA_PATH}/rdoopr.y
    ${GRAMMA_PATH}/rdofun.y
    ${GRAMMA_PATH}/rdofrm.y
    ${GRAMMA_PATH}/rdodpt.y
	)

ADD_LIBRARY(rdo_converter_smr2rdox ${HEADER_FILES} ${INLINE_FILES} ${SOURCE_FILES} ${GENERATED_FILES} ${GRAMMA_FILES})
SET_TARGET_PROPERTIES(rdo_converter_smr2rdox PROPERTIES LINKER_LANGUAGE CXX)
SET_TARGET_PROPERTIES(rdo_converter_smr2rdox PROPERTIES FOLDER ${LIBRARY_FOLDERS})

ADD_DEPENDENCIES(rdo_converter_smr2rdox rdo_utils)
ADD_DEPENDENCIES(rdo_converter_smr2rdox rdo_runtime)

TARGET_LINK_LIBRARIES(rdo_converter_smr2rdox ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_DATE_TIME_LIBRARY})
TARGET_LINK_LIBRARIES(rdo_converter_smr2rdox rdo_utils)
TARGET_LINK_LIBRARIES(rdo_converter_smr2rdox rdo_runtime)

INSTALL(TARGETS rdo_converter_smr2rdox DESTINATION lib)

IF(MSVC)
    SET_TARGET_PROPERTIES(rdo_converter_smr2rdox PROPERTIES COMPILE_FLAGS "-D_UNISTD_H_")
ENDIF(MSVC)

IF(MSVC) # lotions for windows #

SOURCE_GROUP(".animation" FILES 
    rdofrm.h
    rdofrm.cpp
	)
	
SOURCE_GROUP(".common" FILES 
    namespace.h
    rdo_object.h
    rdo_object.cpp
	)	

SOURCE_GROUP(".converter" FILES 
    ${GRAMMA_PATH}/rdobison.h
    rdoparser.cpp
    rdoparser.h
    rdoparser_base.cpp
    rdoparser_base.h
    rdoparser_rdo.cpp
    rdoparser_rdo.h
	)
	
SOURCE_GROUP(".converter_error" FILES 
    rdoparser_error.cpp
    rdoparser_error.h
	)
	
SOURCE_GROUP(".converter_lexer" FILES
    rdoparser_lexer.cpp
    rdoparser_lexer.inl
    rdoparser_lexer.h
	)
	
SOURCE_GROUP(".dpt" FILES
    rdodpt.h
    rdodpt.cpp
    rdo_logic.h
    rdo_logic.cpp
	)	

SOURCE_GROUP(".function" FILES
    rdofun.h
    rdofun.cpp
	)

SOURCE_GROUP(".grammar" FILES
    ${GRAMMA_FILES}
    ${GENERATED_FILES}
	)
	
SOURCE_GROUP(".opt" FILES
    rdoopr.cpp
    rdoopr.h
	)
	
SOURCE_GROUP(".pattern" FILES	
    rdopat.cpp
    rdopat.h
	)

SOURCE_GROUP(".pch" FILES
    pch.h
    pch.cpp
	)

SOURCE_GROUP(".project" FILES
    rdosmr.h
    rdosmr.cpp
	)

SOURCE_GROUP(".rdo_common" FILES
    rdo_common/model_objects_convertor.h
	)

SOURCE_GROUP(".resorces" FILES
    rdortp_param.h
    rdortp_param.cpp
    rdortp.h
    rdortp.cpp
    rdorss.h
    rdorss.cpp
	)

SOURCE_GROUP(".resorces_param" FILES
    param.h
    param.cpp
	)

SOURCE_GROUP(".value" FILES
    rdo_value.h
    rdo_value.inl
    rdo_value.cpp
	)

SOURCE_GROUP(".watch" FILES
    rdopmd.h
    rdopmd.cpp
	)

SOURCE_GROUP(".type" FILES
    rdo_type.h
    rdo_type.cpp
	rdo_type_range.inl
	)

SOURCE_GROUP(".type_enum" FILES
    rdo_enum.h
    rdo_enum.cpp
	)

SOURCE_GROUP(".type_param" FILES
    rdo_type_param.cpp
    rdo_type_param.h
	)

SOURCE_GROUP(".type_such_as" FILES
    rdo_type_param_suchas.h
    rdo_type_param_suchas.cpp
	)

SOURCE_GROUP(".type_param" FILES
    rdo_type_range.h
    rdo_type_range.cpp
	)

SOURCE_GROUP(".runtime" FILES
    runtime/rdo_ie.cpp
    runtime/rdo_ie.h
    runtime/rdo_logic_dptfree.cpp
    runtime/rdo_logic_dptfree.inl
    runtime/rdo_logic_dptfree.h
	)

SOURCE_GROUP(".update" FILES
    update/document.cpp
    update/document.h
    update/document_i.h
    update/update.cpp
    update/update.h
    update/update_i.cpp
    update/update_i.h
	)
	
ENDIF(MSVC)
