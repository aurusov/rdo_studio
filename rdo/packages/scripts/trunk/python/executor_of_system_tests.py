#!python3

###############################################################################
# Copyright (c) 2012-2013 Evgeny Proydakov <lord.tiran@gmail.com>
###############################################################################
 # -*- coding: utf-8 -*-
###############################################################################

import os
import sys
import uuid
import shutil
import zipfile
import argparse
import traceback
import subprocess
import xml.dom.minidom

import utils
import compare

###############################################################################
#                                 constant                                    #
###############################################################################

APP_VERSION = '0.7.0'

# data about rdo apps
app_directory      = '.'
model_directory    = '.'
test_expansion     = '.rtestx'
trace_expansion    = '.trc'
result_expansion   = '.pmv'
build_dir_substr   = 'build'
rdo_ex_substr      = 'rdo'
rdo_test_ex_substr = 'rdo_check_results'

if sys.platform == 'win32':
    rdo_ex_substr = 'rdo.exe'

RDO_CONSOLE_COMPILE_LOG_FILE_NAME = 'log.txt'

# test target type
TARGET_CONSOLE   = 'CONSOLE'
TAGRET_CONVERTER = 'CONVERTOR'
TARGET_GUI       = 'uGUI'

DEFAULT_EXIT_CODE = 0

# python script exit code
APP_CODE_TERMINATION_NORMAL = 0
APP_CODE_TERMINATION_ERROR  = 1

# rdo console exit code
RDO_CONSOLE_TERMINATION_NORMAL                         = 0
RDO_CONSOLE_TERMINATION_WITH_AN_ERROR_NO_MODEL         = 1
RDO_CONSOLE_TERMINATION_WITH_AN_ERROR_OPEN_MODEL_ERROR = 2
RDO_CONSOLE_TERMINATION_WITH_AN_ERROR_PARSE_ERROR      = 3
RDO_CONSOLE_TERMINATION_WITH_AN_ERROR_RUNTIME_ERROR    = 4

IGNORE_PATTERNS = ('^.git','.svn')

CONSOLE_PARAM_ENCODING = sys.getfilesystemencoding()

DIR_LINE = '/'
if sys.platform == 'win32':
    DIR_LINE = '\\'

# ancillary data
dividing_line = '-------------------------------------------------------------------------------'

###############################################################################
#                                 functions                                   #
###############################################################################

def safe_exit():
    utils.enc_print ('\n', 'PYTHON EXIT CODE :', G_EXIT_CODE, '\n')
    sys.exit(G_EXIT_CODE)

###############################################################################

def get_test_files(dir):
    files = utils.get_files_list(dir)
    
    test_files = []
    
    for file in files:
        res = file.endswith(test_expansion)
        if res == True:
            test_files.append(file)

    return test_files

###############################################################################

def get_executables(dir):
    files = utils.get_files_list(dir)

    rdo_ex = ''

    for file in files:
        res = file.endswith(rdo_ex_substr)
        if res == True:
            rdo_ex = file

        if rdo_ex:
           break

    return rdo_ex

###############################################################################

def get_node_attribute_from_dom(dom, node_text, attribute_text):

    nodes = dom.getElementsByTagName(node_text)
    if not len(nodes):
        return ''

    node = nodes[0]
    attribute_text_data = node.getAttribute(attribute_text)

    return attribute_text_data

###############################################################################

def compare_etalons(etalons, basedir):
    cycle_exit_code = APP_CODE_TERMINATION_NORMAL
    for etalon in etalons:
        source_file = basedir + etalon['source']
        target_file = basedir + etalon['target']
        type = etalon['type']
        res = compare.files(source_file, target_file, type)
                    
        compare_string = 'ERROR'
                    
        if res == True:
            compare_string = 'OK'
        else:
            cycle_exit_code = APP_CODE_TERMINATION_ERROR

        utils.enc_print ('COMPARE:  ', etalon['source'], '  AND  ', etalon['target'], ':  ', compare_string)
    
    return cycle_exit_code

###############################################################################

