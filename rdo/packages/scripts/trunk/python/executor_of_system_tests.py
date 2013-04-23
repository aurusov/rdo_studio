###############################################################################
# Copyright (c) 2012-2013 Evgeny Proydakov <lord.tiran@gmail.com>
###############################################################################
 # -*- coding: utf-8 -*-
###############################################################################

import os
import sys
import uuid
import shutil
import argparse
import traceback
import subprocess
import xml.dom.minidom

import utils
import compare

###############################################################################
#                                 constant                                    #
###############################################################################

APP_VERSION = u'0.7.0'

# data about rdo apps
app_directory      = u'.'
model_directory    = u'.'
test_expansion     = u'.rtestx'
trace_expansion    = u'.trc'
result_expansion   = u'.pmv'
build_dir_substr   = u'build'
rdo_ex_substr      = u'rdo'
rdo_test_ex_substr = u'rdo_check_results'

if sys.platform == 'win32':
    rdo_ex_substr = u'rdo.exe'

RDO_CONSOLE_COMPILE_LOG_FILE_NAME = 'log.txt'

# test target type
TARGET_CONSOLE   = u'CONSOLE'
TAGRET_CONVERTER = u'CONVERTOR'
TARGET_GUI       = u'uGUI'

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

DIR_LINE = u'/'
if sys.platform == 'win32':
    DIR_LINE = u'\\'

# ancillary data
dividing_line = '-------------------------------------------------------------------------------'

###############################################################################
#                                 functions                                   #
###############################################################################

def get_test_files(dir):
    files = utils.get_files_list(dir)
    
    test_files = []
    
    for file in files:
        res = file.endswith(test_expansion)
        if res == True:
            test_files.append(file)

    return test_files


def get_executables(dir):
    files = utils.get_files_list(dir)

    rdo_ex = u''

    for file in files:
        res = file.endswith(rdo_ex_substr)
        if res == True:
            rdo_ex = file

        if rdo_ex:
           break

    return rdo_ex


def get_node_attribute_from_dom(dom, node_text, attribute_text):

    nodes = dom.getElementsByTagName(node_text)
    if not len(nodes):
        return u''

    node = nodes[0]
    attribute_text_data = node.getAttribute(attribute_text)

    return attribute_text_data


def compare_etalons(etalons, basedir):
    cycle_exit_code = APP_CODE_TERMINATION_NORMAL
    for etalon in etalons:
        source_file = basedir + etalon['source']
        target_file = basedir + etalon['target']
        type = etalon['type']
        res = compare.files(source_file, target_file, type)
                    
        compare_string = u'ERROR'
                    
        if res == True:
            compare_string = u'OK'
        else:
            cycle_exit_code = APP_CODE_TERMINATION_ERROR

        print u'COMPARE:  ', utils.safe_encode(etalon['source'], sys.getfilesystemencoding()), u'  AND  ', utils.safe_encode(etalon['target'], sys.getfilesystemencoding()), u':  ', compare_string
    
    return cycle_exit_code

###############################################################################
#                                 main code                                   #
###############################################################################

# parse console options

parser = argparse.ArgumentParser(description = u'rdo executor of system tests:', version = APP_VERSION)
parser.add_argument(u'-ad', action = 'store', dest = u'app_directory',   default = app_directory,   help = u'application directory')
parser.add_argument(u'-md', action = 'store', dest = u'model_directory', default = model_directory, help = u'model directory')

args = parser.parse_args()

app_directory   = u'' + args.app_directory
model_directory = u'' + args.model_directory

# global exit code variable
G_EXIT_CODE = APP_CODE_TERMINATION_NORMAL

print u'STARTED SCRIPT :', sys.argv[0]
print u'SYSTEM ENCODING', sys.getdefaultencoding()
print u'FILESYSTEM ENCODING', sys.getfilesystemencoding()

# search rdo and rdo_test executables
rdo_ex = get_executables(app_directory)

if not os.path.exists(rdo_ex):
    print u'Build app not found. Critical error !!!'
    sys.exit(APP_CODE_TERMINATION_ERROR)

# search .rtestx files
files = get_test_files(model_directory)
files.sort()

print u'\nDEBUG INFO'

print u'\nFind RDO executables    :'
print utils.safe_encode(rdo_ex, sys.getfilesystemencoding())

print '\nFind test project files :'
utils.print_list_of_line(files, sys.getfilesystemencoding())

# parse xml and start tests
print u'\nSTARTED TEST CYCLE\n'

bad_models = []

