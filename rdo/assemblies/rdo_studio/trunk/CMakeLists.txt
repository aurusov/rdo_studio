#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# Welcome to the CMake build system for RDO STUDIO (UNIX VERSION).
# This is the main file where we prepare the general build environment
# and provide build configuration options.
#==============================================================================

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)
PROJECT(rdo)

#==============================================================================

SET(PROJECT_NAME         "rdo")
SET(PROJECT_DESCRIPTION  "CMake build system for ${PROJECT_NAME} projects")
SET(PROJECT_VENDOR       "BMSTU")
SET(PROJECT_VENDOR_LONG  "BMSTU")
SET(PROJECT_COPYRIGHT    "Copyright ${PROJECT_VENDOR_LONG}")
SET(PROJECT_VERSION      "{TEMP}")
SET(PROJECT_VERSION_LONG "{TEMP}")
SET(ORG_WEBSITE          "http://rdo.rk9.bmstu.ru/")

#==============================================================================

MESSAGE(STATUS "Create RDO")
STRING(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)

ADD_DEFINITIONS(-DRDO_ST)
ADD_DEFINITIONS(-DRDO_LIMIT_RES)
ADD_DEFINITIONS(-DRDO_LICENSE_ACADEMIC)
ADD_DEFINITIONS(-DNOMINMAX)
ADD_DEFINITIONS(-DBOOST_SIGNALS_NO_DEPRECATION_WARNING)

# work with cmake
SET(CMAKE_PREFIX_PATH $ENV{QTDIR}/lib/cmake)
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# call system info script
INCLUDE(FindOS)
INCLUDE(add.boost.test)

IF(NOT MSVC)
    SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Wall")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
ELSE()
    STRING(REGEX REPLACE /W[0-4] /W4 CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
ENDIF(NOT MSVC)

IF(NOT MSVC)
    SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fPIC")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++0x")
ENDIF(NOT MSVC)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug)
ENDIF(NOT CMAKE_BUILD_TYPE)

IF(NOT MSVC)
    IF(NOT BUILD_SHARED_LIBS)
        SET(BUILD_SHARED_LIBS ON)
    ENDIF(NOT BUILD_SHARED_LIBS)
#   SET(CDEFS "-m32 -o output32")
ENDIF(NOT MSVC)

SET(ENABLE_UNICODE FALSE)
IF(NOT MSVC AND ENABLE_UNICODE)
    ADD_DEFINITIONS(-DUNICODE)
ENDIF(NOT MSVC AND ENABLE_UNICODE)

SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

# add the include folder to the include path
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

INCLUDE(CTest)
ENABLE_TESTING()

IF(MSVC)
    SET(Boost_USE_STATIC_LIBS    ON)
    SET(Boost_USE_MULTITHREADED  ON)
ENDIF(MSVC)

# enable folders for msvc
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

SET(APPS_FOLDERS       ".apps")
SET(TESTS_FOLDERS      ".tests")
SET(LIBRARY_FOLDERS    ".modules")
SET(THIRDPARTY_FOLDERS ".thirdparty")

IF(WIN32)
    IF(NOT BOOST_ROOT)
        SET(BOOST_ROOT $ENV{BOOST_ROOT})
    ENDIF()
ENDIF(WIN32)

# set base directory
SET(RDO_BASE_DIRECTORY ${PROJECT_SOURCE_DIR})

IF(WIN32)
    SET(PATH_STRING $ENV{PATH})
    STRING(FIND "${PATH_STRING}" "thirdparty/bison_flex/bin" OUT)
    IF(${OUT} GREATER -1)
    ELSE()
        SET(ENV{PATH} "$ENV{PATH};${RDO_BASE_DIRECTORY}/thirdparty/bison_flex/bin")
    ENDIF()
ENDIF()

FILE(GLOB DOC_FILES "${RDO_BASE_DIRECTORY}/doc/*.*")

#==============================================================================

FIND_PACKAGE(Subversion)

IF(NOT SUBVERSION_FOUND)
    MESSAGE(FATAL_ERROR "SUBVERSION NOT FOUND")
ENDIF()

SET(SVN_RES TRUE)

SET(FILE_VERSION "${CMAKE_CURRENT_BINARY_DIR}/svn.version.txt")
EXECUTE_PROCESS(COMMAND svnversion -n ${RDO_BASE_DIRECTORY} RESULT_VARIABLE ${SVN_RES} OUTPUT_FILE ${FILE_VERSION})

FILE(READ ${FILE_VERSION} READ_DATA)
STRING(REPLACE "\n" "" CMAKE_SVN_VERSION ${READ_DATA})

MESSAGE(STATUS "SVN VERSION: ${CMAKE_SVN_VERSION}")

SET(PROJECT_VERSION      ${CMAKE_SVN_VERSION})
SET(PROJECT_VERSION_LONG ${CMAKE_SVN_VERSION})

#==============================================================================

SET(SCRIPTS_DIRECTORY ${RDO_BASE_DIRECTORY}/scripts)

SET(SCRIPT_FILES
    ${SCRIPTS_DIRECTORY}/python/executor_of_system_tests.py
    ${SCRIPTS_DIRECTORY}/statistics/statistics.sh
)

SET(BISON_FLEX_DIRECTORY ${RDO_BASE_DIRECTORY}/thirdparty/bison_flex)

IF(WIN32)
    SET(ENV{BISON_PKGDATADIR} ${BISON_FLEX_DIRECTORY}/share/bison)
ENDIF()

