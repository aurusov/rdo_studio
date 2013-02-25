###############################################################################
# Copyright (c) 2013 Evgeny Proydakov <lord.tiran@gmail.com>
###############################################################################
 # -*- coding: utf-8 -*-
###############################################################################

import os

def safe_encode(string, encoding):
    return string.encode(encoding)


def print_list_of_line(list, encoding):
    for string in list:
        print safe_encode(string, encoding)


def get_files_list(dir):
    dirs  = []
    nfile = []
    files = []
    for dirname, dirnames, filenames in os.walk(dir):
        dirs.append(dirname)
        for subdirname in dirnames:
            dirs.append(os.path.join(dirname, subdirname))
        for filename in filenames:
            files.append(os.path.join(dirname, filename))
    return files


def cut_slash(list):
    for index in range(len(list)):
        list[index] = list[index].strip()
        list[index] = list[index].replace('\n', '')
        list[index] = list[index].rstrip()


def wrap_the_string_in_quotes(string):

    new_string = u'"' + string + u'"'
    return new_string
