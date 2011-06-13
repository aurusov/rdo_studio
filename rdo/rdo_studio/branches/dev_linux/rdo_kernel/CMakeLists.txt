cmake_minimum_required (VERSION 2.4.6)

project (rdo_nix_kernel)

# Find Boost
set (Boost_USE_STATIC_LIBS   ON)
set (Boost_USE_MULTITHREADED ON)
find_package (Boost COMPONENTS system filesystem thread REQUIRED)

# Find Boost
link_directories (${Boost_LIBRARY_DIRS})
include_directories (${Boost_INCLUDE_DIRS})

include_directories (../)

set (HEADER_FILES	
	rdothread.h
	rdokernel.h)

set (SOURCE_FILES
	rdothread.cpp
	rdokernel.cpp)

add_library (rdo_nix_kernel ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries (rdo_nix_kernel ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})