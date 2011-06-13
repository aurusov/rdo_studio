cmake_minimum_required (VERSION 2.4.6)

project (rdo_nix_common)

# Find Boost
set (Boost_USE_STATIC_LIBS   ON)
set (Boost_USE_MULTITHREADED ON)
find_package (Boost COMPONENTS system filesystem thread REQUIRED)

# Find Boost
link_directories (${Boost_LIBRARY_DIRS})
include_directories (${Boost_INCLUDE_DIRS})

include_directories (../)

set (HEADER_FILES	
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

set (SOURCE_FILES
	rdocommon.cpp
	rdofile.cpp)

add_library (rdo_nix_common ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries (rdo_nix_common ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})