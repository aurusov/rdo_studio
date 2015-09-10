IF(MSVC_IDE)
    SET(CONFIG_DIRECTORY_DEBUG   ${EXECUTABLE_OUTPUT_PATH}/Debug  )
    SET(CONFIG_DIRECTORY_RELEASE ${EXECUTABLE_OUTPUT_PATH}/Release)
ELSE()
    SET(CONFIG_DIRECTORY_DEBUG   ${EXECUTABLE_OUTPUT_PATH})
    SET(CONFIG_DIRECTORY_RELEASE ${EXECUTABLE_OUTPUT_PATH})
ENDIF()

SET(QT_PLUGINS            plugins)
SET(QT_PLUGINS_PLATFORMS  ${QT_PLUGINS}/platforms)
SET(QT_PLUGINS_SQLDRIVERS ${QT_PLUGINS}/sqldrivers)

SET(QT_LIBRARY_DIR             $ENV{QTDIR}/bin                     )
SET(QT_PLUGINS_PLATFORMS_DIR   $ENV{QTDIR}/${QT_PLUGINS_PLATFORMS} )
SET(QT_PLUGINS_SQLDRIVERS_DIR  $ENV{QTDIR}/${QT_PLUGINS_SQLDRIVERS})

#FIND_FILE(DLL_D3DCOMPILER D3DCompiler_43.dll PATHS ${QT_LIBRARY_DIR})
SET(DLL_D_AND_R
#    ${DLL_D3DCOMPILER}
)

FIND_FILE(DLL_R_PLUGINS_QWINDOWS qwindows.dll  PATHS ${QT_PLUGINS_PLATFORMS_DIR})
FIND_FILE(DLL_D_PLUGINS_QWINDOWS qwindowsd.dll PATHS ${QT_PLUGINS_PLATFORMS_DIR})
FIND_FILE(DLL_R_PLUGINS_QMINIMAL qminimal.dll  PATHS ${QT_PLUGINS_PLATFORMS_DIR})
FIND_FILE(DLL_D_PLUGINS_QMINIMAL qminimald.dll PATHS ${QT_PLUGINS_PLATFORMS_DIR})

SET(CONFIG_PLUGINS_PLATFORM_R
    ${DLL_R_PLUGINS_QWINDOWS}
    ${DLL_R_PLUGINS_QMINIMAL}
)

SET(CONFIG_PLUGINS_PLATFORM_D
    ${DLL_D_PLUGINS_QWINDOWS}
    ${DLL_D_PLUGINS_QMINIMAL}
)

FILE(GLOB DLL_ICUUC ${QT_LIBRARY_DIR}/icuuc*.dll)
FILE(GLOB DLL_ICUIN ${QT_LIBRARY_DIR}/icuin*.dll)
FILE(GLOB DLL_ICUDT ${QT_LIBRARY_DIR}/icudt*.dll)

FIND_FILE(DLL_R_QT_CORE               Qt5Core.dll              PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_D_QT_CORE               Qt5Cored.dll             PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_R_QT_CUI                Qt5Gui.dll               PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_D_QT_CUI                Qt5Guid.dll              PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_R_QT_WIDGETS            Qt5Widgets.dll           PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_D_QT_WIDGETS            Qt5Widgetsd.dll          PATHS ${QT_LIBRARY_DIR})

FIND_FILE(EXE_R_ASSISTANT             assistant.exe            PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_R_QT_CLUCENE            Qt5CLucene.dll           PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_R_QT_HELP               Qt5Help.dll              PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_R_QT_MULTIMEDIA         Qt5Multimedia.dll        PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_R_QT_MULTIMEDIAWIDGETS  Qt5MultimediaWidgets.dll PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_R_QT_NETWORK            Qt5Network.dll           PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_R_QT_OPENGL             Qt5OpenGL.dll            PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_R_QT_POSITIONING        Qt5Positioning.dll       PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_R_QT_PRINTSUPPORT       Qt5PrintSupport.dll      PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_R_QT_QML                Qt5Qml.dll               PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_R_QT_QUICK              Qt5Quick.dll             PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_R_QT_SENSORS            Qt5Sensors.dll           PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_R_QT_SQL                Qt5Sql.dll               PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_R_QT_WEBKIT             Qt5WebKit.dll            PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_R_QT_WEBKITWIDGETS      Qt5WebKitWidgets.dll     PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_R_PLUGINS_QSQLITE       qsqlite.dll              PATHS ${QT_PLUGINS_SQLDRIVERS_DIR})
SET(CONFIG_ASSISTANT
    ${DLL_ICUUC}
    ${DLL_ICUIN}
    ${DLL_ICUDT}
    ${DLL_R_QT_CORE}
    ${DLL_R_QT_CUI}
    ${DLL_R_QT_WIDGETS}
    ${EXE_R_ASSISTANT}
    ${DLL_R_QT_CLUCENE}
    ${DLL_R_QT_HELP}
    ${DLL_R_QT_MULTIMEDIA}
    ${DLL_R_QT_MULTIMEDIAWIDGETS}
    ${DLL_R_QT_NETWORK}
    ${DLL_R_QT_OPENGL}
    ${DLL_R_QT_POSITIONING}
    ${DLL_R_QT_PRINTSUPPORT}
    ${DLL_R_QT_QML}
    ${DLL_R_QT_QUICK}
    ${DLL_R_QT_SENSORS}
    ${DLL_R_QT_SQL}
    ${DLL_R_QT_WEBKIT}
    ${DLL_R_QT_WEBKITWIDGETS}
)
SET(CONFIG_ASSISTANT_PLUGINS_PLATFORM
    ${CONFIG_PLUGINS_PLATFORM_R}
)
SET(CONFIG_ASSISTANT_PLUGINS_QSQLITE
    ${DLL_R_PLUGINS_QSQLITE}
)

