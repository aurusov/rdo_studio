###############################################################################
# Copyright (c) 2013 Evgeny Proydakov <lord.tiran@gmail.com>
###############################################################################
 # -*- coding: utf-8 -*-
###############################################################################

import utils

def full(file1, file2):
    file1_data = open(file1, 'r').readlines()
    file2_data = open(file2, 'r').readlines()

    utils.cut_slash(file1_data)
    utils.cut_slash(file2_data)

    if cmp(file1_data, file2_data) == 0:
        return True

    return False

def result(file1, file2):
    return False

def trace(file1, file2):
    return False
