#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO_PARSER  LIBRARY")

INCLUDE_DIRECTORIES(${BISON_FLEX_DIRECTORY})

# Find Boost
SET(Boost_USE_STATIC_LIBS   ON)
SET(Boost_USE_MULTITHREADED ON)
FIND_PACKAGE(Boost COMPONENTS system filesystem thread date_time REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

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

	CONFIGURE_FILE(${CMAKE_MODULE_PATH}/rdogramma.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/rdogramma.h)

	BISON_TARGET(rdortp rdortp.y ${CMAKE_CURRENT_SOURCE_DIR}/rdogramrtp.cpp COMPILE_FLAGS "-g -v -prtp")
	BISON_TARGET(rdodpt rdodpt.y ${CMAKE_CURRENT_SOURCE_DIR}/rdogramdpt.cpp COMPILE_FLAGS "-g -v -pdpt")
	BISON_TARGET(rdoevn rdoevn.y ${CMAKE_CURRENT_SOURCE_DIR}/rdogramevn.cpp COMPILE_FLAGS "-g -v -pevn")
	BISON_TARGET(rdoevn_preparse rdoevn_preparse.y ${CMAKE_CURRENT_SOURCE_DIR}/rdogramevn_preparse.cpp COMPILE_FLAGS "-g -v -pevn_preparse_")
	BISON_TARGET(rdofrm rdofrm.y ${CMAKE_CURRENT_SOURCE_DIR}/rdogramfrm.cpp COMPILE_FLAGS "-g -v -pfrm")
	BISON_TARGET(rdofun rdofun.y ${CMAKE_CURRENT_SOURCE_DIR}/rdogramfun.cpp COMPILE_FLAGS "-g -v -pfun")
	BISON_TARGET(rdopat rdopat.y ${CMAKE_CURRENT_SOURCE_DIR}/rdogrampat.cpp COMPILE_FLAGS "-g -v -ppat")
	BISON_TARGET(rdopmd rdopmd.y ${CMAKE_CURRENT_SOURCE_DIR}/rdogrampmd.cpp COMPILE_FLAGS "-g -v -ppmd")
	BISON_TARGET(rdoproc_opr rdoproc_opr.y ${CMAKE_CURRENT_SOURCE_DIR}/rdogramproc_opr.cpp COMPILE_FLAGS "-g -v -pproc_opr_")
	BISON_TARGET(rdoproc_rss rdoproc_rss.y ${CMAKE_CURRENT_SOURCE_DIR}/rdogramproc_rss.cpp COMPILE_FLAGS "-g -v -pproc_rss_")
	BISON_TARGET(rdoproc_rtp rdoproc_rtp.y ${CMAKE_CURRENT_SOURCE_DIR}/rdogramproc_rtp.cpp COMPILE_FLAGS "-g -v -pproc_rtp_")
	BISON_TARGET(rdorss rdorss.y ${CMAKE_CURRENT_SOURCE_DIR}/rdogramrss.cpp COMPILE_FLAGS "-g -v -prss")
	BISON_TARGET(rdosmr_file rdosmr_file.y ${CMAKE_CURRENT_SOURCE_DIR}/rdogramsmr_file.cpp COMPILE_FLAGS "-g -v -psmr_file_")
	BISON_TARGET(rdosmr_sim rdosmr_sim.y ${CMAKE_CURRENT_SOURCE_DIR}/rdogramsmr_sim.cpp COMPILE_FLAGS "-g -v -psmr_sim_")

	FLEX_TARGET(lexer_scaner rdo_lexer.l ${CMAKE_CURRENT_SOURCE_DIR}/rdolex.cpp COMPILE_FLAGS "--c++")

	ADD_FLEX_BISON_DEPENDENCY(lexer_scaner rdortp)
	ADD_FLEX_BISON_DEPENDENCY(lexer_scaner rdodpt)
	ADD_FLEX_BISON_DEPENDENCY(lexer_scaner rdoevn)
	ADD_FLEX_BISON_DEPENDENCY(lexer_scaner rdoevn_preparse)
	ADD_FLEX_BISON_DEPENDENCY(lexer_scaner rdofrm)
	ADD_FLEX_BISON_DEPENDENCY(lexer_scaner rdopat)
	ADD_FLEX_BISON_DEPENDENCY(lexer_scaner rdopmd)
	ADD_FLEX_BISON_DEPENDENCY(lexer_scaner rdoproc_opr)
	ADD_FLEX_BISON_DEPENDENCY(lexer_scaner rdoproc_rss)
	ADD_FLEX_BISON_DEPENDENCY(lexer_scaner rdoproc_rtp)
	ADD_FLEX_BISON_DEPENDENCY(lexer_scaner rdorss)
	ADD_FLEX_BISON_DEPENDENCY(lexer_scaner rdosmr_file)
	ADD_FLEX_BISON_DEPENDENCY(lexer_scaner rdosmr_sim)

	SET(GENERATED_FILES
		rdogramma.h
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
		${BISON_rdosmr_sim_OUTPUTS})

ELSE(WIN32)

    SET(GENERATED_FILES
        rdogramma.h
        rdogramrtp.cpp
        rdogramdpt.cpp
        rdogramevn.cpp
        rdogramevn_preparse.cpp
        rdogramfrm.cpp
        rdogramfun.cpp
        rdogrampat.cpp
        rdogrampmd.cpp
        rdogramproc_opr.cpp
        rdogramproc_rss.cpp
        rdogramproc_rtp.cpp
        rdogramrss.cpp
        rdogramsmr_file.cpp
        rdogramsmr_sim.cpp
        	    
	    rdolex.cpp)

ENDIF(PROJECT_OS_LINUX)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/parser)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/context)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/type)