IF(${MSVC_VERSION} MATCHES "1600")
    FIND_FILE(DLL_R_MSVCP msvcp90.dll)
    FIND_FILE(DLL_D_MSVCP msvcp90d.dll)
    FIND_FILE(DLL_R_MSVCR msvcr90.dll)
    FIND_FILE(DLL_D_MSVCR msvcr90d.dll)
ENDIF()

IF(${MSVC_VERSION} MATCHES "1700")
    FIND_FILE(DLL_R_MSVCP msvcp110.dll)
    FIND_FILE(DLL_D_MSVCP msvcp110d.dll)
    FIND_FILE(DLL_R_MSVCR msvcr110.dll)
    FIND_FILE(DLL_D_MSVCR msvcr110d.dll)
ENDIF()

SET(CONFIG_DLL_R
    ${DLL_D_AND_R}
    ${DLL_R_QT_CORE}
    ${DLL_R_QT_CUI}
    ${DLL_R_QT_WIDGETS}
    ${DLL_R_MSVCR}
    ${DLL_R_MSVCP}
)
FILE(COPY ${CONFIG_DLL_R}                      DESTINATION ${CONFIG_DIRECTORY_RELEASE})
FILE(COPY ${CONFIG_PLUGINS_PLATFORM_R}         DESTINATION ${CONFIG_DIRECTORY_RELEASE}/${QT_PLUGINS_PLATFORMS})
FILE(COPY ${CONFIG_ASSISTANT}                  DESTINATION ${CONFIG_DIRECTORY_RELEASE})
FILE(COPY ${CONFIG_ASSISTANT_PLUGINS_PLATFORM} DESTINATION ${CONFIG_DIRECTORY_RELEASE}/${QT_PLUGINS_PLATFORMS})
FILE(COPY ${CONFIG_ASSISTANT_PLUGINS_QSQLITE}  DESTINATION ${CONFIG_DIRECTORY_RELEASE}/${QT_PLUGINS_SQLDRIVERS})

SET(CONFIG_DLL_D
    ${DLL_D_AND_R}
    ${DLL_D_QT_CORE}
    ${DLL_D_QT_CUI}
    ${DLL_D_QT_WIDGETS}
    ${DLL_D_MSVCR}
    ${DLL_D_MSVCP}
    ${DLL_R_MSVCR}
    ${DLL_R_MSVCP}
)
FILE(COPY ${CONFIG_DLL_D}                      DESTINATION ${CONFIG_DIRECTORY_DEBUG})
FILE(COPY ${CONFIG_PLUGINS_PLATFORM_D}         DESTINATION ${CONFIG_DIRECTORY_DEBUG}/${QT_PLUGINS_PLATFORMS})
FILE(COPY ${CONFIG_ASSISTANT}                  DESTINATION ${CONFIG_DIRECTORY_DEBUG})
FILE(COPY ${CONFIG_ASSISTANT_PLUGINS_PLATFORM} DESTINATION ${CONFIG_DIRECTORY_DEBUG}/${QT_PLUGINS_PLATFORMS})
FILE(COPY ${CONFIG_ASSISTANT_PLUGINS_QSQLITE}  DESTINATION ${CONFIG_DIRECTORY_DEBUG}/${QT_PLUGINS_SQLDRIVERS})

FIND_FILE(DLL_R_GLES libEGL.dll     PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_D_GLES libEGLd.dll    PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_R_EGL  libGLESv2.dll  PATHS ${QT_LIBRARY_DIR})
FIND_FILE(DLL_D_EGL  libGLESv2d.dll PATHS ${QT_LIBRARY_DIR})

SET(CONFIG_DLL_R_OPTIONAL
    ${DLL_R_GLES}
    ${DLL_R_EGL}
)

SET(CONFIG_DLL_D_OPTIONAL
    ${CONFIG_DLL_R_OPTIONAL}
    ${DLL_D_GLES}
    ${DLL_D_EGL}
)

FOREACH(DLL_FILE_NAME ${CONFIG_DLL_R_OPTIONAL})
    IF(DLL_FILE_NAME)
        FILE(COPY ${DLL_FILE_NAME} DESTINATION ${CONFIG_DIRECTORY_RELEASE})
    ENDIF()
ENDFOREACH()

FOREACH(DLL_FILE_NAME ${CONFIG_DLL_D_OPTIONAL})
    IF(DLL_FILE_NAME)
        FILE(COPY ${DLL_FILE_NAME} DESTINATION ${CONFIG_DIRECTORY_DEBUG})
    ENDIF()
ENDFOREACH()

FILE(COPY ${CMAKE_MODULE_PATH}/qt.conf  DESTINATION ${CONFIG_DIRECTORY_DEBUG})
FILE(COPY ${CMAKE_MODULE_PATH}/qt.conf  DESTINATION ${CONFIG_DIRECTORY_RELEASE})

CONFIGURE_FILE(${CMAKE_MODULE_PATH}/win32.install.iss.cmake ${CONFIG_DIRECTORY_RELEASE}/win32.install.iss)
