#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO_RUNTIME  LIBRARY")

FIND_PACKAGE(Boost COMPONENTS system filesystem thread date_time chrono REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

SET(HEADER_FILES
	keyboard.h
	equaltype.h
	pch/stdpch.h
	namespace.h
	rdo_activity.h
	rdo_activity_i.h
	rdo_array.h
	rdobase.h
	rdodptrtime.h
	rdo_dptsearch_activity.h
	rdo_dptsearch_activity_i.h
	rdo_enum.h
	rdo_event.h
	rdo_event_i.h
	rdo_exception.h
	rdoframe.h
	rdo_fuzzy.h
	rdo.h
	rdo_keyboard.h
	rdo_keyboard_i.h
	rdo_logic_dptprior.h
	rdo_logic_dptsearch.h
	rdo_logic_dptsearch_i.h
	rdo_logic_dptsome.h
	rdo_logic.h
	rdo_logic_i.h
	rdo_matrix.h
	rdo_memory.h
	rdo_model_i.h
	rdo_object.h
	rdo_operation.h
	rdo_operation_i.h
	rdo_param_i.h
	rdo_pattern.h
	rdo_priority.h
	rdo_priority_i.h
	rdo_random_distribution.h
	rdo_resource.h
	rdo_res_type.h
	rdo_res_type_i.h
	rdo_rule.h
	rdo_rule_i.h
	rdo_runtime.h
	notify.h
	error.h
	hotkey.h
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

	process/advance.h
	process/assign.h
	process/generate.h
	process/queue_depart.h
	process/rdoprocess.h
	process/rdoprocess_i.h
	process/seize_release.h
	process/terminate.h

	calc/calc_array.h
	calc/calc_base.h
	calc/calc_event.h
	calc/calc_pattern.h
	calc/calc_process.h
	calc/calc_sequence.h
	calc/calc_watch.h
	calc/function/calc_function.h
	calc/function/calc_function_internal.h
	calc/function/calc_function_system.h
	calc/operation/calc_arithm.h
	calc/operation/calc_binary.h
	calc/operation/calc_logic.h
	calc/operation/calc_unary.h
	calc/operation/operation_name.h
	calc/operation/operation_type.h
	calc/procedural/calc_braces.h
	calc/procedural/calc_const.h
	calc/procedural/calc_locvar.h
	calc/procedural/calc_nop.h
	calc/procedural/calc_range.h
	calc/procedural/calc_statement.h
	calc/resource/calc_choice_from.h
	calc/resource/calc_create_resource.h
	calc/resource/calc_group.h
	calc/resource/calc_relevant.h
	calc/resource/calc_resource.h
	calc/resource/calc_select.h

	result/result_group.h
	result/result_group_i.h
	result/result.h
	result/result_i.h)

SET(INLINE_FILES
	rdo_activity.inl
	rdobase.inl
	rdodptrtime.inl
	rdo_enum.inl
	rdo_exception.inl
	rdoframe.inl
	rdo_fuzzy.inl
	rdo_logic_dptprior.inl
	rdo_logic_dptsome.inl
	rdo_logic.inl
	rdo_object.inl
	rdo_pattern.inl
	rdo_priority.inl
	rdo_random_distribution.inl
	rdo_resource.inl
	rdo_runtime.inl
	rdo_simulator.inl
	rdotrace.inl
	rdo_type.inl
	rdo_value.inl
	rdo_res_type.inl
	searchtrace.inl
	searchtree.inl
	simtrace.inl

	process/advance.inl
	process/assign.inl
	process/generate.inl
	process/queue_depart.inl
	process/rdoprocess.inl
	process/seize_release.inl
	process/terminate.inl

	calc/calc_base.inl
	calc/calc_sequence.inl
	calc/function/calc_function_internal.inl
	calc/operation/calc_arithm.inl
	calc/operation/calc_binary.inl
	calc/operation/calc_logic.inl
	calc/operation/calc_unary.inl
	calc/operation/operation_name.inl
	calc/operation/operation_type.inl
	calc/procedural/calc_locvar.inl
	calc/resource/calc_relevant.inl)

SET(SOURCE_FILES
	pch/stdpch.cpp
	rdo_activity.cpp
	rdo_array.cpp
	rdobase.cpp
	rdo.cpp
	rdo_dptsearch_activity.cpp
	rdo_event.cpp
	rdoframe.cpp
	rdo_fuzzy.cpp
	rdo_keyboard.cpp
	rdo_logic.cpp
	rdo_logic_dptprior.cpp
	rdo_logic_dptsearch.cpp
	rdo_logic_dptsome.cpp
	rdo_matrix.cpp
	rdo_memory.cpp
	rdo_object.cpp
	rdo_operation.cpp
	rdo_pattern.cpp
	rdo_resource.cpp
	rdo_rule.cpp
	rdo_runtime.cpp
	notify.cpp
	error.cpp
	hotkey.cpp
	rdo_simulator.cpp
	rdotrace.cpp
	rdo_type.cpp
	rdo_value.cpp
	rdo_res_type.cpp
	searchtrace.cpp
	searchtree.cpp
	simtrace.cpp

	process/advance.cpp
	process/assign.cpp
	process/generate.cpp
	process/queue_depart.cpp
	process/rdoprocess.cpp
	process/seize_release.cpp
	process/terminate.cpp

	calc/calc_array.cpp
	calc/calc_base.cpp
	calc/calc_event.cpp
	calc/calc_pattern.cpp
	calc/calc_process.cpp
	calc/calc_sequence.cpp
	calc/calc_watch.cpp
	calc/function/calc_function.cpp
	calc/function/calc_function_internal.cpp
	calc/function/calc_function_system.cpp
	calc/operation/calc_arithm.cpp
	calc/operation/calc_logic.cpp
	calc/operation/calc_unary.cpp
	calc/procedural/calc_braces.cpp
	calc/procedural/calc_const.cpp
	calc/procedural/calc_locvar.cpp
	calc/procedural/calc_nop.cpp
	calc/procedural/calc_range.cpp
	calc/procedural/calc_statement.cpp
	calc/resource/calc_choice_from.cpp
	calc/resource/calc_create_resource.cpp
	calc/resource/calc_group.cpp
	calc/resource/calc_relevant.cpp
	calc/resource/calc_resource.cpp
	calc/resource/calc_select.cpp

	result/result.cpp
	result/result_group.cpp)

ADD_LIBRARY(rdo_runtime ${HEADER_FILES} ${INLINE_FILES} ${SOURCE_FILES})
SET_TARGET_PROPERTIES(rdo_runtime PROPERTIES FOLDER ${LIBRARY_FOLDERS})

ADD_DEPENDENCIES(rdo_runtime rdo_utils)
ADD_DEPENDENCIES(rdo_runtime rdo_simulator_report)

TARGET_LINK_LIBRARIES(rdo_runtime ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_runtime rdo_utils)
TARGET_LINK_LIBRARIES(rdo_runtime rdo_simulator_report)
SET_TARGET_PROPERTIES(rdo_runtime PROPERTIES LINKER_LANGUAGE CXX)

INSTALL(TARGETS rdo_runtime DESTINATION lib)

SET(RDO_RUNTIME_TEST_PATH ${CMAKE_CURRENT_SOURCE_DIR}/test)
ADD_SUBDIRECTORY(${RDO_RUNTIME_TEST_PATH})

IF(MSVC) # lotions for windows #

SOURCE_GROUP(".common" FILES 
	namespace.h)

SOURCE_GROUP(".keyboard" FILES 
	keyboard.h)

SOURCE_GROUP(".pch" FILES 
	pch/stdpch.h
	pch/stdpch.cpp)

SOURCE_GROUP(".rdo_value" FILES 
	rdo_value.h
	rdo_value.inl
	rdo_value.cpp)

SOURCE_GROUP(".rdo_type" FILES 
	rdo_type.h
	rdo_type.inl
	rdo_type.cpp)
	
SOURCE_GROUP(".rdo_type_array" FILES 
	rdo_array.h
	rdo_array.cpp)

SOURCE_GROUP(".rdo_type_enum" FILES 
	rdo_enum.h
	rdo_enum.inl)

SOURCE_GROUP(".rdo_type_fuzzy" FILES 
	rdo_fuzzy.h
	rdo_fuzzy.inl
	rdo_fuzzy.cpp)

SOURCE_GROUP(".rdo_type_matrix" FILES 
	rdo_matrix.h
	rdo_matrix.cpp)

SOURCE_GROUP(".rdo_trace" FILES 
	rdotrace.h
	rdotrace.inl
	rdotrace.cpp
	rdotrace_i.h)

SOURCE_GROUP(".interfaces" FILES 
	rdo_model_i.h
	rdo_runtime_interface_registrator.h)

SOURCE_GROUP(".rdo_frame" FILES 
	rdoframe.h
	rdoframe.inl
	rdoframe.cpp)

SOURCE_GROUP(".rdo_fun" FILES 
	rdo_random_distribution.h
	rdo_random_distribution.inl)

SOURCE_GROUP(".rdo_result" FILES 
	result/result.h
	result/result.cpp
	result/result_group.h
	result/result_group.cpp
	result/result_group_i.h
	result/result_i.h)

SOURCE_GROUP(".rdo_calc" FILES 
	calc/arithm.h
	calc/arithm.inl
	calc/arithm.cpp
	calc/array.h
	calc/array.cpp
	calc/binary.h
	calc/binary.inl
	calc/braces.h
	calc/braces.cpp
	calc/choice_from.h
	calc/choice_from.inl
	calc/choice_from.cpp
	equaltype.h
	calc/event_plan.h
	calc/event_plan.cpp
	calc/locvar.h
	calc/locvar.inl
	calc/locvar.cpp
	calc/logic.h
	calc/logic.inl
	calc/logic.cpp
	calc/process_control.h
	calc/process_control.cpp
	rdocalc.h
	rdocalc.inl
	rdocalc.cpp
	calc/relres.h
	calc/relres.inl
	calc/relres.cpp
	calc/sequence.h
	calc/sequence.inl
	calc/sequence.cpp
	calc/statements.h
	calc/statements.cpp
	calc/std_fun.h
	calc/std_fun.inl
	calc/std_fun.cpp
	calc/unary.h
	calc/unary.inl
	calc/unary.cpp
	calc/watch.h
	calc/watch.cpp
	calc/select.h
	calc/select.cpp
	calc/group.h
	calc/group.cpp
	
	calc/calc_array.h
	calc/calc_array.cpp
	calc/calc_base.h
	calc/calc_base.inl
	calc/calc_base.cpp
	calc/calc_event.h
	calc/calc_event.cpp
	calc/calc_pattern.h
	calc/calc_pattern.cpp
	calc/calc_process.h
	calc/calc_process.cpp
	calc/calc_sequence.h
	calc/calc_sequence.inl
	calc/calc_sequence.cpp
	calc/calc_watch.h
	calc/calc_watch.cpp)

SOURCE_GROUP(".rdo_calc_function" FILES 
	calc/function/calc_function.cpp
	calc/function/calc_function.h
	calc/function/calc_function_internal.cpp
	calc/function/calc_function_internal.h
	calc/function/calc_function_internal.inl
	calc/function/calc_function_system.cpp
	calc/function/calc_function_system.h)

SOURCE_GROUP(".rdo_calc_operation" FILES 
	calc/operation/calc_arithm.cpp
	calc/operation/calc_arithm.h
	calc/operation/calc_arithm.inl
	calc/operation/calc_binary.h
	calc/operation/calc_binary.inl
	calc/operation/calc_logic.cpp
	calc/operation/calc_logic.h
	calc/operation/calc_logic.inl
	calc/operation/calc_unary.cpp
	calc/operation/calc_unary.h
	calc/operation/calc_unary.inl
	calc/operation/operation_name.h
	calc/operation/operation_name.inl
	calc/operation/operation_type.h
	calc/operation/operation_type.inl)

SOURCE_GROUP(".rdo_calc_procedural" FILES 
	calc/procedural/calc_braces.cpp
	calc/procedural/calc_braces.h
	calc/procedural/calc_const.cpp
	calc/procedural/calc_const.h
	calc/procedural/calc_locvar.cpp
	calc/procedural/calc_locvar.h
	calc/procedural/calc_locvar.inl
	calc/procedural/calc_nop.cpp
	calc/procedural/calc_nop.h
	calc/procedural/calc_range.cpp
	calc/procedural/calc_range.h
	calc/procedural/calc_statement.cpp
	calc/procedural/calc_statement.h)

SOURCE_GROUP(".rdo_calc_resource" FILES 
	calc/resource/calc_choice_from.cpp
	calc/resource/calc_choice_from.h
	calc/resource/calc_choice_from.inl
	calc/resource/calc_group.cpp
	calc/resource/calc_group.h
	calc/resource/calc_relevant.cpp
	calc/resource/calc_relevant.h
	calc/resource/calc_relevant.inl
	calc/resource/calc_resource.cpp
	calc/resource/calc_resource.h
	calc/resource/calc_select.cpp
	calc/resource/calc_select.h)

SOURCE_GROUP(".rdo_simulator" FILES 
	rdo_exception.h
	rdo_exception.inl
	rdo_exception.cpp
	rdo_memory.h
	rdo_memory.cpp
	rdo_object.h
	rdo_object.inl
	rdo_object.cpp
	rdo_runtime.h
	rdo_runtime.inl
	rdo_runtime.cpp
	notify.h
	notify.inl
	notify.cpp
	error.h
	error.cpp
	hotkey.h
	hotkey.cpp
	rdo_simulator.h
	rdo_simulator.inl
	rdo_simulator.cpp
	rdobase.h
	rdobase.inl
	rdobase.cpp
	simtrace.h
	simtrace.inl
	simtrace.cpp
	thread_proxy_i.h)

SOURCE_GROUP(".rdo_simulator_db" FILES 
	rdo_res_type.h
	rdo_res_type.inl
	rdo_res_type.cpp
	rdo_res_type_i.h
	rdo_resource.h
	rdo_resource.inl
	rdo_resource.cpp)

SOURCE_GROUP(".rdo_simulator_kb_logic" FILES 
	rdo_logic.h
	rdo_logic.inl
	rdo_logic.cpp
	rdo_logic_i.h
	rdo_logic_dptsome.h
	rdo_logic_dptsome.inl
	rdo_logic_dptsome.cpp
	rdo_logic_dptprior.h
	rdo_logic_dptprior.inl
	rdo_logic_dptprior.cpp
	rdo_logic_dptsearch.h
	rdo_logic_dptsearch.cpp
	rdo_logic_dptsearch_i.h
	rdodptrtime.h
	rdodptrtime.inl
	rdodptrtime.cpp
	searchtrace.h
	searchtrace.inl
	searchtrace.cpp
	searchtrace_i.h
	searchtree.h
	searchtree.inl
	searchtree.cpp
	rdoprocess.h
	rdoprocess.inl
	rdoprocess.cpp
	rdoprocess_i.h)

SOURCE_GROUP(".rdo_simulator_kb_rdo_operations" FILES 
	rdo.h
	rdo.cpp
	rdo_activity.h
	rdo_activity.inl
	rdo_activity.cpp
	rdo_activity_i.h
	rdo_param_i.h
	rdo_pattern.h
	rdo_pattern.inl
	rdo_pattern.cpp
	rdo_priority.h
	rdo_priority.inl
	rdo_priority_i.h
	rdo_event.h
	rdo_event.cpp
	rdo_event_i.h
	rdo_rule.h
	rdo_rule.cpp
	rdo_rule_i.h
	rdo_operation.h
	rdo_operation.cpp
	rdo_operation_i.h
	rdo_keyboard.h
	rdo_keyboard.cpp
	rdo_keyboard_i.h
	rdo_dptsearch_activity.h
	rdo_dptsearch_activity.cpp
	rdo_dptsearch_activity_i.h)

ENDIF(MSVC)
