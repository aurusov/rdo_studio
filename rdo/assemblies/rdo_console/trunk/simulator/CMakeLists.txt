#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

SET(COMPILER_PATH compiler)
SET(RUNTIME_PATH runtime)
SET(SERVICE_PATH service)

ADD_SUBDIRECTORY(${RUNTIME_PATH})
ADD_SUBDIRECTORY(${COMPILER_PATH})
ADD_SUBDIRECTORY(${SERVICE_PATH})