SET(GENERATED_FILES
	${GENERATED_FILES})

SET(HEADER_FILES
	expression.h
	local_variable.h
	namespace.h
	param.h
	pch.h
	rdo_array.h
	rdobison.h
	rdodpt.h
	rdofrm.h
	rdofun.h
	rdo_logic.h
	rdo_object.h
	rdoparser_base.h
	rdoparser_corba.h
	rdoparser_error.h
	rdoparser.h
	rdoparser_lexer.h
	rdoparser_rdo.h
	rdopat.h
	rdopatpreparse.h
	rdopmd.h
	rdorss.h
	rdortp.h
	rdortp_param.h
	rdosmr.h
	rdo_value.h
	variable_container.h
	# context path
	context/context_create_calc_i.h
	context/context_create_expression_i.h
	context/context_find_i.h
	context/context_switch_i.h
	context/context.h
	context/memory.h
	context/stack.h
	context/type.h
	# parses paht
	parser/std_fun.h
	# type path
	type/array.h
	type/atom.h
	type/enum.h
	type/info.h
	type/range.h
	type/such_as.h
	type/type.h)

SET(INLITE_FILES
	rdoparser_lexer.inl
	rdopatpreparse.inl
	rdo_value.inl
	# context path
	context/context.inl
	# type path
	type/info.inl
	type/range.inl)

SET(SOURCE_FILES
	expression.cpp
	local_variable.cpp
	param.cpp
	pch.cpp
	rdo_array.cpp
	rdodpt.cpp
	rdofrm.cpp
	rdofun.cpp
	rdo_logic.cpp
	rdo_object.cpp
	rdoparser_base.cpp
	rdoparser_corba.cpp
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
	rdo_value.cpp
	variable_container.cpp
	# context path
	context/context.cpp
	context/memory.cpp
	context/stack.cpp
	context/type.cpp
	# parses paht
	parser/std_fun.cpp
	# type path
	type/array.cpp
	type/atom.cpp
	type/enum.cpp
	type/info.cpp
	type/such_as.cpp)

SET(BISON_FILES
	# *.y files
	rdodpt.y
	rdoevn_preparse.y
	rdoevn.y
	rdofrm.y
	rdofun.y
	rdopat.y
	rdopmd.y
	rdoproc_opr.y
	rdoproc_rss.y
	rdoproc_rtp.y
	rdorss.y
	rdortp.y
	rdosmr_file.y
	rdosmr_sim.y
	rdo_lexer.l)

