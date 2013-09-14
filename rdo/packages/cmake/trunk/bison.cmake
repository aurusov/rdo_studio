#==============================================================================
# Copyright (c) 2013 Urusov Andrey <drobus@gmail.com>
#==============================================================================

MACRO(RDO_BISON_TARGET FILE_TYPE)
	SET(BISON_NAME ${FILE_TYPE})
	SET(BISON_DEFINE)
	SET(BISON_${FILE_TYPE}_OUTPUT_HEADER)
	IF(${ARGC} GREATER 2 OR ${ARGC} EQUAL 2)
		SET(BISON_NAME ${ARGV1})
		IF(${ARGC} GREATER 3 OR ${ARGC} EQUAL 3)
			SET(BISON_${FILE_TYPE}_OUTPUT_HEADER ${GRAMMA_PATH}/rdogram${FILE_TYPE}.hpp)
			SET(BISON_DEFINE --defines=${BISON_${FILE_TYPE}_OUTPUT_HEADER})
		ENDIF()
	ENDIF()

	ADD_CUSTOM_COMMAND(
		OUTPUT ${GRAMMA_PATH}/rdogram${FILE_TYPE}.cpp ${GRAMMA_PATH}/rdogram${FILE_TYPE}.hpp
		COMMAND set CYGWIN=nodosfilewarning
		COMMAND set BISON_PKGDATADIR=${BISON_FLEX_DIRECTORY}/share/bison
		COMMAND ${BISON_EXECUTABLE} ARGS -g -v ${BISON_DEFINE} -p${BISON_NAME} -o ${GRAMMA_PATH}/rdogram${FILE_TYPE}.cpp ${GRAMMA_PATH}/rdo${FILE_TYPE}.y
		DEPENDS ${GRAMMA_PATH}/rdo${FILE_TYPE}.y
		COMMENT "[BISON][rdo${FILE_TYPE}] Building parser with bison ${BISON_VERSION}"
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
ENDMACRO(RDO_BISON_TARGET)
