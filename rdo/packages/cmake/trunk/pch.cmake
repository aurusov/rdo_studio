#==============================================================================
# Copyright (c) 2013 Urusov Andrey <drobus@gmail.com>
#==============================================================================

MACRO(ADD_PCH SOURCE_FILES NO_PCH_FILES PCH_FILE PCH_PATH)
    FOREACH(source_file ${SOURCE_FILES})
        GET_FILENAME_COMPONENT(CPP ${source_file} EXT)
        IF (${CPP} STREQUAL ".cpp")
            LIST(FIND NO_PCH_FILES ${source_file} no_pch_index)
            IF (${no_pch_index} EQUAL -1)
                SET_SOURCE_FILES_PROPERTIES(${source_file}
                    PROPERTIES COMPILE_FLAGS "/Yu${PCH_PATH}/${PCH_FILE}.h"
                )
            ENDIF (${no_pch_index} EQUAL -1)
        ENDIF (${CPP} STREQUAL ".cpp")
    ENDFOREACH(source_file)

    SET_SOURCE_FILES_PROPERTIES(${PCH_FILE}.cpp
        PROPERTIES COMPILE_FLAGS "/Yc${PCH_PATH}/${PCH_FILE}.h"
    )
ENDMACRO(ADD_PCH)
