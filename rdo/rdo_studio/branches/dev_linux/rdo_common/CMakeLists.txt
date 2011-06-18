cmake_minimum_required (VERSION 2.6.0)

if (NOT TARGET rdo_nix_common)

message (STATUS "CREATE RDO_COMMON LIBRARY PROJECT")

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
	smart_ptr/interface_ptr.h
	smart_ptr/intrusive_ptr.h)

set (INLITE_FILES
	stack.inl
	rdotime.inl
	rdofile.inl
	rdocommon.inl
	rdoanimation.inl
	smart_ptr/counter_reference.inl
	smart_ptr/interface_ptr.inl
	smart_ptr/intrusive_ptr.inl)

set (SOURCE_FILES
	rdocommon.cpp
	rdofile.cpp)

source_group ("animation" FILES 
	rdoanimation.h
	rdoanimation.inl)

source_group ("common" FILES
	rdotypes.h
	namespace.h
	rdomacros.h 
	rdocommon.h
	rdocommon.inl
	rdocommon.cpp
	model_objects.h)

source_group ("debug" FILES 
	rdodebug.h
	static_assert.h)

source_group ("file" FILES 
	rdofile.h
	rdofile.inl
	rdofile.cpp)

source_group ("interface" FILES 
	rdointerface.h)

source_group ("smart_prt" FILES
	rdosmart_ptr_wrapper.h
	smart_ptr/counter_reference.h
	smart_ptr/interface_ptr.h
	smart_ptr/intrusive_ptr.h
	smart_ptr/counter_reference.inl
	smart_ptr/interface_ptr.inl
	smart_ptr/intrusive_ptr.inl)

source_group ("stream" FILES 
	rdostream.h)

source_group ("stuff" FILES 
	stack.h
	stack.inl
	supersubclass.h
	rdosingletone.h
	rdoidgenerator.h
	rdoindexedstack.h)

source_group ("time" FILES
	rdotime.h
	rdotime.inl)

add_library (rdo_nix_common ${HEADER_FILES} ${INLITE_FILES} ${SOURCE_FILES})
target_link_libraries (rdo_nix_common ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})

endif (NOT TARGET rdo_nix_common)