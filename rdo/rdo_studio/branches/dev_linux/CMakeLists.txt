CMAKE_MINIMUM_REQUIRED (VERSION 2.4.6)

PROJECT (rdo_nix_studio)

MESSAGE (STATUS "Cmake version : " ${CMAKE_VERSION})
MESSAGE (STATUS "CMake generates " ${CMAKE_GENERATOR})

MESSAGE (STATUS "System: " ${CMAKE_SYSTEM})
MESSAGE (STATUS "Processor: " ${CMAKE_HOST_SYSTEM_PROCESSOR})

IF (${CMAKE_SYSTEM_NAME} STREQUAL Windows)
IF (MSVC)
MESSAGE (STATUS "Compiler: MSVC")
ENDIF (MSVC)
IF (BORLAND) 
    MESSAGE (STATUS "Compiler: BCC")
ENDIF (BORLAND) 
ELSE (${CMAKE_SYSTEM_NAME} STREQUAL Linux) 
    MESSAGE (STATUS "Only GCC is supported on Linux")
ENDIF (${CMAKE_SYSTEM_NAME} STREQUAL Windows)

MESSAGE (STATUS "Project Source Dir: " ${PROJECT_SOURCE_DIR})

# set the default path for built executables to the "bin" directory
IF (NOT DEFINED(EXECUTABLE_OUTPUT_PATH))
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
ENDIF (NOT DEFINED(EXECUTABLE_OUTPUT_PATH))
# set the default path for built libraries to the "lib" directory
IF (NOT DEFINED(LIBRARY_OUTPUT_PATH))
    set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
ENDIF (NOT DEFINED(LIBRARY_OUTPUT_PATH))

SET (CMAKE_BUILD_TYPE Debug)
MESSAGE (STATUS "Build Type: " ${CMAKE_BUILD_TYPE})

# Find Boost
SET (Boost_USE_STATIC_LIBS   ON)
SET (Boost_USE_MULTITHREADED ON)
FIND_PACKAGE (Boost COMPONENTS system filesystem thread REQUIRED)

# Find Boost
LINK_DIRECTORIES (${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES (${Boost_INCLUDE_DIRS})

# add the include folder to the include path
INCLUDE_DIRECTORIES (${PROJECT_SOURCE_DIR})

# Compile the rdo_common
ADD_SUBDIRECTORY (rdo_common)

# Compile the rdo_kernel
ADD_SUBDIRECTORY (rdo_kernel)

# Compile the rdo_repository
ADD_SUBDIRECTORY (rdo_repository)






















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