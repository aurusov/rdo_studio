#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
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

SET(GRAMMA_PATH ${CMAKE_CURRENT_SOURCE_DIR}/grammar)

CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/cmake/rdogramma.h.cmake ${GRAMMA_PATH}/rdogramma.h)

BISON_TARGET(rdortp ${GRAMMA_PATH}/rdortp.y ${GRAMMA_PATH}/rdogramrtp.cpp COMPILE_FLAGS "-g -v -prtp")
BISON_TARGET(rdodpt ${GRAMMA_PATH}/rdodpt.y ${GRAMMA_PATH}/rdogramdpt.cpp COMPILE_FLAGS "-g -v -pdpt")
BISON_TARGET(rdoevn ${GRAMMA_PATH}/rdoevn.y ${GRAMMA_PATH}/rdogramevn.cpp COMPILE_FLAGS "-g -v -pevn")
BISON_TARGET(rdoevn_preparse ${GRAMMA_PATH}/rdoevn_preparse.y ${GRAMMA_PATH}/rdogramevn_preparse.cpp COMPILE_FLAGS "-g -v -pevn_preparse_")
BISON_TARGET(rdofrm ${GRAMMA_PATH}/rdofrm.y ${GRAMMA_PATH}/rdogramfrm.cpp COMPILE_FLAGS "-g -v -pfrm")
BISON_TARGET(rdofun ${GRAMMA_PATH}/rdofun.y ${GRAMMA_PATH}/rdogramfun.cpp COMPILE_FLAGS "-g -v -pfun")
BISON_TARGET(rdopat ${GRAMMA_PATH}/rdopat.y ${GRAMMA_PATH}/rdogrampat.cpp COMPILE_FLAGS "-g -v -ppat")
BISON_TARGET(rdopmd ${GRAMMA_PATH}/rdopmd.y ${GRAMMA_PATH}/rdogrampmd.cpp COMPILE_FLAGS "-g -v -ppmd")
BISON_TARGET(rdoproc_opr ${GRAMMA_PATH}/rdoproc_opr.y ${GRAMMA_PATH}/rdogramproc_opr.cpp COMPILE_FLAGS "-g -v -pproc_opr_")
BISON_TARGET(rdoproc_rss ${GRAMMA_PATH}/rdoproc_rss.y ${GRAMMA_PATH}/rdogramproc_rss.cpp COMPILE_FLAGS "-g -v -pproc_rss_")
BISON_TARGET(rdoproc_rtp ${GRAMMA_PATH}/rdoproc_rtp.y ${GRAMMA_PATH}/rdogramproc_rtp.cpp COMPILE_FLAGS "-g -v -pproc_rtp_")
BISON_TARGET(rdorss ${GRAMMA_PATH}/rdorss.y ${GRAMMA_PATH}/rdogramrss.cpp COMPILE_FLAGS "-g -v -prss")
BISON_TARGET(rdosmr_file ${GRAMMA_PATH}/rdosmr_file.y ${GRAMMA_PATH}/rdogramsmr_file.cpp COMPILE_FLAGS "-g -v -psmr_file_")
BISON_TARGET(rdosmr_sim ${GRAMMA_PATH}/rdosmr_sim.y ${GRAMMA_PATH}/rdogramsmr_sim.cpp COMPILE_FLAGS "-g -v -psmr_sim_")

FLEX_TARGET(lexer_scaner ${GRAMMA_PATH}/rdo_lexer.l ${GRAMMA_PATH}/rdolex.cpp COMPILE_FLAGS "-+")

SET(GENERATED_FILES
    ${GRAMMA_PATH}/rdogramma.h
    ${FLEX_lexer_scaner_OUTPUTS}
    ${BISON_rdortp_OUTPUTS}
    ${BISON_rdortp_OUTPUT_HEADER}
    ${BISON_rdodpt_OUTPUTS}
    ${BISON_rdoevn_OUTPUTS}
    ${BISON_rdoevn_preparse_OUTPUTS}
    ${BISON_rdofrm_OUTPUTS}
    ${BISON_rdofun_OUTPUTS}
    ${BISON_rdopat_OUTPUTS}
    ${BISON_rdopmd_OUTPUTS}
    ${BISON_rdoproc_opr_OUTPUTS}
    ${BISON_rdoproc_rss_OUTPUTS}
    ${BISON_rdoproc_rtp_OUTPUTS}
    ${BISON_rdorss_OUTPUTS}
    ${BISON_rdosmr_file_OUTPUTS}
    ${BISON_rdosmr_sim_OUTPUTS}
    )

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/parser)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/context)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/type)

