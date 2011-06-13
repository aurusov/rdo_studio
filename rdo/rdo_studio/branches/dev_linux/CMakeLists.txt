cmake_minimum_required (VERSION 2.4.6)

project (rdo_nix_studio)

message (status "Cmake version : " ${CMAKE_VERSION})
message (status "CMake generates " ${CMAKE_GENERATOR})

message (status "System: " ${CMAKE_SYSTEM})
message (status "Processor: " ${CMAKE_HOST_SYSTEM_PROCESSOR})

if (${CMAKE_SYSTEM_NAME} STREQUAL Windows)
if (MSVC)
message (status "Compiler: MSVC")
endif (MSVC)
if (BORLAND) 
    MESSAGE (status "Compiler: BCC")
endif (BORLAND) 
else (${CMAKE_SYSTEM_NAME} STREQUAL Linux) 
    MESSAGE (STATUS "Only GCC is supported on Linux")
endif (${CMAKE_SYSTEM_NAME} STREQUAL Windows)

message (status "Project Source Dir: " ${PROJECT_SOURCE_DIR})

# set the default path for built executables to the "bin" directory
if (NOT DEFINED(EXECUTABLE_OUTPUT_PATH))
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
endif (NOT DEFINED(EXECUTABLE_OUTPUT_PATH))
# set the default path for built libraries to the "lib" directory
if (NOT DEFINED(LIBRARY_OUTPUT_PATH))
    set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
endif (NOT DEFINED(LIBRARY_OUTPUT_PATH))

set (CMAKE_BUILD_TYPE Debug)
message (STATUS "Build Type: " ${CMAKE_BUILD_TYPE})

# Find Boost
set (Boost_USE_STATIC_LIBS   ON)
set (Boost_USE_MULTITHREADED ON)
find_package (Boost COMPONENTS system filesystem thread REQUIRED)

# Find Boost
link_directories (${Boost_LIBRARY_DIRS})
include_directories (${Boost_INCLUDE_DIRS})

# add the include folder to the include path
include_directories (${PROJECT_SOURCE_DIR})

# Compile the rdo_common
add_subdirectory (rdo_common)

# Compile the rdo_kernel
add_subdirectory (rdo_kernel)

# Compile the rdo_repository
add_subdirectory (rdo_repository)






















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