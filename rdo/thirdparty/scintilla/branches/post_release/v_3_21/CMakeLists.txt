PROJECT(scintilla)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

FIND_PACKAGE(Qt5Widgets REQUIRED)

INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/lexlib)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/rdo)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/qt)

SET(INCLUDE_HEADERS
    include/ILexer.h
    include/Platform.h
    include/SciLexer.h
    include/Scintilla.h
    include/ScintillaWidget.h
    )

SET(LEXLIB_FILES
    lexlib/Accessor.cxx
    lexlib/Accessor.h
    lexlib/CharacterSet.cxx
    lexlib/CharacterSet.h
    lexlib/LexAccessor.h
    lexlib/LexerBase.cxx
    lexlib/LexerBase.h
    lexlib/LexerModule.cxx
    lexlib/LexerModule.h
    lexlib/LexerNoExceptions.cxx
    lexlib/LexerNoExceptions.h
    lexlib/LexerSimple.cxx
    lexlib/LexerSimple.h
    lexlib/OptionSet.h
    lexlib/PropSetSimple.cxx
    lexlib/PropSetSimple.h
    lexlib/SparseState.h
    lexlib/StyleContext.cxx
    lexlib/StyleContext.h
    lexlib/WordList.cxx
    lexlib/WordList.h
    )

SET(RDO_FILES
    rdo/LexFind.cxx
    rdo/LexFind.h
    rdo/LexRdo.cxx
    rdo/LexRdo.h
    rdo/WordListUtil.cxx
    rdo/WordListUtil.h
    )

SET(SCINTILLA_SRC
    src/AutoComplete.cxx
    src/AutoComplete.h
    src/CallTip.cxx
    src/CallTip.h
    src/Catalogue.cxx
    src/Catalogue.h
    src/CellBuffer.cxx
    src/CellBuffer.h
    src/CharClassify.cxx
    src/CharClassify.h
    src/ContractionState.cxx
    src/ContractionState.h
    src/Decoration.cxx
    src/Decoration.h
    src/Document.cxx
    src/Document.h
    src/Editor.cxx
    src/Editor.h
    src/ExternalLexer.cxx
    src/ExternalLexer.h
    src/files.txt
    src/FontQuality.h
    src/Indicator.cxx
    src/Indicator.h
    src/KeyMap.cxx
    src/KeyMap.h
    src/LexGen.py
    src/LineMarker.cxx
    src/LineMarker.h
    src/Partitioning.h
    src/PerLine.cxx
    src/PerLine.h
    src/PositionCache.cxx
    src/PositionCache.h
    src/RESearch.cxx
    src/RESearch.h
    src/RunStyles.cxx
    src/RunStyles.h
    src/ScintillaBase.cxx
    src/ScintillaBase.h
    src/SciTE.properties
    src/Selection.cxx
    src/Selection.h
    src/SplitVector.h
    src/Style.cxx
    src/Style.h
    src/SVector.h
    src/UniConversion.cxx
    src/UniConversion.h
    src/ViewStyle.cxx
    src/ViewStyle.h
    src/XPM.cxx
    src/XPM.h
    )

SET(QT_BINDS
    qt/ScintillaEdit/ScintillaDocument.cpp
    qt/ScintillaEdit/ScintillaDocument.h
    qt/ScintillaEdit/ScintillaEdit.cpp.template
    qt/ScintillaEdit/ScintillaEdit.h.template
    qt/ScintillaEditBase/PlatQt.cpp
    qt/ScintillaEditBase/PlatQt.h
    qt/ScintillaEditBase/ScintillaEditBase.cpp
    qt/ScintillaEditBase/ScintillaEditBase.h
    qt/ScintillaEditBase/ScintillaQt.cpp
    qt/ScintillaEditBase/ScintillaQt.h
    )

SET(QT_HEADER
    qt/ScintillaEditBase/ScintillaQt.h
    qt/ScintillaEditBase/ScintillaEditBase.h
    )

QT5_WRAP_CPP(MOC_SOURCES ${QT_HEADER})

ADD_LIBRARY(scintilla STATIC ${CMAKE_MINIMUM_REQUIRED} ${LEXLIB_FILES} ${RDO_FILES} ${SCINTILLA_SRC} ${QT_BINDS} ${MOC_SOURCES})
TARGET_LINK_LIBRARIES(scintilla ${Qt5Widgets_LIBRARIES})

SET_TARGET_PROPERTIES(scintilla PROPERTIES COMPILE_FLAGS "-DRDO_ST -DSCI_LEXER -D_CRT_SECURE_NO_WARNINGS")
SET_TARGET_PROPERTIES(scintilla PROPERTIES FOLDER ${THIRDPARTY_FOLDERS})

IF(MSVC) # lotions for windows #

SOURCE_GROUP(".include" FILES
    ${HEADERS_INCLUDE}
    )

SOURCE_GROUP(".lexlib" FILES
    ${LEXLIB_FILES}
    )

SOURCE_GROUP(".rdo" FILES
    ${RDO_FILES}
    )

SOURCE_GROUP(".src" FILES
    ${SCINTILLA_SRC}
    )

SOURCE_GROUP(".qt" FILES
    ${QT_BINDS}
    ${MOC_SOURCES}
    )  

ENDIF(MSVC)