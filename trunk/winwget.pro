TEMPLATE	+= app
TARGET		+= winwget
include( $(QTDIR)/.qmake.cache )
CONFIG		+= qt warn_on release
OBJECTS_DIR	= .
MOC_DIR		= .

HEADERS += about_wget.h\
           default_options.h\
           int_validator.h\
           download_options.h\
           log_view.h\
           mainform.h\
           wget_process.h\
           processes_list.h

INTERFACES += about_wget_base.ui\
              about_winwget_base.ui\
              default_options_base.ui\
              download_options_base.ui

SOURCES += about_wget.cpp\
           default_options.cpp\
           download_options.cpp\
           int_validator.cpp\
           log_view.cpp\
           main.cpp\
           mainform.cpp\
           processes_list.cpp\
           unicode_log.cpp\
           wget_process.cpp

win32-msvc:RC_FILE = winwget.rc
