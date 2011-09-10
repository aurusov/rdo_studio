CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

MESSAGE(STATUS "CREATE RDO_COMMON LIBRARY PROJECT")

# Find Boost
SET(Boost_USE_STATIC_LIBS   ON)
SET(Boost_USE_MULTITHREADED ON)
FIND_PACKAGE(Boost COMPONENTS system filesystem thread date_time REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/parser)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/context)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/type)

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
        # *.l files
        rdo_lexer.l)

ADD_LIBRARY(rdo_nix_parser ${HEADER_FILES} ${INLITE_FILES} ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(rdo_nix_parser ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})
TARGET_LINK_LIBRARIES(rdo_nix_parser rdo_nix_common)

SET(RDO_PARSER_TEST_PATH ${CMAKE_CURRENT_SOURCE_DIR}/test)
ADD_SUBDIRECTORY(${RDO_PARSER_TEST_PATH})
