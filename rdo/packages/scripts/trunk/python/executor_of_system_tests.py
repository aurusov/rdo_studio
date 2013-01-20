###############################################################################
# Copyright (c) 2012-2013 Evgeny Proydakov <lord.tiran@gmail.com>
###############################################################################

import os
import sys
import uuid
import shutil
import argparse
import subprocess
import xml.dom.minidom

###############################################################################
#                                 constant                                    #
###############################################################################

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
    rdo_ex_substr      = u'rdo.exe'
    rdo_test_ex_substr = u'rdo_check_results.exe'

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

# rdo_check_result
RDO_CHECK_RESULT_TERMINATION_NORMAL                 = 0
RDO_CHECK_RESULT_TERMINATION_ERROR_FILE_NOT_FOUND   = 1
RDO_CHECK_RESULT_TERMINATION_ERROR_INVALID_INPUT    = 2
RDO_CHECK_RESULT_TERMINATION_ERROR_RESULT           = 3
RDO_CHECK_RESULT_TERMINATION_ERROR_TRACE            = 4
RDO_CHECK_RESULT_TERMINATION_ERROR_RESULT_AND_TRACE = 5

# ancillary data
dividing_line = '-------------------------------------------------------------------------------'

###############################################################################
#                                 functions                                   #
###############################################################################

def safe_encode(string):
    return string.encode(sys.getfilesystemencoding())


def print_list_of_line(list):
    for string in list:
        print string


def cut_slash(list):
    for index in range(len(list)):
        list[index] = list[index].strip()
        list[index] = list[index].replace('\n', '')
        list[index] = list[index].rstrip()


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


def get_test_files(dir):
    files = get_files_list(dir)
    
    test_files = []
    
    for file in files:
        res = file.endswith(test_expansion)
        if res == True:
            test_files.append(file)

    return test_files


def get_executables(dir):
    files = get_files_list(dir)

    rdo_ex = u''
    rdo_test_ex = u''

    for file in files:
        res = file.endswith(rdo_ex_substr)
        if res == True:
            rdo_ex = file

        res = file.endswith(rdo_test_ex_substr)
        if res == True:
            rdo_test_ex = file

        if rdo_ex and rdo_test_ex:
           break

    return rdo_ex, rdo_test_ex


def get_node_attribute_from_dom(dom, node_text, attribute_text):

    nodes = dom.getElementsByTagName(node_text)
    if not len(nodes):
        return u''

    node = nodes[0]
    attribute_text_data = node.getAttribute(attribute_text)

    return attribute_text_data


def wrap_the_string_in_quotes(string):

    new_string = u'"' + string + u'"'
    return new_string


def compare_text_files(file1, file2):

    file1_data = open(file1, 'r').readlines()
    file2_data = open(file2, 'r').readlines()

    cut_slash(file1_data)
    cut_slash(file2_data)
    
    if cmp(file1_data, file2_data) == 0:
        return True
    
    return False

###############################################################################
#                                 main code                                   #
###############################################################################

# parse console options

parser = argparse.ArgumentParser(description = u'rdo executor of system tests:', version = u'0.7.0')
parser.add_argument(u'-ad', action = 'store', dest = u'app_directory',   default = app_directory,   help = u'application directory')
parser.add_argument(u'-md', action = 'store', dest = u'model_directory', default = model_directory, help = u'model directory')

args = parser.parse_args()

app_directory   = u'' + args.app_directory
model_directory = u'' + args.model_directory

# global exit code variable
G_EXIT_CODE = APP_CODE_TERMINATION_NORMAL

print u'STARTED SCRIPT :', sys.argv[0]
print u'ENCODING', sys.getfilesystemencoding()

# search rdo and rdo_test executables
executables = get_executables(app_directory)

rdo_ex      = executables[0]
rdo_test_ex = executables[1]

if not os.path.exists(rdo_ex) or not os.path.exists(rdo_test_ex):
    print u'Build app not found. Critical error !!!'
    exit(EXIT_CODE_TERMINATION_ERROR)

