###############################################################################
# Copyright (c) 2013 Evgeny Proydakov <lord.tiran@gmail.com>
###############################################################################
 # -*- coding: utf-8 -*-
###############################################################################

import sys
import utils

# compate type
COMPARE_FULL   = u'FULL'
COMPARE_TRACE  = u'TRACE'
COMPARE_RESULT = u'RESULT'

def _read_result_data(name):
    file_data_temp = open(name, u'r').readlines()
    utils.cut_slash(file_data_temp)
    
    file_data = []

    push = False
    for line in file_data_temp:
        if(push):
            file_data.append(line)
        if(line.find('BExpCalcCounter') != -1):
            push = True

    return file_data


def _read_trace_data(name):
    file_data_temp = open(name, u'r').readlines()
    utils.cut_slash(file_data_temp)

    file_data = []

    push = False
    for line in file_data_temp:
        if(line.find('DPS_MM') != -1):
            break;
        if(push):
            file_data.append(line)
        if(line.find('$Changes') != -1):
            push = True

    return file_data


def full(file1, file2):
    file1_data = open(file1, 'r').readlines()
    file2_data = open(file2, 'r').readlines()

    utils.cut_slash(file1_data)
    utils.cut_slash(file2_data)

    if cmp(file1_data, file2_data) == 0:
        return True

    return False


def result(file1, file2):
    file1_data = _read_result_data(file1)
    file2_data = _read_result_data(file2)
    
    if cmp(file1_data, file2_data) == 0:
        return True

    return False


def trace(file1, file2):
    file1_data = _read_trace_data(file1)
    file2_data = _read_trace_data(file2)
    
    if cmp(file1_data, file2_data) == 0:
        return True

    return False

def files(file1, file2, type):
    if   type == COMPARE_FULL:
        return full(file1, file2)
    elif type == COMPARE_TRACE:
        return trace(file1, file2)
    elif type == COMPARE_RESULT:
        return result(file1, file2)
    
    return False