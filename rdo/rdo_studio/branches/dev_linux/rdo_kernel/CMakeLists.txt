CMAKE_MINIMUM_REQUIRED (VERSION 2.4.6)

PROJECT (rdo_nix_kernel)

# Find Boost
SET (Boost_USE_STATIC_LIBS   ON)
SET (Boost_USE_MULTITHREADED ON)
FIND_PACKAGE (Boost COMPONENTS system filesystem thread REQUIRED)

# Find Boost
LINK_DIRECTORIES (${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES (${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES (../)

SET (HEADER_FILES	
	rdothread.h
	rdokernel.h)

SET (SOURCE_FILES
	rdothread.cpp
	rdokernel.cpp)

ADD_LIBRARY (rdo_nix_kernel ${HEADER_FILES} ${SOURCE_FILES})
TARGET_LINK_LIBRARIES (rdo_nix_kernel ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})