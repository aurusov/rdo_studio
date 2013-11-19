###############################################################################
# Copyright (c) 2013 Evgeny Proydakov <lord.tiran@gmail.com>
###############################################################################
 # -*- coding: utf-8 -*-
###############################################################################

ENC = ''
DEC = ''

import os, sys

def enc_print(*args):
    flag = False
    for arg in args:
        if flag:
            sys.stdout.write(' ')
        if len(ENC)*len(DEC):
            sys.stdout.write(str(arg).encode(ENC).decode(DEC))
        else:
            sys.stdout.write(str(arg))
        flag = True
    sys.stdout.write('\n')
        

def print_list_of_line(list):
    for string in list:
        enc_print (string)


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
