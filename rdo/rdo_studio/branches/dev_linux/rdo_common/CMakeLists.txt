CMAKE_MINIMUM_REQUIRED (VERSION 2.4.6)

PROJECT (rdo_nix_common)

# Find Boost
SET (Boost_USE_STATIC_LIBS   ON)
SET (Boost_USE_MULTITHREADED ON)
FIND_PACKAGE (Boost COMPONENTS system filesystem thread REQUIRED)

# Find Boost
LINK_DIRECTORIES (${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES (${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES (../)

SET (HEADER_FILES	
	rdocommon.h
	model_objects.h
	namespace.h
	rdoanimation.h
	rdoanimation.inl
	rdocommon.inl
	rdodebug.h
	rdofile.h
	rdofile.inl
	rdoidgenerator.h
	rdoindexedstack.h
	rdointerface.h
	rdomacros.h
	rdosingletone.h
	rdosmart_ptr_wrapper.h
	rdostream.h
	rdotime.h
	rdotime.inl
	rdotypes.h
	stack.h
	stack.inl
	static_assert.h
	supersubclass.h
	smart_ptr/counter_reference.h
	smart_ptr/counter_reference.inl
	smart_ptr/interface_ptr.h
	smart_ptr/interface_ptr.inl
	smart_ptr/intrusive_ptr.h
	smart_ptr/intrusive_ptr.inl)

SET (SOURCE_FILES
	rdocommon.cpp
	rdofile.cpp)

ADD_LIBRARY (rdo_nix_common ${HEADER_FILES} ${SOURCE_FILES})
TARGET_LINK_LIBRARIES (rdo_nix_common ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})