ADD_LIBRARY(rdo_parser ${HEADER_FILES} ${INLITE_FILES} ${SOURCE_FILES} ${BISON_FILES} ${GENERATED_FILES})

ADD_DEPENDENCIES(rdo_parser rdo_utils)
ADD_DEPENDENCIES(rdo_parser rdo_runtime)

TARGET_LINK_LIBRARIES(rdo_parser ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})
TARGET_LINK_LIBRARIES(rdo_parser rdo_utils)
TARGET_LINK_LIBRARIES(rdo_parser rdo_runtime)

IF(MSVC)
	SET_TARGET_PROPERTIES(rdo_parser PROPERTIES COMPILE_FLAGS "-D_UNISTD_H_")
ENDIF(MSVC)

SET(RDO_PARSER_TEST_PATH ${CMAKE_CURRENT_SOURCE_DIR}/test)
#ADD_SUBDIRECTORY(${RDO_PARSER_TEST_PATH})

IF(MSVC) # lotions for windows #

SOURCE_GROUP(".animation" FILES 
	rdofrm.h
	rdofrm.cpp)

SOURCE_GROUP(".bison" FILES 
	${BISON_FILES}
	rdobison.h)
	
SOURCE_GROUP(".common" FILES 
	local_variable.h
	local_variable.cpp
	namespace.h
	rdo_object.h
	rdo_object.cpp
	variable_container.h
	variable_container.cpp)	

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
	context/context_switch_i.h)
	
SOURCE_GROUP(".dpt" FILES
	rdodpt.h
	rdodpt.cpp
	rdo_logic.h
	rdo_logic.cpp)	

SOURCE_GROUP(".expression" FILES
	expression.h
	expression.cpp)	

SOURCE_GROUP(".function" FILES
	rdofun.h
	rdofun.cpp)	

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
	rdoparser.cpp)

SOURCE_GROUP(".parser_error" FILES
	rdoparser_error.h
	rdoparser_error.cpp)		

SOURCE_GROUP(".parser_lexer" FILES
	rdoparser_lexer.h
	rdoparser_lexer.inl
	rdoparser_lexer.cpp)

SOURCE_GROUP(".pattern" FILES
	rdopat.h
	rdopat.cpp
	rdopatpreparse.h
	rdopatpreparse.inl)

SOURCE_GROUP(".pch" FILES
	pch.h
	pch.cpp)

SOURCE_GROUP(".project" FILES
	rdosmr.h
	rdosmr.cpp)

SOURCE_GROUP(".resorces" FILES
	rdortp_param.h
	rdortp_param.cpp
	rdortp.h
	rdortp.cpp
	rdorss.h
	rdorss.cpp)

SOURCE_GROUP(".resorces_param" FILES
	param.h
	param.cpp)

SOURCE_GROUP(".type" FILES
	type/type.h
	type/info.h
	type/info.inl
	type/info.cpp)

SOURCE_GROUP(".value_array" FILES
	rdo_array.h
	rdo_array.cpp)

SOURCE_GROUP(".value" FILES
	rdo_value.h
	rdo_value.inl
	rdo_value.cpp)

SOURCE_GROUP(".watch" FILES
	rdopmd.h
	rdopmd.cpp)

SOURCE_GROUP(".type_array" FILES
	type/array.h
	type/array.cpp)

SOURCE_GROUP(".type_atom" FILES
	type/atom.h
	type/atom.cpp)

SOURCE_GROUP(".type_enum" FILES
	type/enum.h
	type/enum.cpp)

SOURCE_GROUP(".type_range" FILES
	type/range.h
	type/range.inl)

SOURCE_GROUP(".type_such_as" FILES
	type/such_as.h
	type/such_as.cpp)

SOURCE_GROUP(".generated" FILES
	${GENERATED_FILES})

ENDIF(MSVC)
