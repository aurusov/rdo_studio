CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

MESSAGE(STATUS "CREATE RDO_RUNTIME LIBRARY PROJECT")

# Find Boost
IF(MSVC)
    SET(Boost_USE_STATIC_LIBS   ON)
    SET(Boost_USE_MULTITHREADED ON)
ENDIF(MSVC)
FIND_PACKAGE(Boost COMPONENTS system filesystem thread date_time REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

SET(HEADER_FILES
        equaltype.h
        pch.h
        rdo_activity.h
        rdo_activity_i.h
        rdo_array_def.h
        rdo_array.h
        rdobase.h
        rdocalc.h
        rdodptrtime.h
        rdo_dptsearch_activity.h
        rdo_dptsearch_activity_i.h
        rdo_enum.h
        rdo_event.h
        rdo_event_i.h
        rdo_exception.h
        rdoframe.h
        rdo_fuzzy_def.h
        rdo_fuzzy.h
        rdo.h
        rdo_keyboard.h
        rdo_keyboard_i.h
        rdo_logic_dptfree.h
        rdo_logic_dptprior.h
        rdo_logic_dptsearch.h
        rdo_logic_dptsearch_i.h
        rdo_logic_dptsome.h
        rdo_logic.h
        rdo_logic_i.h
        rdo_matrix_def.h
        rdo_matrix.h
        rdo_memory.h
        rdo_model_i.h
        rdo_object.h
        rdo_operation.h
        rdo_operation_i.h
        rdo_param_i.h
        rdo_pattern.h
        rdopokaz_group.h
        rdopokaz_group_i.h
        rdopokaz.h
        rdopokaz_i.h
        rdo_priority.h
        rdo_priority_i.h
        rdoprocess.h
        rdoprocess_i.h
        rdo_random_distribution.h
        rdo_resource.h
        rdo_res_type.h
        rdo_res_type_i.h
        rdo_rule.h
        rdo_rule_i.h
        rdo_runtime.h
        rdo_runtime_interface_registrator.h
        rdo_simulator.h
        rdotrace.h
        rdotrace_i.h
        rdo_type.h
        rdo_value.h
        searchtrace.h
        searchtrace_i.h
        searchtree.h
        simtrace.h
        thread_proxy_i.h
        calc/arithm.h
        calc/array.h
        calc/binary.h
        calc/braces.h
        calc/choice_from.h
        calc/event_plan.h
        calc/locvar.h
        calc/logic.h
        calc/operation_name.h
        calc/operation_type.h
        calc/process_control.h
        calc/relres.h
        calc/sequence.h
        calc/statements.h
        calc/std_fun.h
        calc/unary.h
        calc/watch.h)

SET(INLITE_FILES
        rdo_activity.inl
        rdobase.inl
        rdocalc.inl
        rdodptrtime.inl
        rdo_enum.inl
        rdo_exception.inl
        rdoframe.inl
        rdo_fuzzy.inl
        rdo_logic_dptfree.inl
        rdo_logic_dptprior.inl
        rdo_logic_dptsome.inl
        rdo_logic.inl
        rdo_object.inl
        rdo_pattern.inl
        rdo_priority.inl
        rdoprocess.inl
        rdo_random_distribution.inl
        rdo_resource.inl
        rdo_runtime.inl
        rdo_simulator.inl
        rdotrace.inl
        rdo_type.inl
        rdo_value.inl
        searchtrace.inl
        searchtree.inl
        simtrace.inl
        calc/arithm.inl
        calc/binary.inl
        calc/choice_from.inl
        calc/locvar.inl
        calc/logic.inl
        calc/operation_name.inl
        calc/operation_type.inl
        calc/relres.inl
        calc/sequence.inl
        calc/std_fun.inl
        calc/unary.inl)

SET(SOURCE_FILE
        pch.cpp
        rdo_activity.cpp
        rdo_array.cpp
        rdobase.cpp
        rdocalc.cpp
        rdo.cpp
        rdo_dptsearch_activity.cpp
        rdo_event.cpp
        rdoframe.cpp
        rdo_fuzzy.cpp
        rdo_keyboard.cpp
        rdo_logic.cpp
        rdo_logic_dptfree.cpp
        rdo_logic_dptprior.cpp
        rdo_logic_dptsearch.cpp
        rdo_logic_dptsome.cpp
        rdo_matrix.cpp
        rdo_memory.cpp
        rdo_object.cpp
        rdo_operation.cpp
        rdo_pattern.cpp
        rdopokaz.cpp
        rdopokaz_group.cpp
        rdoprocess.cpp
        rdo_resource.cpp
        rdo_res_type.cpp
        rdo_rule.cpp
        rdo_runtime.cpp
        rdo_simulator.cpp
        rdotrace.cpp
        rdo_type.cpp
        rdo_value.cpp
        searchtrace.cpp
        searchtree.cpp
        simtrace.cpp
        calc/arithm.cpp
        calc/array.cpp
        calc/braces.cpp
        calc/choice_from.cpp
        calc/event_plan.cpp
        calc/locvar.cpp
        calc/logic.cpp
        calc/process_control.cpp
        calc/relres.cpp
        calc/sequence.cpp
        calc/statements.cpp
        calc/std_fun.cpp
        calc/unary.cpp
        calc/watch.cpp)

ADD_LIBRARY(rdo_nix_runtime ${HEADER_FILES} ${INLITE_FILES} ${SOURCE_FILES})
ADD_DEPENDENCIES(test_rdo_utils rdo_nix_utils)
TARGET_LINK_LIBRARIES(rdo_nix_runtime rdo_nix_utils)
SET_TARGET_PROPERTIES(rdo_nix_runtime PROPERTIES LINKER_LANGUAGE CXX)
TARGET_LINK_LIBRARIES(rdo_nix_runtime ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})

IF(MSVC) # lotions for windows #

ENDIF(MSVC)
