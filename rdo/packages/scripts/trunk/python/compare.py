#!/usr/bin/env python

###############################################################################
# Copyright (c) 2013 Evgeny Proydakov <lord.tiran@gmail.com>
###############################################################################
# -*- coding: utf-8 -*-
###############################################################################

import sys
import utils
import re

# compate type
COMPARE_FULL   = 'FULL'
COMPARE_TRACE  = 'TRACE'
COMPARE_RESULT = 'RESULT'

def cmp(a, b):
    return (a > b) - (a < b)

def _read_result_data(name):
    file_data_temp = open(name, 'r').readlines()
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
    file_data_temp = open(name, 'r').readlines()
    utils.cut_slash(file_data_temp)

    file_data = []

    push = False
    for line in file_data_temp:
        if (push):
            line = re.sub(r'SES\s+(\S+)\s(\S+)\s(\S+)', r'SES \1', line)
            line = re.sub(r'DPS_TM\s(\S+)(\s+)(\S+)(\s+)(\S+)', r'DPS_TM', line)
            line = re.sub(r'DPS_ME\s(\S+)(\s+)(\S+)(\s+)(\S+)', r'DPS_ME', line)
            line = re.sub(r'DPS_TT\s(\S+)(\s+)(\S+)(\s+)(\S+)', r'DPS_TT', line)
            line = re.sub(r'DPS_MM\s(\S+)', r'DPS_MM', line)
            file_data.append(line)

        if (line.find('$Changes') != -1):
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

if __name__ == '__main__':

    utils.enc_print (str(files('game5.trc', 'game5_etalon.trc', COMPARE_TRACE)))
