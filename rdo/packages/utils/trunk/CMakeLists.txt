#==============================================================================
# Copyright (c) 2011-2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO_UTILS  LIBRARY ")

FIND_PACKAGE(Boost COMPONENTS system filesystem thread date_time locale REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/smart_ptr)

SET(ANIMATION_FILES
	rdoanimation.h
	rdoanimation.inl
)

SET(COMMON_FILES
	model_objects.cpp
	model_objects.h
	namespace.h
	platform.h
	rdocommon.cpp
	rdocommon.h
	rdocommon.inl
	rdomacros.h 
	rdotypes.h
	warning_disable.h
	warning_enable.h
)

SET(DEBUG_FILES
	rdodebug.h
	static_assert.h
)

SET(FILE_FILES
	rdofile.cpp
	rdofile.h
)

SET(INTERFACE_FILES
	rdointerface.h
	rdointerface.inl
)

SET(LOCALE_FILES
	rdolocale.cpp
	rdolocale.h
)

SET(STREAM_FILES
	rdostream.h
)

SET(STUFF_FILES
	explicit_value.h
	rdoidgenerator.h
	rdoindexedstack.h
	rdosingletone.h
	stack.h
	stack.inl
	supersubclass.h
)

SET(TIME_FILES
	rdotime.h
	rdotime.inl
)

SET(SMARTPTR_FACTORY_FILES
	smart_ptr/factory.h
	smart_ptr/factory.inl
)

SET(SMARTPTR_INTERFACEPTR_FILES
	smart_ptr/interface_ptr.h
	smart_ptr/interface_ptr.inl
)

SET(SMARTPTR_INTRUSIVEPTR_FILES
	smart_ptr/intrusive_ptr.h
	smart_ptr/intrusive_ptr.inl
	smart_ptr/intrusive_ptr_interface_wrapper.h
	smart_ptr/intrusive_ptr_interface_wrapper.inl
)

SET(SMARTPTR_REFCOUNTER_FILES
	smart_ptr/counter_reference.h
	smart_ptr/counter_reference.inl
	smart_ptr/ref_counter.h
	smart_ptr/ref_counter.inl
	smart_ptr/ref_counter_i.h
)

SET(SMARTPTR_WRAPPER_FILES
	rdosmart_ptr_wrapper.h
)

SET(SMART_PTR_FILES
	${SMARTPTR_FACTORY_FILES}
	${SMARTPTR_INTERFACEPTR_FILES}
	${SMARTPTR_INTRUSIVEPTR_FILES}
	${SMARTPTR_REFCOUNTER_FILES}
	${SMARTPTR_WRAPPER_FILES}
)

SET(SOURCE_FILES
	${ANIMATION_FILES}
	${COMMON_FILES}
	${DEBUG_FILES}
	${FILE_FILES}
	${INTERFACE_FILES}
	${LOCALE_FILES}
	${STREAM_FILES}
	${STUFF_FILES}
	${TIME_FILES}
	${SMART_PTR_FILES}
)

ADD_LIBRARY(rdo_utils ${SOURCE_FILES})
SET_TARGET_PROPERTIES(rdo_utils PROPERTIES LINKER_LANGUAGE CXX)

INCLUDE(${PROJECT_SOURCE_DIR}/utils/projects/common/utils_common.cmake)
SET_TARGET_PROPERTIES(rdo_utils PROPERTIES FOLDER ${PROJECT_FOLDER})

TARGET_LINK_LIBRARIES(rdo_utils ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_DATE_TIME_LIBRARY} ${Boost_LOCALE_LIBRARY})

INSTALL(TARGETS rdo_utils DESTINATION lib)

SET(RDO_UTILS_TEST_PATH ${CMAKE_CURRENT_SOURCE_DIR}/test)
ADD_SUBDIRECTORY(${RDO_UTILS_TEST_PATH})

IF(MSVC) # options for windows #

	SOURCE_GROUP(".animation" FILES
		${ANIMATION_FILES}
	)

	SOURCE_GROUP(".common" FILES
		${COMMON_FILES}
	)

	SOURCE_GROUP(".debug" FILES
		${DEBUG_FILES}
	)

	SOURCE_GROUP(".file" FILES
		${FILE_FILES}
	)

	SOURCE_GROUP(".interface" FILES
		${INTERFACE_FILES}
	)

	SOURCE_GROUP(".locale" FILES
		${LOCALE_FILES}
	)

	SOURCE_GROUP(".stream" FILES
		${STREAM_FILES}
	)

	SOURCE_GROUP(".stuff" FILES
		${STUFF_FILES}
	)

	SOURCE_GROUP(".time" FILES
		${TIME_FILES}
	)

	SOURCE_GROUP(".smart_ptr\\.factory" FILES
		${SMARTPTR_FACTORY_FILES}
	)

	SOURCE_GROUP(".smart_ptr\\.interface_ptr" FILES
		${SMARTPTR_INTERFACEPTR_FILES}
	)

	SOURCE_GROUP(".smart_ptr\\.intrusive_ptr" FILES
		${SMARTPTR_INTRUSIVEPTR_FILES}
	)

	SOURCE_GROUP(".smart_ptr\\.ref_counter" FILES
		${SMARTPTR_REFCOUNTER_FILES}
	)

	SOURCE_GROUP(".smart_ptr\\.wrapper" FILES
		${SMARTPTR_WRAPPER_FILES}
	)

ENDIF(MSVC)