SET(HEADER_FILES
    ${GRAMMA_PATH}/rdobison.h
    rdortp.h
    expression.h
    src/function/local_variable/local_variable.h
    src/function/local_variable/local_variable_context.h
    namespace.h
    param.h
    pch.h
    rdo_array.h
    rdodpt.h
    rdofun.h
    rdo_logic_base.h
    rdo_logic.h
    rdo_object.h
    rdoparser_base.h
    rdoparser_corba.h
    rdoparser_error.h
    rdoparser.h
    rdoparser_lexer.h
    rdoparser_rdo.h
    src/animation/animation_base.h
    src/animation/animation_frame.h
    src/animation/animation_sprite.h
    src/function/function.h
    rdopat.h
    rdopatpreparse.h
    rdopmd.h
    rdoprocess.h
    rdorss.h
    rdortp.h
    rdortp_param.h
    rdosmr.h
    rdo_value.h
    bison_value_pair.h
    # context path
    context/context_create_calc_i.h
    context/context_find_i.h
    context/context_switch_i.h
    context/context.h
    context/memory.h
    context/statement.h
    context/stack.h
    # parses paht
    parser/std_fun.h
    # type path
    type/array.h
    type/atom.h
    type/enum.h
    type/info.h
    type/range.h
    type/such_as.h
    type/type.h
    type/function_type.h
    type/runtime_wrapper_type.h
    context/context_type.h
    context/function/context_function_body.h
    context/function/context_function_param_definition.h
    )

SET(INLITE_FILES
    rdo_logic.inl
    bison_value_pair.inl
    rdoparser_lexer.inl
    rdopatpreparse.inl
    rdo_value.inl
    # context path
    context/context.inl
    # type path
    type/info.inl
    type/range.inl
    )

SET(SOURCE_FILES
    rdortp.cpp
    expression.cpp
    src/function/local_variable/local_variable.cpp
    src/function/local_variable/local_variable_context.cpp
    param.cpp
    pch.cpp
    rdo_array.cpp
    rdodpt.cpp
    rdofun.cpp
    rdo_logic_base.cpp
    rdo_logic.cpp
    rdo_object.cpp
    rdoparser_base.cpp
    rdoparser_corba.cpp
    rdoparser.cpp
    rdoparser_error.cpp
    rdoparser_lexer.cpp
    rdoparser_rdo.cpp
    src/animation/animation_base.cpp
    src/animation/animation_frame.cpp
    src/animation/animation_sprite.cpp
    src/function/function.cpp
    rdopat.cpp
    rdopmd.cpp
    rdoprocess.cpp
    rdorss.cpp
    rdortp.cpp
    rdortp_param.cpp
    rdosmr.cpp
    rdo_value.cpp
    # context path
    context/context_find.cpp
    context/context.cpp
    context/memory.cpp
    context/statement.cpp
    context/stack.cpp
    # parses paht
    parser/std_fun.cpp
    # type path
    type/array.cpp
    type/atom.cpp
    type/enum.cpp
    type/info.cpp
    type/such_as.cpp
    type/type.cpp
    type/range.cpp
    type/function_type.cpp
    type/runtime_wrapper_type.cpp
    context/context_type.cpp
    context/function/context_function_body.cpp
    context/function/context_function_param_definition.cpp
    )

SET(BISON_FILES
    # *.y files
    ${GRAMMA_PATH}/rdodpt.y
    ${GRAMMA_PATH}/rdoevn_preparse.y
    ${GRAMMA_PATH}/rdoevn.y
    ${GRAMMA_PATH}/rdofrm.y
    ${GRAMMA_PATH}/rdofun.y
    ${GRAMMA_PATH}/rdopat.y
    ${GRAMMA_PATH}/rdopmd.y
    ${GRAMMA_PATH}/rdoproc_opr.y
    ${GRAMMA_PATH}/rdoproc_rss.y
    ${GRAMMA_PATH}/rdoproc_rtp.y
    ${GRAMMA_PATH}/rdorss.y
    ${GRAMMA_PATH}/rdortp.y
    ${GRAMMA_PATH}/rdosmr_file.y
    ${GRAMMA_PATH}/rdosmr_sim.y
    ${GRAMMA_PATH}/rdo_lexer.l
    )

