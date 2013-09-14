#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>,
#               2013 Andrey Urusov <drobus@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO_PARSER  LIBRARY")

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

SET(GRAMMA_INPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/grammar)
SET(GRAMMA_OUTPUT_PATH ${GRAMMA_INPUT_PATH}/generated)
SET(GRAMMA_H ${GRAMMA_OUTPUT_PATH}/rdogramma.h)

INCLUDE(${PROJECT_SOURCE_DIR}/cmake/bison.cmake)

RDO_BISON_TARGET(rtp rtp define)
RDO_BISON_TARGET(rss)
RDO_BISON_TARGET(evn)
RDO_BISON_TARGET(evn_preparse evn_preparse_)
RDO_BISON_TARGET(pat)
RDO_BISON_TARGET(dpt)
RDO_BISON_TARGET(frm)
RDO_BISON_TARGET(fun)
RDO_BISON_TARGET(pmd)
RDO_BISON_TARGET(proc_rtp proc_rtp_)
RDO_BISON_TARGET(proc_rss proc_rss_)
RDO_BISON_TARGET(proc_opr proc_opr_)
RDO_BISON_TARGET(smr_file smr_file_)
RDO_BISON_TARGET(smr_sim smr_sim_)

FLEX_TARGET(lexer_scaner ${GRAMMA_INPUT_PATH}/rdo_lexer.l ${GRAMMA_OUTPUT_PATH}/rdolex.cpp COMPILE_FLAGS "-+")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/parser)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/context)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/type)

SET(ANIMATION_FILES
    src/animation/animation_base.cpp
    src/animation/animation_base.h
    src/animation/animation_frame.cpp
    src/animation/animation_frame.h
    src/animation/animation_sprite.cpp
    src/animation/animation_sprite.h
)

SET(COMMON_FILES
    bison_value_pair.h
    bison_value_pair.inl
    namespace.h
    rdo_object.cpp
    rdo_object.h
)

SET(CONTEXT_FILES
    context/context.cpp
    context/context.h
    context/context.inl
    context/context_create_calc_i.h
    context/context_find.cpp
    context/context_find_i.h
    context/context_switch_i.h
    context/context_type.cpp
    context/context_type.h
    context/memory.cpp
    context/memory.h
    context/stack.cpp
    context/stack.h
    context/statement.cpp
    context/statement.h
)

SET(CONTEXT_FUNCTION_FILES
    context/function/context_function_body.cpp
    context/function/context_function_body.h
    context/function/context_function_param_definition.cpp
    context/function/context_function_param_definition.h
)

SET(DPT_FILES
    rdo_logic.cpp
    rdo_logic.h
    rdo_logic.inl
    rdo_logic_base.h
    rdo_logic_base.cpp
    rdodpt.cpp
    rdodpt.h
    rdoprocess.h
    rdoprocess.cpp
)

SET(FUNCTION_FILES
    rdofun.cpp
    rdofun.h
    src/function/function.cpp
    src/function/function.h
)

SET(FUNCTION_EXPRESSION_FILES
    expression.h
    expression.cpp
)

SET(FUNCTION_LOCAL_VARIABLE_FILES
    src/function/local_variable/local_variable.cpp
    src/function/local_variable/local_variable.h
    src/function/local_variable/local_variable_context.cpp
    src/function/local_variable/local_variable_context.h
)