# check model cycle
for task in files:

    print dividing_line

    utask   = task
    dirname = os.path.dirname(utask) + DIR_LINE

    text_task = open(utask, 'r').read()

    parse_result = False
    
    try:
        dom = xml.dom.minidom.parseString(text_task)
        parse_result = True
    
    except:
        print u'PARSE XML ERROR:', utask
        traceback.print_exc(file=sys.stdout)
        G_EXIT_CODE = APP_CODE_TERMINATION_ERROR
    
    if parse_result:
        model = {}
        model['name']            = get_node_attribute_from_dom(dom, u'model', u'name'           )
        model['target']          = get_node_attribute_from_dom(dom, u'model', u'target'         )
        model['exit_code']       = get_node_attribute_from_dom(dom, u'model', u'exit_code'      )
        model['log_compilation'] = get_node_attribute_from_dom(dom, u'model', u'log_compilation')
        
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
        
        print u'Project              :', utils.safe_encode(task, sys.getfilesystemencoding())
        print u'Model file           :', utils.safe_encode(model['name'], sys.getfilesystemencoding())
        print u'Target               :', utils.safe_encode(model['target'], sys.getfilesystemencoding())
        print u'Exit code            :', utils.safe_encode(model['exit_code'], sys.getfilesystemencoding())
        print u'Log compilation file :', utils.safe_encode(model['log_compilation'], sys.getfilesystemencoding())

        if len(etalons):
            print '\n', u'Etalons :', '\n'
        
        for etalon in etalons:
            print u'SOURCE:  ', utils.safe_encode(etalon['source'], sys.getfilesystemencoding()), u'  TARGET:  ', utils.safe_encode(etalon['target'], sys.getfilesystemencoding()), u'  TYPE:  ',   etalon['type']

        print ''

        model_file    = dirname + model['name']
        model_name = model['name'].partition('.')[0]

        cycle_exit_code = APP_CODE_TERMINATION_ERROR

        # select console target
        if model['target'] == TARGET_CONSOLE:
            # run rdo_console app on test model

            command = (rdo_ex + u' -i ' + utils.wrap_the_string_in_quotes(model_file))
            print 'Run:', utils.safe_encode(command, 'UTF-8'), '\n'
            simulation_code = subprocess.call(utils.safe_encode(command, CONSOLE_PARAM_ENCODING), shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

            print u'SIMYLATION EXIT CODE :', simulation_code

            # check simulation exit code
            simulation_exit_code_string = u'ERROR'

            if simulation_code == exit_code:
                simulation_exit_code_string = u'OK'
            else:
                cycle_exit_code = APP_CODE_TERMINATION_ERROR
            
            print u'CHECK SIM EXIT CODE  :', simulation_exit_code_string, u'\n'  
            
            # nornal simulation check
            if simulation_code == RDO_CONSOLE_TERMINATION_NORMAL:

                try:
                    # compare etalons
                    cycle_exit_code = compare_etalons(etalons, dirname)

                except:
                    traceback.print_exc(file=sys.stdout)
                    cycle_exit_code = APP_CODE_TERMINATION_ERROR

            # .rdox model not found
            elif simulation_code == RDO_CONSOLE_TERMINATION_WITH_AN_ERROR_NO_MODEL:
                cycle_exit_code = APP_CODE_TERMINATION_NORMAL

            # check compile error log
            elif simulation_code == RDO_CONSOLE_TERMINATION_WITH_AN_ERROR_PARSE_ERROR:
                try:
                    simulation_log_file = dirname + RDO_CONSOLE_COMPILE_LOG_FILE_NAME
                    simulation_log_file_etalon = dirname + model['log_compilation']

                    res = compare.full(simulation_log_file, simulation_log_file_etalon)

                    check_message_cmp_string = u'ERROR'

                    if res:
                        cycle_exit_code = APP_CODE_TERMINATION_NORMAL
                        check_message_cmp_string = u'OK'

                    print u'CHECK ERROR LIST     :', check_message_cmp_string

                except:
                    traceback.print_exc(file=sys.stdout)
                    cycle_exit_code = APP_CODE_TERMINATION_ERROR

            elif simulation_code == RDO_CONSOLE_TERMINATION_WITH_AN_ERROR_RUNTIME_ERROR:
                cycle_exit_code = APP_CODE_TERMINATION_NORMAL

        elif model['target'] == TAGRET_CONVERTER:
            try:
                text_uuid = str(uuid.uuid4())
                temp_directory_name = dirname + text_uuid + DIR_LINE
                
                shutil.copytree(dirname, temp_directory_name, ignore=shutil.ignore_patterns(*IGNORE_PATTERNS))
                
                model_file = temp_directory_name + model['name']
                command = (rdo_ex + u' -i ' + utils.wrap_the_string_in_quotes(model_file) + u' -c')
                print 'Run:', utils.safe_encode(command, 'UTF-8'), '\n'
                convertor_exit_code = subprocess.call(utils.safe_encode(command, 'UTF-8'), shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                
                print u'CONVERT EXIT CODE :', convertor_exit_code, u'\n'
                
                if convertor_exit_code == exit_code:
                    cycle_exit_code = compare_etalons(etalons, temp_directory_name)
                
                shutil.rmtree(temp_directory_name)
            except:
                traceback.print_exc(file=sys.stdout)
                cycle_exit_code = APP_CODE_TERMINATION_ERROR

        else:
            print 'INVALID TARGET'

        if cycle_exit_code == APP_CODE_TERMINATION_ERROR:
            bad_models.append(task)
            G_EXIT_CODE = APP_CODE_TERMINATION_ERROR

print '\n', u'MODEL WITH ERRORS:'
utils.print_list_of_line(bad_models, sys.getfilesystemencoding())

if(len(bad_models) < 1):
   print u'(empty)'

print '\n', u'PYTHON EXIT CODE :', G_EXIT_CODE, '\n'

sys.exit(G_EXIT_CODE)
