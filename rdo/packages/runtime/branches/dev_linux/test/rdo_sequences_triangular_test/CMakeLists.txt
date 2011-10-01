MESSAGE(STATUS "CREATE  TEST  FOR  RDO  RUNTIME  SEQUENCES  TRIANGULAR")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${RDO_BASE_DIRECTORY})

SET(FILES
	data.txt)

SET(SOURCE_FILES
	main.cpp)

SOURCE_GROUP(".test" FILES 
	${SOURCE_FILES}
	${FILES})

FIND_PACKAGE(Boost COMPONENTS unit_test_framework REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

ADD_EXECUTABLE(test_rdo_runtime_sequences_triangular ${SOURCE_FILES} ${FILES})
ADD_TEST(NAME test_rdo_runtime_sequences_triangular COMMAND ${EXECUTABLE_OUTPUT_PATH}/test_rdo_runtime_sequences_triangular)
ADD_DEPENDENCIES(test_rdo_runtime_sequences_triangular rdo_utils)

IF(MSVC)
	SET_TARGET_PROPERTIES(test_rdo_runtime_sequences_triangular PROPERTIES COMPILE_FLAGS "/EHa")
ENDIF()

TARGET_LINK_LIBRARIES(test_rdo_runtime_sequences_triangular rdo_runtime)
TARGET_LINK_LIBRARIES(test_rdo_runtime_sequences_triangular ${Boost_LIBRARIES})