SET(GRAMMAR_FILES
    ${GRAMMA_INPUT_PATH}/rdobison.h
    ${GRAMMA_INPUT_PATH}/rdo_lexer.l
    ${GRAMMA_INPUT_PATH}/rdortp.y
    ${GRAMMA_INPUT_PATH}/rdorss.y
    ${GRAMMA_INPUT_PATH}/rdoevn_preparse.y
    ${GRAMMA_INPUT_PATH}/rdoevn.y
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
    ${BISON_evn_preparse_OUTPUTS}
    ${BISON_evn_OUTPUTS}
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

SET(PARSER_FILES
    rdoparser.cpp
    rdoparser.h
    rdoparser_base.cpp
    rdoparser_base.h
    rdoparser_corba.cpp
    rdoparser_corba.h
    rdoparser_rdo.cpp
    rdoparser_rdo.h
    parser/std_fun.cpp
    parser/std_fun.h
)

SET(PARSER_ERROR_FILES
    rdoparser_error.cpp
    rdoparser_error.h
)

SET(PARSER_LEXER_FILES
    rdoparser_lexer.cpp
    rdoparser_lexer.h
    rdoparser_lexer.inl
)

SET(PATTERN_FILES
    rdopat.cpp
    rdopat.h
    rdopatpreparse.h
    rdopatpreparse.inl
)

SET(PCH_FILES
    pch.cpp
    pch.h
)

SET(PROJECT_FILES
    rdosmr.cpp
    rdosmr.h
)

SET(RESORCES_FILES
    rdorss.cpp
    rdorss.h
    rdortp.cpp
    rdortp.h
    rdortp.inl
    rdortp_param.cpp
    rdortp_param.h
)

SET(RESORCES_PARAM_FILES
    param.h
    param.cpp
)

SET(TYPE_FILES
    type/type.cpp
    type/type.h
    type/type_i.h
)

SET(TYPE_FUNCTION_FILES
    type/function_type.cpp
    type/function_type.h
)

SET(TYPE_INFO_FILES
    type/info.cpp
    type/info.h
    type/info.inl
)

SET(TYPE_RUNTIMEWRAPPER_FILES
    type/runtime_wrapper_type.cpp
    type/runtime_wrapper_type.h
)

SET(TYPE_RUNTIMEWRAPPER_ARRAY_FILES
    type/array.cpp
    type/array.h
)

SET(TYPE_RUNTIMEWRAPPER_ATOM_FILES
    type/atom.cpp
    type/atom.h
)

SET(TYPE_RUNTIMEWRAPPER_ENUM_FILES
    type/enum.cpp
    type/enum.h
)

SET(TYPE_RUNTIMEWRAPPER_RANGE_FILES
    type/range.cpp
    type/range.h
    type/range.inl
)

SET(TYPE_SUCHAS_FILES
    type/such_as.cpp
    type/such_as.h
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
    ${ANIMATION_FILES}
    ${COMMON_FILES}
    ${CONTEXT_FILES}
    ${CONTEXT_FUNCTION_FILES}
    ${DPT_FILES}
    ${FUNCTION_FILES}
    ${FUNCTION_EXPRESSION_FILES}
    ${FUNCTION_LOCAL_VARIABLE_FILES}
    ${GRAMMAR_FILES}
    ${GRAMMAR_GENERATED_FILES}
    ${PARSER_FILES}
    ${PARSER_ERROR_FILES}
    ${PARSER_LEXER_FILES}
    ${PATTERN_FILES}
    ${PCH_FILES}
    ${PROJECT_FILES}
    ${RESORCES_FILES}
    ${RESORCES_PARAM_FILES}
    ${TYPE_FILES}
    ${TYPE_FUNCTION_FILES}
    ${TYPE_INFO_FILES}
    ${TYPE_RUNTIMEWRAPPER_FILES}
    ${TYPE_RUNTIMEWRAPPER_ARRAY_FILES}
    ${TYPE_RUNTIMEWRAPPER_ATOM_FILES}
    ${TYPE_RUNTIMEWRAPPER_ENUM_FILES}
    ${TYPE_RUNTIMEWRAPPER_RANGE_FILES}
    ${TYPE_SUCHAS_FILES}
    ${VALUE_FILES}
    ${VALUE_ARRAY_FILES}
    ${WATCH_FILES}
)

SET(NO_PCH_FILES
    ${GRAMMAR_GENERATED_NO_PCH}
    ${PCH_FILES}
)

ADD_LIBRARY(rdo_parser ${SOURCE_FILES})

SET_TARGET_PROPERTIES(rdo_parser PROPERTIES FOLDER ${LIBRARY_FOLDERS})

ADD_DEPENDENCIES(rdo_parser rdo_utils)
ADD_DEPENDENCIES(rdo_parser rdo_runtime)

TARGET_LINK_LIBRARIES(rdo_parser ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_parser rdo_utils)
TARGET_LINK_LIBRARIES(rdo_parser rdo_runtime)

IF(MSVC)
    SET_TARGET_PROPERTIES(rdo_parser PROPERTIES COMPILE_FLAGS "-D_UNISTD_H_")
ENDIF(MSVC)

INSTALL(TARGETS rdo_parser DESTINATION lib)

SET(RDO_PARSER_TEST_PATH ${CMAKE_CURRENT_SOURCE_DIR}/test)
#ADD_SUBDIRECTORY(${RDO_PARSER_TEST_PATH})

IF(MSVC) # options for windows #

    INCLUDE(${PROJECT_SOURCE_DIR}/cmake/pch.cmake)
    ADD_PCH("${SOURCE_FILES}" "${NO_PCH_FILES}" pch simulator/compiler/parser)

    SOURCE_GROUP(".animation" FILES
        ${ANIMATION_FILES}
    )

    SOURCE_GROUP(".common" FILES
        ${COMMON_FILES}
    )

    SOURCE_GROUP(".context" FILES
        ${CONTEXT_FILES}
    )

    SOURCE_GROUP(".context\\.function" FILES
        ${CONTEXT_FUNCTION_FILES}
    )

    SOURCE_GROUP(".dpt" FILES
        ${DPT_FILES}
    )

    SOURCE_GROUP(".function" FILES
        ${FUNCTION_FILES}
    )

    SOURCE_GROUP(".function\\.expression" FILES
        ${FUNCTION_EXPRESSION_FILES}
    )

    SOURCE_GROUP(".function\\.local_variable" FILES
        ${FUNCTION_LOCAL_VARIABLE_FILES}
    )

    SOURCE_GROUP(".grammar" FILES
        ${GRAMMAR_FILES}
    )

    SOURCE_GROUP(".grammar\\generated" FILES
        ${GRAMMAR_GENERATED_FILES}
    )

    SOURCE_GROUP(".parser" FILES
        ${PARSER_FILES}
    )

    SOURCE_GROUP(".parser\\.error" FILES
        ${PARSER_ERROR_FILES}
    )

    SOURCE_GROUP(".parser\\.lexer" FILES
        ${PARSER_LEXER_FILES}
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

    SOURCE_GROUP(".resorces" FILES
        ${RESORCES_FILES}
    )

    SOURCE_GROUP(".resorces\\.param" FILES
        ${RESORCES_PARAM_FILES}
    )

    SOURCE_GROUP(".type" FILES
        ${TYPE_FILES}
    )

    SOURCE_GROUP(".type\\.function" FILES
        ${TYPE_FUNCTION_FILES}
    )

    SOURCE_GROUP(".type\\.info" FILES
        ${TYPE_INFO_FILES}
    )

    SOURCE_GROUP(".type\\.runtime_wrapper" FILES
        ${TYPE_RUNTIMEWRAPPER_FILES}
    )

    SOURCE_GROUP(".type\\.runtime_wrapper\\.array" FILES
        ${TYPE_RUNTIMEWRAPPER_ARRAY_FILES}
    )

    SOURCE_GROUP(".type\\.runtime_wrapper\\.atom" FILES
        ${TYPE_RUNTIMEWRAPPER_ATOM_FILES}
    )

    SOURCE_GROUP(".type\\.runtime_wrapper\\.enum" FILES
        ${TYPE_RUNTIMEWRAPPER_ENUM_FILES}
    )

    SOURCE_GROUP(".type\\.runtime_wrapper\\.range" FILES
        ${TYPE_RUNTIMEWRAPPER_RANGE_FILES}
    )

    SOURCE_GROUP(".type\\.such_as" FILES
        ${TYPE_SUCHAS_FILES}
    )

    SOURCE_GROUP(".value" FILES
        ${VALUE_FILES}
    )

    SOURCE_GROUP(".value\\.array" FILES
        ${VALUE_ARRAY_FILES}
    )

    SOURCE_GROUP(".watch" FILES
        ${WATCH_FILES}
    )

ENDIF(MSVC)
