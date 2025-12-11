#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

IF(BUILD_RDO_CONSOLE)
    ADD_SUBDIRECTORY(rdo_console)
ENDIF()

IF(BUILD_RDO_STUDIO)
    ADD_SUBDIRECTORY(rdo_studio)
ENDIF()
