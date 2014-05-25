#!/usr/bin/env python3
# -*- coding: utf-8 -*-
###############################################################################
# Copyright (c) 2014 Evgeny Proydakov <e.proydakov@gmail.com>
###############################################################################

import re
import os
import sys
import shutil
import zipfile
import argparse
import subprocess

###############################################################################
#                                 constant                                    #
###############################################################################

APP_VERSION = '0.0.1'

ISCC_APP = 'iscc.exe'

EXIT_OK    = 0
EXIT_ERROR = 1

###############################################################################
#                                  install                                    #
###############################################################################

INSTALL = []

APPS = ['${build}/rdo.exe',
        '${build}/rdo_studio.exe'
]

QT_CONF = ['${build}/qt.conf']

QT_APPS = ['$env{QTDIR}/bin/assistant.exe'
]

QT_LIBS = ['$env{QTDIR}/bin/Qt5Core.dll',
           '$env{QTDIR}/bin/Qt5Gui.dll',
           '$env{QTDIR}/bin/Qt5Widgets.dll',
           '$env{QTDIR}/bin/Qt5CLucene.dll',
           '$env{QTDIR}/bin/Qt5Help.dll',
           '$env{QTDIR}/bin/Qt5Multimedia.dll',
           '$env{QTDIR}/bin/Qt5MultimediaWidgets.dll',
           '$env{QTDIR}/bin/Qt5Network.dll',
           '$env{QTDIR}/bin/Qt5OpenGL.dll',
           '$env{QTDIR}/bin/Qt5Positioning.dll',
           '$env{QTDIR}/bin/Qt5PrintSupport.dll',
           '$env{QTDIR}/bin/Qt5Qml.dll',
           '$env{QTDIR}/bin/Qt5Quick.dll',
           '$env{QTDIR}/bin/Qt5Sensors.dll',
           '$env{QTDIR}/bin/Qt5Sql.dll',
           '$env{QTDIR}/bin/Qt5WebKit.dll',
           '$env{QTDIR}/bin/Qt5WebKitWidgets.dll'
]

QT_COMMON_LIBS = ['$env{QTDIR}/bin/icudt52.dll',
                  '$env{QTDIR}/bin/icuin52.dll',
                  '$env{QTDIR}/bin/icuuc52.dll',
                  '$env{QTDIR}/bin/libEGL.dll',
                  '$env{QTDIR}/bin/libGLESv2.dll',
                  '$env{QTDIR}/bin/D3DCompiler_47.dll'
]

QT_PLAGINS_PLATFORM_LIBS = ['$env{QTDIR}/plugins/platforms/qwindows.dll',
                            '$env{QTDIR}/plugins/platforms/qminimal.dll'
]

QT_PLAGINS_SQLDRIVERS_LIBS = ['$env{QTDIR}/plugins/sqldrivers/qsqlite.dll'
]

MSVC_LIBS = ['$env{VS120COMNTOOLS}/../../VC/redist/x86/Microsoft.VC120.CRT/msvcr120.dll',
             '$env{VS120COMNTOOLS}/../../VC/redist/x86/Microsoft.VC120.CRT/msvcp120.dll'
]

INSTALL.append({'from': APPS,           'to': '${root}'})
INSTALL.append({'from': QT_CONF,        'to': '${root}'})
INSTALL.append({'from': QT_APPS,        'to': '${root}'})
INSTALL.append({'from': MSVC_LIBS,      'to': '${root}'})
INSTALL.append({'from': QT_LIBS,        'to': '${root}'})
INSTALL.append({'from': QT_COMMON_LIBS, 'to': '${root}'})
INSTALL.append({'from': QT_PLAGINS_PLATFORM_LIBS,   'to': '${root}/plugins/platforms'})
INSTALL.append({'from': QT_PLAGINS_SQLDRIVERS_LIBS, 'to': '${root}/plugins/sqldrivers'})

###############################################################################

suffixes = ['B', 'KB', 'MB', 'GB', 'TB', 'PB']
def humansize(nbytes):
    if nbytes == 0: return '0 B'
    i = 0
    while nbytes >= 1024 and i < len(suffixes)-1:
        nbytes /= 1024.
        i += 1
    f = ('%.2f' % nbytes).rstrip('0').rstrip('.')
    return '%s %s' % (f, suffixes[i])

