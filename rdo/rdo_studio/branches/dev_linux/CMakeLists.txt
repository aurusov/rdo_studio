cmake_minimum_required (VERSION 2.6.0)

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
if (MSVC)
string (REGEX REPLACE /W[0-4] /W4 CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
endif (MSVC)

# add the include folder to the include path
include_directories (${PROJECT_SOURCE_DIR})

# Compile the rdo_common
add_subdirectory (rdo_common rdo_common)

# Compile the rdo_kernel
add_subdirectory (rdo_kernel rdo_kernel)

# Compile the rdo_repository
add_subdirectory (rdo_repository rdo_repository)

# Compile rdo_lib
add_subdirectory (rdo_lib rdo_lib)