# Compile the rdo_common
SET(UTILS_PATH ${RDO_BASE_DIRECTORY}/utils)
ADD_SUBDIRECTORY(${UTILS_PATH})

# Compile the rdo_kernel
SET(KERNEL_PATH ${RDO_BASE_DIRECTORY}/kernel)
ADD_SUBDIRECTORY(${KERNEL_PATH})

# Compile XML plugin
SET(XML_PUGIXML_PATH ${RDO_BASE_DIRECTORY}/thirdparty/pugixml/scripts)
ADD_SUBDIRECTORY(${XML_PUGIXML_PATH})

# Compile scintilla editor
SET(SCINTILLA_PATH ${RDO_BASE_DIRECTORY}/thirdparty/scintilla)
ADD_SUBDIRECTORY(${SCINTILLA_PATH})

# Compile the rdo_repository
SET(REPOSITORY_PATH ${RDO_BASE_DIRECTORY}/repository)
ADD_SUBDIRECTORY(${REPOSITORY_PATH})

# Compile rdo_converter
SET(CONVERTER_PATH ${RDO_BASE_DIRECTORY}/converter)
ADD_SUBDIRECTORY(${CONVERTER_PATH})

# Compile rdo_lib
SET(SIMULATOR_PATH ${RDO_BASE_DIRECTORY}/simulator)
ADD_SUBDIRECTORY(${SIMULATOR_PATH})

SET(ABSTRACT_UI_PATH ${RDO_BASE_DIRECTORY}/ui/abstract)
ADD_SUBDIRECTORY(${ABSTRACT_UI_PATH})

SET(UI_QT_PATH ${RDO_BASE_DIRECTORY}/ui/qt)
ADD_SUBDIRECTORY(${UI_QT_PATH})

SET(APLICATIONS_DIRECTORY ${RDO_BASE_DIRECTORY}/app)
ADD_SUBDIRECTORY(${APLICATIONS_DIRECTORY})

#==============================================================================
#---------------------------------- info --------------------------------------
#==============================================================================

# build info
SET(BUILD_INFO_BAR "====================================================================================")
SET(NOOP_STRING "")

MESSAGE(STATUS ${NOOP_STRING})
MESSAGE(STATUS ${BUILD_INFO_BAR})
MESSAGE(STATUS "Summary of the build:")
MESSAGE(STATUS ${BUILD_INFO_BAR})
MESSAGE(STATUS "ENABLE UNICODE:               ${ENABLE_UNICODE}")
MESSAGE(STATUS ${BUILD_INFO_BAR})
MESSAGE(STATUS "BUILD rdo console:            TRUE")
MESSAGE(STATUS ${BUILD_INFO_BAR})
MESSAGE(STATUS "Build type : ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS ${BUILD_INFO_BAR})
MESSAGE(STATUS ${NOOP_STRING})

#==============================================================================
#------------------------------install unix------------------------------------
#==============================================================================

IF(PROJECT_OS_LINUX)
ENDIF()

IF(MSVC)
    SET(UNINSTALL_COMMAND "UNINSTALL")
ELSE()
    SET(UNINSTALL_COMMAND "uninstall")
ENDIF()

CONFIGURE_FILE(
    "${PROJECT_SOURCE_DIR}/cmake/cmake.uninstall.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake.uninstall.cmake"
    IMMEDIATE @ONLY
    )

ADD_CUSTOM_TARGET(${UNINSTALL_COMMAND}
    COMMAND "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake.uninstall.cmake"
    )

#==============================================================================

INCLUDE(InstallRequiredSystemLibraries)

SET(CPACK_PACKAGING_INSTALL_PREFIX    "/")
SET(CPACK_PACKAGE_NAME                "${PROJECT_NAME_LOWER}")
SET(CPACK_PACKAGE_VENDOR              "${PROJECT_VENDOR_LONG}")

SET(CPACK_PACKAGE_DESCRIPTION         "rdo application")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${CPACK_PACKAGE_DESCRIPTION}")
SET(CPACK_PACKAGE_CONTACT             "Evgeny Proydakov <lord.tiran@gmail.com>")

SET(CPACK_DEBIAN_PACKAGE_VERSION      ${PROJECT_VERSION})
SET(CPACK_DEBIAN_PACKAGE_PRIORITY     "standard")
SET(CPACK_DEBIAN_PACKAGE_SECTION      "non-free")
SET(CPACK_DEBIAN_ARCHITECTURE         ${CMAKE_SYSTEM_PROCESSOR})
SET(CPACK_DEBIAN_PACKAGE_DEPENDS      "libc6 (> = 2.1.3)")
SET(CPACK_DEBIAN_PACKAGE_HOMEPAGE     "${ORG_WEBSITE}")

SET(CPACK_RPM_PACKAGE_VERSION         ${PROJECT_VERSION})
SET(CPACK_RPM_PACKAGE_GROUP           "Applications/Base")
SET(CPACK_RPM_PACKAGE_LICENSE         "Proprietary")
SET(CPACK_RPM_PACKAGE_ARCHITECTURE    ${CMAKE_SYSTEM_PROCESSOR})
SET(CPACK_RPM_PACKAGE_URL             "${ORG_WEBSITE}")
SET(CPACK_RPM_PACKAGE_DESCRIPTION     "${CPACK_PACKAGE_DESCRIPTION}")

SET(CPACK_PACKAGE_FILE_NAME           "${PROJECT_NAME_LOWER}_${PROJECT_VERSION}_${CMAKE_SYSTEM_PROCESSOR}")

INCLUDE(CPack)
INCLUDE(Documentation)
