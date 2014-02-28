#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>,
#               2013 Andrey Urusov <drobus@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO_RUNTIME  LIBRARY")

FIND_PACKAGE(Boost COMPONENTS system filesystem thread date_time chrono REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

SET(CALC_FILES
	calc/calc_array.cpp
	calc/calc_array.h
	calc/calc_base.cpp
	calc/calc_base.h
	calc/calc_base.inl
	calc/calc_event.cpp
	calc/calc_event.h
	calc/calc_pattern.cpp
	calc/calc_pattern.h
	calc/calc_process.h
	calc/calc_process.cpp
	calc/calc_sequence.cpp
	calc/calc_sequence.h
	calc/calc_sequence.inl
	calc/calc_watch.cpp
	calc/calc_watch.h
)

SET(CALC_FUNCTION_FILES
	calc/function/calc_function.cpp
	calc/function/calc_function.h
	calc/function/calc_function_internal.cpp
	calc/function/calc_function_internal.h
	calc/function/calc_function_internal.inl
	calc/function/calc_function_system.cpp
	calc/function/calc_function_system.h
)

SET(CALC_OPERATION_FILES
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
	calc/operation/operation_type.inl
)

SET(CALC_PROCEDURAL_FILES
	calc/procedural/calc_braces.cpp
	calc/procedural/calc_braces.h
	calc/procedural/calc_const.cpp
	calc/procedural/calc_const.h
	calc/procedural/calc_locvar.cpp
	calc/procedural/calc_locvar.h
	calc/procedural/calc_nop.cpp
	calc/procedural/calc_nop.h
	calc/procedural/calc_range.cpp
	calc/procedural/calc_range.h
	calc/procedural/calc_statement.cpp
	calc/procedural/calc_statement.h
)

SET(CALC_RESOURCE_FILES
	calc/resource/calc_choice_from.cpp
	calc/resource/calc_choice_from.h
	calc/resource/calc_create_resource.cpp
	calc/resource/calc_create_resource.h
	calc/resource/calc_group.cpp
	calc/resource/calc_group.h
	calc/resource/calc_relevant.cpp
	calc/resource/calc_relevant.h
	calc/resource/calc_relevant.inl
	calc/resource/calc_resource.cpp
	calc/resource/calc_resource.h
	calc/resource/calc_resource.inl
	calc/resource/calc_select.cpp
	calc/resource/calc_select.h
)

SET(COMMON_FILES
	namespace.h
	mode.h
	set_operation_type.cpp
	set_operation_type.h
)

SET(FRAME_FILES
	rdoframe.cpp
	rdoframe.h
	rdoframe.inl
)

SET(FUN_FILES
	rdo_random_distribution.h
	rdo_random_distribution.inl
)

SET(INTERFACES_FILES
	rdo_model_i.h
	rdo_runtime_interface_registrator.h
)

SET(PCH_FILES
	pch/stdpch.cpp
	pch/stdpch.h
)

SET(RESULT_FILES
	result/result.cpp
	result/result.h
	result/result_group.cpp
	result/result_group.h
	result/result_group_i.h
	result/result_i.h
)

SET(SIMULATOR_FILES
	keyboard.h
	rdo_exception.h
	rdo_exception.inl
	rdo_memory.cpp
	rdo_memory.h
	rdo_object.cpp
	rdo_object.h
	rdo_object.inl
	rdo_runtime.cpp
	rdo_runtime.h
	rdo_runtime.inl
	rdo_simulator.cpp
	rdo_simulator.h
	rdo_simulator.inl
	rdobase.cpp
	rdobase.h
	rdobase.inl
	simtrace.cpp
	simtrace.h
	simtrace.inl
	thread_proxy_i.h
)

SET(SIMULATOR_ERROR_FILES
	error.cpp
	error.h
)

SET(SIMULATOR_HOTKEY_FILES
	hotkey.cpp
	hotkey.h
)

SET(SIMULATOR_NOTIFY_FILES
	notify.cpp
	notify.h
	notify_i.h
)

SET(SIMULATOR_DB_FILES
	rdo_res_type.cpp
	rdo_res_type.h
	rdo_res_type_i.h
	rdo_resource.cpp
	rdo_resource.h
)

SET(SIMULATOR_KB_LOGIC_BASE_FILES
	rdo_logic.cpp
	rdo_logic.h
	rdo_logic.inl
	rdo_logic_i.h
)

SET(SIMULATOR_KB_LOGIC_DPTSOME_FILES
	rdo_logic_dptsome.cpp
	rdo_logic_dptsome.h
	rdo_logic_dptsome.inl
)

SET(SIMULATOR_KB_LOGIC_DPTPRIOR_FILES
	rdo_logic_dptprior.cpp
	rdo_logic_dptprior.h
	rdo_logic_dptprior.inl
)

SET(SIMULATOR_KB_LOGIC_DPTSEARCH_FILES
	rdo_logic_dptsearch.cpp
	rdo_logic_dptsearch.h
	rdo_logic_dptsearch_i.h
	rdodptrtime.h
	rdodptrtime.inl
	searchtrace.cpp
	searchtrace.h
	searchtrace.inl
	searchtrace_i.h
	searchtree.cpp
	searchtree.h
	searchtree.inl
)

SET(SIMULATOR_KB_LOGIC_PROCESS_FILES
	process/advance.cpp
	process/advance.h
	process/advance.inl
	process/assign.cpp
	process/assign.h
	process/assign.inl
	process/generate.cpp
	process/generate.h
	process/generate.inl
	process/queue_depart.cpp
	process/queue_depart.h
	process/queue_depart.inl
	process/rdoprocess.cpp
	process/rdoprocess.h
	process/rdoprocess.inl
	process/rdoprocess_i.h
	process/seize_release.cpp
	process/seize_release.h
	process/seize_release.inl
	process/terminate.cpp
	process/terminate.h
	process/terminate.inl
)

SET(SIMULATOR_KB_OPERATIONS_BASE_FILES
	rdo.cpp
	rdo.h
	rdo_activity.cpp
	rdo_activity.h
	rdo_activity.inl
	rdo_activity_i.h
	rdo_param_i.h
	rdo_pattern.cpp
	rdo_pattern.h
	rdo_pattern.inl
	rdo_priority.h
	rdo_priority.inl
	rdo_priority_i.h
)

SET(SIMULATOR_KB_OPERATIONS_EVENT_FILES
	rdo_event.cpp
	rdo_event.h
	rdo_event_i.h
)

SET(SIMULATOR_KB_OPERATIONS_RULE_FILES
	rdo_rule.cpp
	rdo_rule.h
	rdo_rule_i.h
)

SET(SIMULATOR_KB_OPERATIONS_OPERATION_FILES
	rdo_operation.cpp
	rdo_operation.h
	rdo_operation_i.h
)

SET(SIMULATOR_KB_OPERATIONS_KEYBOARD_FILES
	rdo_keyboard.cpp
	rdo_keyboard.h
	rdo_keyboard_i.h
)

SET(SIMULATOR_KB_OPERATIONS_DPTSEARCH_ACTIVITY_FILES
	rdo_dptsearch_activity.cpp
	rdo_dptsearch_activity.h
	rdo_dptsearch_activity_i.h
)

SET(TRACE_FILES
	rdotrace.cpp
	rdotrace.h
	rdotrace.inl
	rdotrace_i.h
)

SET(TYPE_FILES
	rdo_type.cpp
	rdo_type.h
	rdo_type.inl
)

SET(TYPE_ARRAY_FILES
	rdo_array.cpp
	rdo_array.h
)

SET(TYPE_ENUM_FILES
	rdo_enum.h
	rdo_enum.inl
)

SET(TYPE_FUZZY_FILES
	rdo_fuzzy.cpp
	rdo_fuzzy.h
	rdo_fuzzy.inl
)

SET(TYPE_MATRIX_FILES
	rdo_matrix.cpp
	rdo_matrix.h
)

SET(VALUE_FILES
	rdo_value.cpp
	rdo_value.h
	rdo_value.inl
)

SET(SOURCE_FILES
	${PCH_FILES}
	${CALC_FILES}
	${CALC_FUNCTION_FILES}
	${CALC_OPERATION_FILES}
	${CALC_PROCEDURAL_FILES}
	${CALC_RESOURCE_FILES}
	${COMMON_FILES}
	${FRAME_FILES}
	${FUN_FILES}
	${INTERFACES_FILES}
	${RESULT_FILES}
	${SIMULATOR_FILES}
	${SIMULATOR_ERROR_FILES}
	${SIMULATOR_HOTKEY_FILES}
	${SIMULATOR_NOTIFY_FILES}
	${SIMULATOR_DB_FILES}
	${SIMULATOR_KB_LOGIC_BASE_FILES}
	${SIMULATOR_KB_LOGIC_DPTSOME_FILES}
	${SIMULATOR_KB_LOGIC_DPTPRIOR_FILES}
	${SIMULATOR_KB_LOGIC_DPTSEARCH_FILES}
	${SIMULATOR_KB_LOGIC_PROCESS_FILES}
	${SIMULATOR_KB_OPERATIONS_BASE_FILES}
	${SIMULATOR_KB_OPERATIONS_EVENT_FILES}
	${SIMULATOR_KB_OPERATIONS_RULE_FILES}
	${SIMULATOR_KB_OPERATIONS_OPERATION_FILES}
	${SIMULATOR_KB_OPERATIONS_KEYBOARD_FILES}
	${SIMULATOR_KB_OPERATIONS_DPTSEARCH_ACTIVITY_FILES}
	${TRACE_FILES}
	${TYPE_FILES}
	${TYPE_ARRAY_FILES}
	${TYPE_ENUM_FILES}
	${TYPE_FUZZY_FILES}
	${TYPE_MATRIX_FILES}
	${VALUE_FILES}
)

SET(NO_PCH_FILES
	${PCH_FILES}
)

ADD_LIBRARY(rdo_runtime STATIC ${SOURCE_FILES})

INCLUDE(${PROJECT_SOURCE_DIR}/simulator/simulator.cmake)
SET_TARGET_PROPERTIES(rdo_runtime PROPERTIES FOLDER ${PROJECT_FOLDER}/.runtime)

ADD_DEPENDENCIES(rdo_runtime rdo_utils)
ADD_DEPENDENCIES(rdo_runtime rdo_simulator_report)

TARGET_LINK_LIBRARIES(rdo_runtime ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(rdo_runtime rdo_utils)
TARGET_LINK_LIBRARIES(rdo_runtime rdo_simulator_report)
SET_TARGET_PROPERTIES(rdo_runtime PROPERTIES LINKER_LANGUAGE CXX)

INSTALL(TARGETS rdo_runtime DESTINATION lib)

SET(RDO_RUNTIME_TEST_PATH ${CMAKE_CURRENT_SOURCE_DIR}/test)
ADD_SUBDIRECTORY(${RDO_RUNTIME_TEST_PATH})

IF(MSVC) # options for windows #

	INCLUDE(${PROJECT_SOURCE_DIR}/cmake/pch.cmake)
	ADD_PCH("${SOURCE_FILES}" "${NO_PCH_FILES}" simulator/runtime pch stdpch)

	SOURCE_GROUP(".calc" FILES
		${CALC_FILES}
	)

	SOURCE_GROUP(".calc\\.function" FILES
		${CALC_FUNCTION_FILES}
	)

	SOURCE_GROUP(".calc\\.operation" FILES
		${CALC_OPERATION_FILES}
	)

	SOURCE_GROUP(".calc\\.procedural" FILES
		${CALC_PROCEDURAL_FILES}
	)

	SOURCE_GROUP(".calc\\.resource" FILES
		${CALC_RESOURCE_FILES}
	)

	SOURCE_GROUP(".common" FILES
		${COMMON_FILES}
	)

	SOURCE_GROUP(".frame" FILES
		${FRAME_FILES}
	)

	SOURCE_GROUP(".fun" FILES
		${FUN_FILES}
	)

	SOURCE_GROUP(".interfaces" FILES
		${INTERFACES_FILES}
	)

	SOURCE_GROUP(".pch" FILES
		${PCH_FILES}
	)

	SOURCE_GROUP(".result" FILES
		${RESULT_FILES}
	)

	SOURCE_GROUP(".simulator" FILES
		${SIMULATOR_FILES}
	)

	SOURCE_GROUP(".simulator\\.error" FILES
		${SIMULATOR_ERROR_FILES}
	)

	SOURCE_GROUP(".simulator\\.hotkey" FILES
		${SIMULATOR_HOTKEY_FILES}
	)

	SOURCE_GROUP(".simulator\\.notify" FILES
		${SIMULATOR_NOTIFY_FILES}
	)

	SOURCE_GROUP(".simulator\\.rdo_db" FILES
		${SIMULATOR_DB_FILES}
	)

	SOURCE_GROUP(".simulator\\.rdo_kb\\.logic\\1. base" FILES
		${SIMULATOR_KB_LOGIC_BASE_FILES}
	)

	SOURCE_GROUP(".simulator\\.rdo_kb\\.logic\\2. dpt_some" FILES
		${SIMULATOR_KB_LOGIC_DPTSOME_FILES}
	)

	SOURCE_GROUP(".simulator\\.rdo_kb\\.logic\\3. dpt_prior" FILES
		${SIMULATOR_KB_LOGIC_DPTPRIOR_FILES}
	)

	SOURCE_GROUP(".simulator\\.rdo_kb\\.logic\\4. dpt_search" FILES
		${SIMULATOR_KB_LOGIC_DPTSEARCH_FILES}
	)

	SOURCE_GROUP(".simulator\\.rdo_kb\\.logic\\5. process" FILES
		${SIMULATOR_KB_LOGIC_PROCESS_FILES}
	)

	SOURCE_GROUP(".simulator\\.rdo_kb\\.operations\\1. base" FILES
		${SIMULATOR_KB_OPERATIONS_BASE_FILES}
	)

	SOURCE_GROUP(".simulator\\.rdo_kb\\.operations\\2. event" FILES
		${SIMULATOR_KB_OPERATIONS_EVENT_FILES}
	)

	SOURCE_GROUP(".simulator\\.rdo_kb\\.operations\\3. rule" FILES
		${SIMULATOR_KB_OPERATIONS_RULE_FILES}
	)

	SOURCE_GROUP(".simulator\\.rdo_kb\\.operations\\4. operation" FILES
		${SIMULATOR_KB_OPERATIONS_OPERATION_FILES}
	)

	SOURCE_GROUP(".simulator\\.rdo_kb\\.operations\\5. keyboard" FILES
		${SIMULATOR_KB_OPERATIONS_KEYBOARD_FILES}
	)

	SOURCE_GROUP(".simulator\\.rdo_kb\\.operations\\6. dptsearch_activity" FILES
		${SIMULATOR_KB_OPERATIONS_DPTSEARCH_ACTIVITY_FILES}
	)

	SOURCE_GROUP(".trace" FILES
		${TRACE_FILES}
	)

	SOURCE_GROUP(".type" FILES
		${TYPE_FILES}
	)

	SOURCE_GROUP(".type\\.array" FILES
		${TYPE_ARRAY_FILES}
	)

	SOURCE_GROUP(".type\\.enum" FILES
		${TYPE_ENUM_FILES}
	)

	SOURCE_GROUP(".type\\.fuzzy" FILES
		${TYPE_FUZZY_FILES}
	)

	SOURCE_GROUP(".type\\.matrix" FILES
		${TYPE_MATRIX_FILES}
	)

	SOURCE_GROUP(".value" FILES
		${VALUE_FILES}
	)

ENDIF(MSVC)