# search .rtestx files
files = get_test_files(model_directory)
files.sort()

print u'\nDEBUG INFO'

print u'\nFind RDO executables    :'
print_list_of_line(executables)

print '\nFind test project files :'
print_list_of_line(files)

# parse xml and start tests
print u'\nSTARTED TEST CYCLE\n'

# check model cycle
for task in files:

    print dividing_line

    utask   = task
    dirname = os.path.dirname(utask) + u'/'

    text_task = open(utask, 'r').read()

    parse_result = False
    
    try:
        dom = xml.dom.minidom.parseString(text_task)
        parse_result = True
    
    except:
        print u'PARSE XML ERROR:', utask
        G_EXIT_CODE = APP_CODE_TERMINATION_ERROR
    
    if parse_result:
        model = {}
        model['name']            = get_node_attribute_from_dom(dom, u'model', u'name'           )
        model['target']          = get_node_attribute_from_dom(dom, u'model', u'target'         )
        model['exit_code']       = get_node_attribute_from_dom(dom, u'model', u'exit_code'      )
        model['trace']           = get_node_attribute_from_dom(dom, u'model', u'trace'          )
        model['result']          = get_node_attribute_from_dom(dom, u'model', u'result'         )
        model['log_compilation'] = get_node_attribute_from_dom(dom, u'model', u'log_compilation')
        
        etalons = []
        node_etalons = dom.getElementsByTagName('etalons')
        if len(node_etalons):
            files = node_etalons[0].getElementsByTagName('file')

            for file in files:
                etalon = {}
                etalon['source'] = file.getAttribute('source')
                etalon['target'] = file.getAttribute('target')
                
                etalons.append(etalon)
        
        try:
            exit_code = int(model['exit_code'])
        except:
            exit_code = DEFAULT_EXIT_CODE
        
        print u'Project              :', task
        print u'Model file           :', model['name']
        print u'Target               :', model['target']
        print u'Exit code            :', model['exit_code']
        print u'Trace file           :', model['trace']
        print u'Result file          :', model['result']
        print u'Log compilation file :', model['log_compilation']

        if len(etalons):
            print '\n', u'Etalons :', '\n'
        
        for etalon in etalons:
            print u'source:', etalon['source'], u'target', etalon['target']

        print ''

        model_file    = dirname + model['name']
        etalon_trace  = dirname + model['trace']
        etalon_result = dirname + model['result']

        model_name = model['name'].partition('.')[0]

        simulation_trace = dirname + model_name + trace_expansion
        simulation_result = dirname + model_name + result_expansion

        cycle_exit_code = APP_CODE_TERMINATION_ERROR

        # select console target
        if model['target'] == TARGET_CONSOLE:
            # run rdo_console app on test model
            command = (rdo_ex + u' -i ' + wrap_the_string_in_quotes(model_file))
            simulation_code = subprocess.call(safe_encode(command), shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            print u'SIMYLATION EXIT CODE :', simulation_code

            # check simulation exit code
            simulation_exit_code_string = u'ERROR'

            if simulation_code == exit_code:
                simulation_exit_code_string = u'OK'
            else:
                cycle_exit_code = APP_CODE_TERMINATION_ERROR
            
            print u'CHECK SIM EXIT CODE  :', simulation_exit_code_string    
            
            # nornal simulation check
            if simulation_code == RDO_CONSOLE_TERMINATION_NORMAL:

                command = (rdo_test_ex + ' -T ' + wrap_the_string_in_quotes(etalon_trace) + ' -R ' + wrap_the_string_in_quotes(etalon_result) 
                                       + ' -t ' + wrap_the_string_in_quotes(simulation_trace) + ' -r ' + wrap_the_string_in_quotes(simulation_result)
                                       )

                test_code = subprocess.call(safe_encode(command), shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

                check_exit_code_string = u'ERROR UNKNOWN'

                if test_code == RDO_CHECK_RESULT_TERMINATION_NORMAL:
                    cycle_exit_code = APP_CODE_TERMINATION_NORMAL
                    check_exit_code_string = u'OK'

                elif test_code == RDO_CHECK_RESULT_TERMINATION_ERROR_FILE_NOT_FOUND:
                    check_exit_code_string = u'ERROR FILE NOT FOUND'

                elif test_code == RDO_CHECK_RESULT_TERMINATION_ERROR_INVALID_INPUT:
                    check_exit_code_string = u'ERROR INVALID INPUT'

                elif test_code == RDO_CHECK_RESULT_TERMINATION_ERROR_RESULT:
                    check_exit_code_string = u'ERROR RESULT'

                elif test_code == RDO_CHECK_RESULT_TERMINATION_ERROR_TRACE:
                    check_exit_code_string = u'ERROR TRACE'            

                elif test_code == RDO_CHECK_RESULT_TERMINATION_ERROR_RESULT_AND_TRACE:
                    check_exit_code_string = u'ERROR RESULT AND TRACE'            

                print u'TEST EXIT CODE       :', test_code
                print u'CHECK TEST CODE      :', check_exit_code_string

            # .rdox model not found
            elif simulation_code == RDO_CONSOLE_TERMINATION_WITH_AN_ERROR_NO_MODEL:
                cycle_exit_code = APP_CODE_TERMINATION_NORMAL

            # check compile error log
            elif simulation_code == RDO_CONSOLE_TERMINATION_WITH_AN_ERROR_PARSE_ERROR:

                simulation_log_file = dirname + RDO_CONSOLE_COMPILE_LOG_FILE_NAME
                simulation_log_file_etalon = dirname + model['log_compilation']

                res = compare_text_files(simulation_log_file, simulation_log_file_etalon)

                check_message_cmp_string = u'ERROR'

                if res:
                    cycle_exit_code = APP_CODE_TERMINATION_NORMAL
                    check_message_cmp_string = u'OK'

                print u'CHECK ERROR LIST     :', check_message_cmp_string 

            elif simulation_code == RDO_CONSOLE_TERMINATION_WITH_AN_ERROR_RUNTIME_ERROR:
                cycle_exit_code = APP_CODE_TERMINATION_NORMAL

        elif model['target'] == TAGRET_CONVERTER:
            text_uuid = str(uuid.uuid1())
            temp_directory_name = dirname + text_uuid + u'/'
            shutil.copytree(dirname, temp_directory_name)
            
            model_file    = temp_directory_name + model['name']
            command = (rdo_ex + u' -i ' + wrap_the_string_in_quotes(model_file) + u' -c')
            convertor_exit_code = subprocess.call(safe_encode(command), shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            print u'CONVERT EXIT CODE :', convertor_exit_code, u'\n'
            
            if convertor_exit_code == exit_code:
                # compare etalons
                for etalon in etalons:
                    source_file = temp_directory_name + etalon['source']
                    target_file = temp_directory_name + etalon['target']
                    res = compare_text_files(source_file, target_file)
                    
                    compare_string = u'ERROR'
                    
                    if res == True:
                        compare_string = u'OK'
                    else:
                        cycle_exit_code = APP_CODE_TERMINATION_ERROR
                    
                    print u'COMPARE', etalon['source'], u'AND', etalon['target'], u':', compare_string
            
            else:
                cycle_exit_code = APP_CODE_TERMINATION_ERROR
            
            shutil.rmtree(temp_directory_name)

        else:
            print 'INVALID TARGET'

        # remove temp files
        #os.remove(simulation_trace)
        #os.remove(simulation_result)

        if cycle_exit_code == APP_CODE_TERMINATION_ERROR:
            G_EXIT_CODE = APP_CODE_TERMINATION_ERROR

print '\n', u'PYTHON EXIT CODE :', G_EXIT_CODE, '\n'

sys.exit(G_EXIT_CODE)
