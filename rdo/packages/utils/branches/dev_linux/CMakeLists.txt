MESSAGE(STATUS "CREATE  RDO_UTILS  LIBRARY  PROJECT")

# Find Boost
IF(MSVC)
    SET(Boost_USE_STATIC_LIBS   ON)
    SET(Boost_USE_MULTITHREADED ON)
ENDIF(MSVC)
FIND_PACKAGE(Boost COMPONENTS system filesystem thread date_time REQUIRED)

# Find Boost
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
	supersubclass.h
	smart_ptr/counter_reference.h
	smart_ptr/factory.h
	smart_ptr/interface_ptr.h
	smart_ptr/intrusive_ptr.h
	smart_ptr/intrusive_ptr_interface_wrapper.h
	smart_ptr/ref_counter.h
	smart_ptr/ref_counter_i.h)

SET(INLITE_FILES
	rdoanimation.inl
	rdocommon.inl
	rdofile.inl
	rdointerface.inl
	rdotime.inl
	stack.inl
	smart_ptr/counter_reference.inl
	smart_ptr/factory.inl
	smart_ptr/interface_ptr.inl
	smart_ptr/intrusive_ptr.inl
	smart_ptr/intrusive_ptr_interface_wrapper.inl
	smart_ptr/ref_counter.inl)

SET(SOURCE_FILES
	rdocommon.cpp
	rdofile.cpp)

ADD_LIBRARY(rdo_utils ${HEADER_FILES} ${INLITE_FILES} ${SOURCE_FILES})
SET_TARGET_PROPERTIES(rdo_utils PROPERTIES LINKER_LANGUAGE CXX)
TARGET_LINK_LIBRARIES(rdo_utils ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})

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

SOURCE_GROUP(".smart_prt" FILES
	rdosmart_ptr_wrapper.h
	smart_ptr/counter_reference.h
	smart_ptr/interface_ptr.h
	smart_ptr/intrusive_ptr.h
	smart_ptr/counter_reference.inl
	smart_ptr/interface_ptr.inl
	smart_ptr/intrusive_ptr.inl)

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

ENDIF(MSVC)
