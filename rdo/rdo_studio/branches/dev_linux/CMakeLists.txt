cmake_minimum_required(VERSION 2.4.6)

project(rdo_studio_linux)

message(STATUS "System: " ${CMAKE_SYSTEM_NAME} " " ${CMAKE_SYSTEM_VERSION})
message(STATUS "Processor: " ${CMAKE_HOST_SYSTEM_PROCESSOR})
#message(STATUS "DIR: " ${PROJECT_SOURCE_DIR} " - " ${CMAKE_SOURCE_DIR})
message(STATUS "Boost_LIBRARY_DIRS: " ${Boost_LIBRARY_DIRS})
message(STATUS "Boost_INCLUDE_DIRS: " ${Boost_INCLUDE_DIRS})

if(${CMAKE_SYSTEM_NAME} STREQUAL Windows)
if(MSVC)
message(STATUS "Compiler: MSVC, version: " ${MSVC_VERSION})
endif(MSVC)

if(BORLAND) 
    message(STATUS "Compiler: BCC")
endif(BORLAND) 

else(${CMAKE_SYSTEM_NAME} STREQUAL Linux) 
    message(STATUS "Only GCC is supported on Linux")
endif(${CMAKE_SYSTEM_NAME} STREQUAL Windows)

message(STATUS "CMake generates " ${CMAKE_GENERATOR})

# Find Boost
set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost COMPONENTS system filesystem thread REQUIRED)

# Find Boost
link_directories(${Boost_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

# add the include folder to the include path
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/rdo_common)
include_directories(${PROJECT_SOURCE_DIR}/rdo_common/smart_ptr)

# Compile the Serial Library
add_library(rdo_common/rdo_linux_common
	rdo_common/rdocommon.cpp
	rdo_common/rdocommon.h
	rdo_common/model_objects.h
	rdo_common/namespace.h
	rdo_common/rdoanimation.h
	rdo_common/rdoanimation.inl
	rdo_common/rdocommon.inl
	rdo_common/rdodebug.h
	rdo_common/rdofile.cpp
	rdo_common/rdofile.h
	rdo_common/rdofile.inl
	rdo_common/rdoidgenerator.h
	rdo_common/rdoindexedstack.h
	rdo_common/rdointerface.h
	rdo_common/rdomacros.h
	rdo_common/rdosingletone.h
	rdo_common/rdosmart_ptr_wrapper.h
	rdo_common/rdostream.h
	rdo_common/rdotime.h
	rdo_common/rdotime.inl
	rdo_common/rdotypes.h
	rdo_common/stack.h
	rdo_common/stack.inl
	rdo_common/static_assert.h
	rdo_common/supersubclass.h
	rdo_common/smart_ptr/counter_reference.h
	rdo_common/smart_ptr/counter_reference.inl
	rdo_common/smart_ptr/interface_ptr.h
	rdo_common/smart_ptr/interface_ptr.inl
	rdo_common/smart_ptr/intrusive_ptr.h
	rdo_common/smart_ptr/intrusive_ptr.inl)
target_link_libraries(rdo_common/rdo_linux_common ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})


