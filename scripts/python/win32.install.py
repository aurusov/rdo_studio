# -*- coding: utf-8 -*-

import os
import sys
import shutil
import zipfile


def copy_qt(path_to_qt, path_to_install):
    files = [
        'assistant.exe',
        'icudt52.dll',
        'icuin52.dll',
        'icuuc52.dll',
        'libEGL.dll',
        'libGLESv2.dll',
        'Qt5CLucene.dll',
        'Qt5Core.dll',
        'Qt5Gui.dll',
        'Qt5Help.dll',
        'Qt5Multimedia.dll',
        'Qt5MultimediaWidgets.dll',
        'Qt5Network.dll',
        'Qt5OpenGL.dll',
        'Qt5Positioning.dll',
        'Qt5PrintSupport.dll',
        'Qt5Qml.dll',
        'Qt5Quick.dll',
        'Qt5Sensors.dll',
        'Qt5Sql.dll',
        'Qt5WebKit.dll',
        'Qt5WebKitWidgets.dll',
        'Qt5Widgets.dll',
    ]

    for file in files:
        shutil.copy(path_to_qt + 'bin/' + file, path_to_install + file)

    pluging = [
        'platforms/qminimal.dll',
        'platforms/qwindows.dll',
        'sqldrivers/qsqlite.dll'
    ]

    for plugin in pluging:
        plugin_install_file_name = path_to_install + 'plugins/' + plugin
        plugin_install_path = os.path.normpath(os.path.dirname(plugin_install_file_name))
        if not os.path.exists(plugin_install_path):
            os.makedirs(plugin_install_path)
        shutil.copy(path_to_qt + 'plugins/' + plugin, plugin_install_file_name)

    with open(path_to_install + 'qt.conf', 'w+') as qt_conf:
        qt_conf.write('[Paths]\n')
        qt_conf.write('Plugins=plugins\n')


def copy_help(path_to_help, path_to_install):
    files = [
        'RAO-help.qhc',
        'rdo_lang_rus.qch',
        'rdo_studio_rus.qch',
    ]

    for file in files:
        shutil.copy(path_to_help + file, path_to_install + file)


def copy_doc(path_to_doc, path_to_install):
    with open(path_to_install + 'Если не запускается.txt', encoding='utf-8', mode='w+') as doc:
        doc.write('Установите Visual C++ Redistributable Packages for Visual Studio 2013\n')
        doc.write('http://www.microsoft.com/en-US/download/details.aspx?id=40784\n')


def copy_binary(path_to_binary, path_to_install):
    shutil.copy(path_to_binary + 'rdo.exe', path_to_install + 'rdo.exe')
    shutil.copy(path_to_binary + 'rdo_studio.exe', path_to_install + 'RAO-studio.exe')

    if not os.path.exists(path_to_install + 'plugins'):
        os.makedirs(path_to_install + 'plugins')

    shutil.copy(path_to_binary + 'plugins/plugin_game5.dll', path_to_install + 'plugins/plugin_game5.dll')


def create_zip(path_to_binary, path_to_install):

    with open(path_to_rdo_binary + '../git.version.long') as git_version:
        zip_file_name = 'RAO-studio-' + git_version.readlines()[0].rstrip() + '.zip'

    os.chdir(path_to_install)
    zip_file = zipfile.ZipFile(zip_file_name, 'w')

    for root, dirs, files in os.walk('.'):
        for file in files:
            file_name, file_extension = os.path.splitext(file)
            if file_extension != '.zip':
                zip_file.write(os.path.join(root, file))


if __name__ == '__main__':

    if len(sys.argv) != 3:
        print ('python win32.install.py <path_to_rdo_binary> <path_to_install>')
        exit()

    path_to_rdo_binary = os.path.normpath(sys.argv[1]) + os.sep
    path_to_install = os.path.normpath(sys.argv[2]) + os.sep

    # path_to_rdo_binary = os.path.normpath('../../build/Release') + os.sep
    # path_to_install = os.path.normpath('../../build/install') + os.sep

    path_to_qt = os.path.normpath(str(os.environ["QTDIR"])) + os.sep

    if not os.path.exists(path_to_install):
        os.makedirs(path_to_install)

    copy_qt(path_to_qt, path_to_install)
    copy_help(path_to_rdo_binary + '../', path_to_install)
    copy_doc(path_to_rdo_binary + '../../doc/', path_to_install)
    copy_binary(path_to_rdo_binary, path_to_install)

    create_zip(path_to_rdo_binary, path_to_install)
