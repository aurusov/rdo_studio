#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE CONVERTER SMR2RDOX LIBRARY")

# Find Boost
IF(MSVC)
    SET(Boost_USE_STATIC_LIBS   ON)
    SET(Boost_USE_MULTITHREADED ON)
ENDIF(MSVC)
FIND_PACKAGE(Boost COMPONENTS system filesystem thread date_time REQUIRED)

FIND_PACKAGE(BISON)
FIND_PACKAGE(FLEX)

IF(NOT BISON_FOUND)
	MESSAGE(ERROR "Bison can not be found")
ENDIF(NOT BISON_FOUND)
IF(NOT FLEX_FOUND)
	MESSAGE(ERROR "Flex can not be found")
ENDIF(NOT FLEX_FOUND)

SET(GENERATED_FILES)

IF(PROJECT_OS_LINUX)

	FLEX_TARGET(lexer_scaner grammar/rdo_lexer.l  ${CMAKE_CURRENT_SOURCE_DIR}/rdolex.cpp)

	SET(BISON_TEMP_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/grammar/rdogramsmr_sim.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/grammar/rdogramsmr_file.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/grammar/rdogramrtp.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/grammar/rdogramrss.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/grammar/rdogramproc_rtp.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/grammar/rdogramproc_rss.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/grammar/rdogramproc_opr.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/grammar/rdogrampmd.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/grammar/rdogrampat.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/grammar/rdogramopr.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/grammar/rdogramfun.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/grammar/rdogramfrm.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/grammar/rdogramdpt.cpp)

SET(GENERATED_FILES
	${FLEX_lexer_scaner_OUTPUTS}
	${BISON_TEMP_FILES}
	grammar/rdogramma.h)

ENDIF(PROJECT_OS_LINUX)

SET(HEADER_FILES	
    namespace.h
    param.h
    pch.h
    rdobison.h
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

    context/stack.h
    context/context.h

    runtime/rdo_ie.h
    runtime/rdo_logic_dptfree.h

    update/document.h
    update/document_i.h
    update/update.h
    update/update_i.h)

SET(INLINE_FILES
    rdoparser_lexer.inl
    rdo_type_range.inl
    rdo_value.inl

    runtime/rdo_logic_dptfree.inl

    update/update_i.inl)

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

    context/stack.cpp
    context/context.cpp

    runtime/rdo_ie.cpp
    runtime/rdo_logic_dptfree.cpp

    update/document.cpp
    update/update.cpp)

ADD_LIBRARY(rdo_converter_smr2rdox ${HEADER_FILES} ${SOURCE_FILES} ${GENERATED_FILES})
SET_TARGET_PROPERTIES(rdo_converter_smr2rdox PROPERTIES LINKER_LANGUAGE CXX)

ADD_DEPENDENCIES(rdo_converter_smr2rdox rdo_utils)
ADD_DEPENDENCIES(rdo_converter_smr2rdox rdo_runtime)

TARGET_LINK_LIBRARIES(rdo_converter_smr2rdox ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_DATE_TIME_LIBRARY})
TARGET_LINK_LIBRARIES(rdo_converter_smr2rdox rdo_utils)
TARGET_LINK_LIBRARIES(rdo_converter_smr2rdox rdo_runtime)
