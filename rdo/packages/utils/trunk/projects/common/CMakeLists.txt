#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "CREATE  RDO_UTILS  LIBRARY ")

FIND_PACKAGE(Boost COMPONENTS system filesystem thread date_time REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/smart_ptr)

SET(HEADER_FILES	
	model_objects.h
	namespace.h
	platform.h
	rdoanimation.h
	rdocommon.h
	rdodebug.h
	rdofile.h
	rdoidgenerator.h
	rdoindexedstack.h
	rdointerface.h
	rdomacros.h
	rdosingletone.h
	rdosmart_ptr_wrapper.h
	rdostream.h
	rdotime.h
	rdotypes.h
	stack.h
	static_assert.h
	supersubclass.h)

SET(INLINE_FILES
	rdoanimation.inl
	rdocommon.inl
	rdofile.inl
	rdointerface.inl
	rdotime.inl
	stack.inl)

SET(SOURCE_FILES
	rdocommon.cpp
	rdofile.cpp)

SET(SMART_PTR_FILES
	smart_ptr/factory.h
	smart_ptr/factory.inl

	smart_ptr/interface_ptr.h
	smart_ptr/interface_ptr.inl

	smart_ptr/intrusive_ptr.h
	smart_ptr/intrusive_ptr_interface_wrapper.h
	smart_ptr/intrusive_ptr.inl
	smart_ptr/intrusive_ptr_interface_wrapper.inl

	smart_ptr/ref_counter_i.h
	smart_ptr/ref_counter.inl
	smart_ptr/ref_counter.h
	smart_ptr/counter_reference.h
	smart_ptr/counter_reference.inl)

ADD_LIBRARY(rdo_utils ${HEADER_FILES} ${INLINE_FILES} ${SOURCE_FILES} ${SMART_PTR_FILES})
SET_TARGET_PROPERTIES(rdo_utils PROPERTIES LINKER_LANGUAGE CXX)
TARGET_LINK_LIBRARIES(rdo_utils ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_DATE_TIME_LIBRARY})

SET(RDO_UTILS_TEST_PATH ${CMAKE_CURRENT_SOURCE_DIR}/test)
ADD_SUBDIRECTORY(${RDO_UTILS_TEST_PATH})

IF(MSVC) # lotions for windows #

SOURCE_GROUP(".animation" FILES 
	rdoanimation.h
	rdoanimation.inl)

SOURCE_GROUP(".common" FILES
	rdotypes.h
	namespace.h
	rdomacros.h 
	rdocommon.h
	rdocommon.inl
	rdocommon.cpp
	model_objects.h)

SOURCE_GROUP(".debug" FILES 
	rdodebug.h
	static_assert.h)

SOURCE_GROUP(".file" FILES 
	rdofile.h
	rdofile.inl
	rdofile.cpp)

SOURCE_GROUP(".interface" FILES 
	rdointerface.h
	rdointerface.inl)

SOURCE_GROUP(".smart_ptr_factory" FILES
	smart_ptr/factory.h
	smart_ptr/factory.inl)

SOURCE_GROUP(".smart_ptr_wrapper" FILES
	rdosmart_ptr_wrapper.h)

SOURCE_GROUP(".smart_ptr_ref_counter" FILES
	smart_ptr/ref_counter_i.h
	smart_ptr/ref_counter.inl
	smart_ptr/ref_counter.h
	smart_ptr/counter_reference.h
	smart_ptr/counter_reference.inl)

SOURCE_GROUP(".smart_ptr_interface_ptr" FILES
	smart_ptr/interface_ptr.h
	smart_ptr/interface_ptr.inl)

SOURCE_GROUP(".smart_ptr_intrusive_ptr" FILES
	smart_ptr/intrusive_ptr.h
	smart_ptr/intrusive_ptr_interface_wrapper.h
	smart_ptr/intrusive_ptr.inl
	smart_ptr/intrusive_ptr_interface_wrapper.inl)

SOURCE_GROUP(".stream" FILES 
	rdostream.h)

SOURCE_GROUP(".stuff" FILES 
	stack.h
	stack.inl
	supersubclass.h
	rdosingletone.h
	rdoidgenerator.h
	rdoindexedstack.h)

SOURCE_GROUP(".time" FILES
	rdotime.h
	rdotime.inl)

SOURCE_GROUP(".platform" FILES
	platform.h)

ENDIF(MSVC)