def test_console(dirname, model):
    # run rdo_console app on test model
    model_file = '' + dirname + model['name']
    command = (rdo_ex + ' -i ' + utils.wrap_the_string_in_quotes(model_file))
    utils.enc_print('Run:', command, '\n')
    simulation_code = subprocess.call(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    utils.enc_print ('SIMULATION EXIT CODE :', simulation_code)

    # check simulation exit code
    simulation_exit_code_string = 'ERROR'
    if simulation_code == exit_code:
        cycle_exit_code = APP_CODE_TERMINATION_NORMAL
        simulation_exit_code_string = 'OK'
    else:
        cycle_exit_code = APP_CODE_TERMINATION_ERROR
            
    utils.enc_print ('CHECK SIM EXIT CODE  :', simulation_exit_code_string, '\n')
            
    # nornal simulation check
    if simulation_code == RDO_CONSOLE_TERMINATION_NORMAL:
        try:
            # compare etalons
            cycle_exit_code = compare_etalons(etalons, dirname)
        except:
            traceback.print_exc(file=sys.stdout)
            cycle_exit_code = APP_CODE_TERMINATION_ERROR

    # check compile error log
    elif simulation_code == RDO_CONSOLE_TERMINATION_WITH_AN_ERROR_PARSE_ERROR:
        try:
            simulation_log_file = dirname + RDO_CONSOLE_COMPILE_LOG_FILE_NAME
            simulation_log_file_etalon = dirname + model['log_compilation']
            res = compare.full(simulation_log_file, simulation_log_file_etalon)
            check_message_cmp_string = 'ERROR'
            if res:
                cycle_exit_code = APP_CODE_TERMINATION_NORMAL
                check_message_cmp_string = 'OK'
            utils.enc_print ('CHECK ERROR LIST     :', check_message_cmp_string)

        except:
            traceback.print_exc(file=sys.stdout)
            cycle_exit_code = APP_CODE_TERMINATION_ERROR

    return cycle_exit_code

###############################################################################

def test_convertor(dirname, model):
    try:
        text_uuid = str(uuid.uuid4())
        temp_directory_name = dirname + text_uuid + DIR_LINE
        shutil.copytree(dirname, temp_directory_name, ignore=shutil.ignore_patterns(*IGNORE_PATTERNS))
        model_file = '' + temp_directory_name + model['name']
        command = (rdo_ex + ' -i ' + utils.wrap_the_string_in_quotes(model_file) + ' -c')
        utils.enc_print ('Run:', command, '\n')
        convertor_exit_code = subprocess.call(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        utils.enc_print ('CONVERT EXIT CODE :', convertor_exit_code, '\n')
        if convertor_exit_code == exit_code:
            cycle_exit_code = compare_etalons(etalons, temp_directory_name)
        if cycle_exit_code != APP_CODE_TERMINATION_NORMAL:
            arc_name = text_uuid + '.zip'
            utils.enc_print('Make zip archive: ' + arc_name)
            zipf = zipfile.ZipFile(arc_name, 'w')
            zipdir(dirname, zipf)
            zipf.close()
        shutil.rmtree(temp_directory_name)
    except:
        traceback.print_exc(file=sys.stdout)
        cycle_exit_code = APP_CODE_TERMINATION_ERROR

    return cycle_exit_code

###############################################################################

def zipdir(path, zip):
    for root, dirs, files in os.walk(path):
        for file in files:
            zip.write(os.path.join(root, file))

###############################################################################
#                                 main code                                   #
###############################################################################

# parse console options
parser = argparse.ArgumentParser(description = 'rdo executor of system tests: ' + str(APP_VERSION))
parser.add_argument('-ad', action = 'store', dest = 'app_directory',   default = app_directory,   help = 'application directory')
parser.add_argument('-md', action = 'store', dest = 'model_directory', default = model_directory, help = 'model directory')
parser.add_argument('-enc', action='store', default = '')
parser.add_argument('-dec', action='store', default = '')

args = parser.parse_args()

app_directory   = '' + args.app_directory
model_directory = '' + args.model_directory

utils.ENC = args.enc
utils.DEC = args.dec

# global exit code variable
G_EXIT_CODE = APP_CODE_TERMINATION_NORMAL

utils.enc_print (dividing_line)
utils.enc_print ('STARTED SCRIPT :', sys.argv[0])
utils.enc_print ('SYSTEM ENCODING', sys.getdefaultencoding())
utils.enc_print ('FILESYSTEM ENCODING', sys.getfilesystemencoding())

# search rdo and rdo_test executables
rdo_ex = get_executables(app_directory)
if not os.path.exists(rdo_ex):
    utils.enc_print ('Build app not found. Critical error !!!')
    sys.exit(APP_CODE_TERMINATION_ERROR)

# search .rtestx files
files = get_test_files(model_directory)
files.sort()

utils.enc_print ('\nDEBUG INFO')
utils.enc_print ('\nFind RDO executables    :')
utils.enc_print (rdo_ex)

utils.enc_print ('\nFind test project files :')
utils.print_list_of_line(files)

# parse xml and start tests
utils.enc_print ('\nSTARTED TEST CYCLE\n')
bad_models = []

# check model cycle
for task in files:

    utils.enc_print (dividing_line)

    utask   = task
    dirname = os.path.dirname(utask) + DIR_LINE

    text_task = open(utask, 'r', encoding = 'utf-8').read()

    parse_result = False
    
    try:
        dom = xml.dom.minidom.parseString(text_task)
        parse_result = True
    
    except:
        utils.enc_print ('PARSE XML ERROR:', utask)
        traceback.print_exc(file=sys.stdout)
        G_EXIT_CODE = APP_CODE_TERMINATION_ERROR
        safe_exit()

    if parse_result:
        model = {}
        model['name']            = get_node_attribute_from_dom(dom, 'model', 'name'           )
        model['target']          = get_node_attribute_from_dom(dom, 'model', 'target'         )
        model['exit_code']       = get_node_attribute_from_dom(dom, 'model', 'exit_code'      )
        model['log_compilation'] = get_node_attribute_from_dom(dom, 'model', 'log_compilation')

        utils.enc_print ('Project              :', task)
        utils.enc_print ('Model file           :', model['name'])
        utils.enc_print ('Target               :', model['target'])
        utils.enc_print ('Exit code            :', model['exit_code'])
        utils.enc_print ('Log compilation file :', model['log_compilation'])
        sys.stdout.flush()

        etalons = []
        node_etalons = dom.getElementsByTagName('etalons')
        if len(node_etalons):
            files = node_etalons[0].getElementsByTagName('file')
            for file in files:
                etalon = {}
                etalon['source'] = file.getAttribute('source')
                etalon['target'] = file.getAttribute('target')
                etalon['type']   = file.getAttribute('type')
                etalons.append(etalon)
        try:
            exit_code = int(model['exit_code'])
        except:
            traceback.print_exc(file=sys.stdout)
            exit_code = DEFAULT_EXIT_CODE

        if len(etalons):
            utils.enc_print ('\n', 'Etalons :', '\n')
        for etalon in etalons:
            utils.enc_print ('SOURCE:  ', etalon['source'], '  TARGET:  ', etalon['target'], '  TYPE:  ',   etalon['type'])
        utils.enc_print ('')

        cycle_exit_code = APP_CODE_TERMINATION_ERROR

        # select console target
        if model['target'] == TARGET_CONSOLE:
            cycle_exit_code = test_console(dirname, model)
        elif model['target'] == TAGRET_CONVERTER:
            cycle_exit_code = test_convertor(dirname, model)
        else:
            utils.enc_print ('INVALID TARGET')

        if cycle_exit_code == APP_CODE_TERMINATION_ERROR:
            bad_models.append(task)
            G_EXIT_CODE = APP_CODE_TERMINATION_ERROR

utils.enc_print ('\n', 'MODEL WITH ERRORS:')
utils.print_list_of_line(bad_models)

if(len(bad_models) < 1):
    utils.enc_print ('(empty)')

safe_exit()
