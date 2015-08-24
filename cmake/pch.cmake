#==============================================================================
# Copyright (c) 2013 Urusov Andrey <drobus@gmail.com>
#==============================================================================

MACRO(ADD_PCH SOURCE_FILES NO_PCH_FILES PROJECT_PATH PCH_PATH_LOCAL PCH_FILE_NAME)

    SET(PCH_PATH_LOCAL_INTERNAL ${PCH_PATH_LOCAL})
    IF (${PCH_PATH_LOCAL_INTERNAL} STREQUAL "./" OR ${PCH_PATH_LOCAL_INTERNAL} STREQUAL ".")
        SET(PCH_PATH_LOCAL_INTERNAL)
    ELSE()
        SET(PCH_PATH_LOCAL_INTERNAL ${PCH_PATH_LOCAL_INTERNAL}/)
    ENDIF()

    SET(PCH_FILE_HEADER_IN_SOURCE ${PROJECT_PATH}/${PCH_PATH_LOCAL_INTERNAL}${PCH_FILE_NAME}.h)
    SET(PCH_BINARY_OUT ${CMAKE_CURRENT_BINARY_DIR}/${PCH_FILE_NAME}.pch)
    SET(PHP_FILE_CPP ${PCH_PATH_LOCAL_INTERNAL}${PCH_FILE_NAME}.cpp)

    FOREACH(source_file ${SOURCE_FILES})
        GET_FILENAME_COMPONENT(CPP ${source_file} EXT)
        IF(${CPP} STREQUAL ".cpp")
            IF(${source_file} STREQUAL ${PHP_FILE_CPP})
                SET_SOURCE_FILES_PROPERTIES(${PHP_FILE_CPP}
                    PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} /Yc${PCH_FILE_HEADER_IN_SOURCE} /Fp${PCH_BINARY_OUT}"
                )
                SET_SOURCE_FILES_PROPERTIES(${PHP_FILE_CPP}
                    PROPERTIES OBJECT_OUTPUTS ${PCH_BINARY_OUT}
                )
            ELSE()
                LIST(FIND NO_PCH_FILES ${source_file} no_pch_index)
                IF(${no_pch_index} EQUAL -1)
                    SET_SOURCE_FILES_PROPERTIES(${source_file}
                        PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} /Yu${PCH_FILE_HEADER_IN_SOURCE} /Fp${PCH_BINARY_OUT}"
                    )
                    SET_SOURCE_FILES_PROPERTIES(${source_file}
                        PROPERTIES OBJECT_DEPENDS ${PCH_BINARY_OUT}
                    )
                ENDIF(${no_pch_index} EQUAL -1)
            ENDIF()
        ENDIF()
    ENDFOREACH(source_file)

ENDMACRO(ADD_PCH)