###############################################################################

def zipdir(path, zip):
    for root, dirs, files in os.walk(path):
        for file in files:
            zip.write(os.path.join(root, file))

###############################################################################

def check_app(app, path):
    find = False
    path = os.environ['PATH']
    path = path.split(";")
    for e in path:
        test_path = os.path.join(e, ISCC_APP)
        if os.path.exists(test_path):
            find = True
            break
    return find

###############################################################################

def process_line(line, replacement):
    if '$env' in line:
        m = re.match(r"\$env{(\w+)}", line)
        if m is None:
            raise Exception('bad line: ' + line)
        env = m.group(1)
        env_val = os.environ[env]
        line = line.replace('$env{' + env + '}', env_val)
    elif '$' in line:
        m = re.match(r"\${(\w+)}", line)
        if m is None:
            raise Exception('bad line: ' + line)
        var = m.group(1)
        var_val = replacement[var]
        line = line.replace('${' + var + '}', var_val)
    return line

def process_from(list, replacement):
    for htable in list:
        tlist = []
        for e in htable['from']:
            tlist.append(process_line(e, replacement))
        htable['from'] = tlist

def process_to(list, replacement):
    for htable in list:
        htable['to'] = process_line(htable['to'], replacement)

###############################################################################

if __name__ == '__main__':

    argparser = argparse.ArgumentParser(description=os.path.basename(__file__))
    argparser.add_argument('-b', action='store', dest='build', default=None, help='build directory')
    argparser.add_argument('-t', action='store', dest='type',  default=None, help='build type (portable|install)')

    args = argparser.parse_args()

    if args.build is None or args.type is None:
        argparser.print_help()
        sys.exit(EXIT_OK)

    version_file = open(os.path.join(args.build, 'git.version.long.txt'), 'r')
    version = version_file.read()
    version_file.close()
    version = version.replace('\n', '')
    version = version.replace('\r', '')

    build = args.build
    release = os.path.join(build, 'Release')
    if os.path.exists(release):
        build = release
    build = os.path.abspath(build)
    replacement = {'build': build}
    process_from(INSTALL, replacement)

    if args.type == 'portable':
        tmp = 'rdo'
        if os.path.exists(tmp):
            shutil.rmtree(tmp)
        os.makedirs(tmp)

        replacement = {'root': tmp}
        process_to(INSTALL, replacement)

        for htable in INSTALL:
            for f in htable['from']:
                to = htable['to']
                if not os.path.exists(to):
                    print('make dir: ' + to)
                    os.makedirs(to)
                print('copy from: ' + f + ' to: ' + to)
                shutil.copy(f, to)

        zipname = os.path.join(args.build, 'rdo-' + version + '.zip')
        zipf = zipfile.ZipFile(zipname, 'w')
        zipdir(tmp, zipf)
        zipf.close()
        shutil.rmtree(tmp)
        print('make zip arch: ' + zipname + ' size: ' + humansize(os.path.getsize(zipname)))
        sys.exit(EXIT_OK)

    elif args.type == 'install':
        if not check_app(ISCC_APP, os.environ['PATH']):
            sys.stdout.write('not found ' + ISCC_APP + '. Check $PATH\n')
            sys.exit(EXIT_ERROR)

        replacement = {'root': '{app}'}
        process_to(INSTALL, replacement)

        template_iss_script = os.path.join(build, 'rdo.win32.install.iss.template')
        iss_script = os.path.join(args.build, 'rdo.win32.install.iss')
        
        shutil.copy(template_iss_script, iss_script)
        file = open(iss_script, 'a')

        for htable in INSTALL:
            for f in htable['from']:
                to = htable['to']
                file.write('Source: "%s"; DestDir: "%s"\n' % (f, to))

        file.close()

        command = ISCC_APP + ' ' + iss_script
        proc = subprocess.Popen(command, shell=True)
        proc.wait()
        sys.exit(EXIT_OK)

    else:
        argparser.print_help()

    sys.exit(EXIT_ERROR)