ADD_LIBRARY(rdo_parser ${HEADER_FILES} ${INLITE_FILES} ${SOURCE_FILES} ${BISON_FILES} ${GENERATED_FILES})
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

IF(MSVC) # lotions for windows #

SOURCE_GROUP(".animation" FILES 
    rdofrm.h
    rdofrm.cpp
    )

SOURCE_GROUP(".bison" FILES 
    ${BISON_FILES}
    ${GRAMMA_PATH}/rdobison.h
    )
    
SOURCE_GROUP(".common" FILES 
    local_variable.h
    local_variable.cpp
    rdo_object.h
    rdo_object.cpp
    variable_container.h
    variable_container.cpp
    )    

SOURCE_GROUP(".context" FILES 
    context/context.h
    context/context.inl
    context/context.cpp
    context/context_create_calc_i.h
    context/context_create_expression_i.h
    context/memory.h
    context/memory.cpp
    context/stack.h
    context/stack.cpp
    context/type.h
    context/type.cpp
    context/context_find_i.h
    context/context_find.cpp
    context/context_switch_i.h
    )
    
SOURCE_GROUP(".dpt" FILES
    rdodpt.h
    rdoprocess.h
    rdodpt.cpp
    rdoprocess.cpp
    rdo_logic.h
    rdo_logic.cpp
    )    

SOURCE_GROUP(".expression" FILES
    expression.h
    expression.cpp
    )    

SOURCE_GROUP(".function" FILES
    rdofun.h
    rdofun.cpp
    )    

SOURCE_GROUP(".parser" FILES
    parser/std_fun.h
    parser/std_fun.cpp
    rdoparser_rdo.h
    rdoparser_rdo.cpp
    rdoparser_corba.h
    rdoparser_corba.cpp
    rdoparser_base.h
    rdoparser_base.cpp
    rdoparser.h
    rdoparser.cpp
    )

SOURCE_GROUP(".parser_error" FILES
    rdoparser_error.h
    rdoparser_error.cpp
    )        

SOURCE_GROUP(".parser_lexer" FILES
    rdoparser_lexer.h
    rdoparser_lexer.inl
    rdoparser_lexer.cpp
    )

SOURCE_GROUP(".pattern" FILES
    rdopat.h
    rdopat.cpp
    rdopatpreparse.h
    rdopatpreparse.inl
    )

SOURCE_GROUP(".pch" FILES
    pch.h
    pch.cpp
    )

SOURCE_GROUP(".project" FILES
    rdosmr.h
    rdosmr.cpp
    )

SOURCE_GROUP(".resorces" FILES
    rdortp_param.h
    rdortp_param.cpp
    rdortp.h
    rdortp.cpp
    rdortp.inl
    rdorss.h
    rdorss.cpp
    )

SOURCE_GROUP(".resorces_param" FILES
    param.h
    param.cpp
    )

SOURCE_GROUP(".type" FILES
    type.h
    type.cpp
    type/type.h
    type/info.h
    type/info.inl
    type/info.cpp
    type/runtime_wrapper_type.h
    type/runtime_wrapper_type.cpp
    )

SOURCE_GROUP(".value_array" FILES
    rdo_array.h
    rdo_array.cpp
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

SOURCE_GROUP(".type_array" FILES
    type/array.h
    type/array.cpp
    )

SOURCE_GROUP(".type_atom" FILES
    type/atom.h
    type/atom.cpp
    )

SOURCE_GROUP(".type_enum" FILES
    type/enum.h
    type/enum.cpp
    )

SOURCE_GROUP(".type_range" FILES
    type/range.h
    type/range.inl
    type/range.cpp
    )

SOURCE_GROUP(".type_such_as" FILES
    type/such_as.h
    type/such_as.cpp
    )

SOURCE_GROUP(".generated" FILES
    ${GENERATED_FILES}
    )

ENDIF(MSVC)
