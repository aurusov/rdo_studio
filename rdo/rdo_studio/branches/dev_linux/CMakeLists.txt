cmake_minimum_required (VERSION 2.4.6)

project (rdo_nix_studio)

message (STATUS "Cmake version : " ${CMAKE_VERSION})
message (STATUS "CMake generates " ${CMAKE_GENERATOR})

message (STATUS "System: " ${CMAKE_SYSTEM})
message (STATUS "Processor: " ${CMAKE_HOST_SYSTEM_PROCESSOR})

if (${CMAKE_SYSTEM_NAME} STREQUAL Windows)
if (MSVC)
message (STATUS "Compiler: MSVC")
endif (MSVC)
if (BORLAND) 
    MESSAGE (STATUS "Compiler: BCC")
endif (BORLAND) 
else (${CMAKE_SYSTEM_NAME} STREQUAL Linux) 
    MESSAGE (STATUS "Only GCC is supported on Linux")
endif (${CMAKE_SYSTEM_NAME} STREQUAL Windows)

message (STATUS "Project Source Dir: " ${PROJECT_SOURCE_DIR})

set (CMAKE_BUILD_TYPE Debug)
message (STATUS "Build Type: " ${CMAKE_BUILD_TYPE})

# set warning level to maximum Compiler
string (REGEX REPLACE /W[0-4] /W4 CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

# add the include folder to the include path
include_directories (${PROJECT_SOURCE_DIR})

# Compile the rdo_common
add_subdirectory (rdo_common)

# Compile the rdo_kernel
add_subdirectory (rdo_kernel)

# Compile the rdo_repository
add_subdirectory (rdo_repository)

# Compile rdo_lib
add_subdirectory (rdo_lib)






















# Compile the rdo_converter

# Compile the rdo_corba
#ADD_LIBRARY(rdo_nix_corba
#	rdo_lib/rdo_corba/RDOCorba.hh
#	rdo_lib/rdo_corba/RDOCorbaSK.cc
#TARGET_LINK_LIBRARIES(rdo_nix_corba ${Boost_SYSTEM_LIBRARY})

# Compile the rdo_mbuilder
#ADD_LIBRARY(rdo_nix_mbuilder
#	rdo_lib/rdo_mbuilder/namespace.h
#	rdo_lib/rdo_mbuilder/pch.h
#	rdo_lib/rdo_mbuilder/pch.cpp
#	rdo_lib/rdo_mbuilder/process.h
#	rdo_lib/rdo_mbuilder/process.cpp
#	rdo_lib/rdo_mbuilder/rdo_resources.h
#	rdo_lib/rdo_mbuilder/rdo_resources.cpp
#TARGET_LINK_LIBRARIES(rdo_nix_mbuilder)

# Compile the rdo_parser

# Compile the rdo_runtime

# Compile the rdo_simulator
#ADD_LIBRARY(rdo_nix_simulator
#	rdo_lib/rdo_simulator/procgui.h
#	rdo_lib/rdo_simulator/procgui.cpp
#	rdo_lib/rdo_simulator/rdosimwin.h
#	rdo_lib/rdo_simulator/rdosimwin.cpp)
#TARGET_LINK_LIBRARIES(rdo_nix_simulator ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})