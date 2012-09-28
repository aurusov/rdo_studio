###############################################################################
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
###############################################################################

import os
import sys
import subprocess
import xml.dom.minidom

###############################################################################
#                                 constant                                    #
###############################################################################

# data about rdo apps
directory           = '.'
model_directory     = '.'
test_expansion      = '.rtestx'
trace_expansion     = '.trc'
result_expansion    = '.pmv'
build_dir_substr    = 'build'
rdo_ex_substr       = 'rdo'
rdo_test_ex_substr  = 'rdo_check_results'

if sys.platform == 'win32':
    rdo_ex_substr      = 'rdo.exe'
    rdo_test_ex_substr = 'rdo_check_results.exe'

RDO_CONSOLE_COMPILE_LOG_FILE_NAME = 'log.txt'
    
# test target type
TARGET_CONSOLE   = 'CONSOLE'
TAGRET_CONVERTER = 'CONVERTOR'
TARGET_GUI       = 'GUI'

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
null_file = 'null_temp_file'
dividing_line = '-------------------------------------------------------------------------------'

###############################################################################
#                                 functions                                   #
###############################################################################

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
    nfile = filter(lambda x: x.endswith(test_expansion), files)
    return nfile


def get_executables(dir):
    # get rdo_studio_console executable and test_rdo_studio_console executable
    files = get_files_list(dir)
    rdo_ex = filter(lambda x: x.endswith(rdo_ex_substr), files)[0]
    rdo_test_ex = filter(lambda x: x.endswith(rdo_test_ex_substr), files)[0]

    return rdo_ex, rdo_test_ex


def get_text_from_dom(dom, node_text):

    node = dom.getElementsByTagName(node_text)
    if not len(node):
        return ''

    nodelist = node[0].childNodes
    rc = []
    for node in nodelist:
        if node.nodeType == node.TEXT_NODE:
            rc.append(node.data)
    return ''.join(rc)

def wrap_the_string_in_quotes(string):
    new_string = '"' + string + '"'
    return new_string
    
###############################################################################
#                                 main code                                   #
###############################################################################

# global exit code variable
G_EXIT_CODE = APP_CODE_TERMINATION_NORMAL

print "STARTED SCRIPT :", sys.argv[0]

# search rdo and rdo_test executables
executables = get_executables(directory)

rdo_ex      = executables[0]
rdo_test_ex = executables[1]

if not os.path.exists(rdo_ex) or not os.path.exists(rdo_test_ex):
    print 'Build app not found. Critical error !!!'
    exit(EXIT_CODE_TERMINATION_ERROR)

# search .rtestx files
files = get_test_files(model_directory)
files.sort()

print '\nDEBUG INFO'

print '\nFind RDO executables    :'
print_list_of_line(executables)

print '\nFind test project files :'
print_list_of_line(files)

# parse xml and start tests
print '\nSTARTED TEST CYCLE\n'

# check model cycle
for task in files:

    print dividing_line
    
    dirname = os.path.dirname(unicode(task, sys.getfilesystemencoding())) + u'/'
    
    dom = xml.dom.minidom.parse(task)

    model_name_with_ex    = get_text_from_dom(dom, 'model')
    target                = get_text_from_dom(dom, 'target')
    text_exit_code        = get_text_from_dom(dom, 'exit_code')
    etalon_trace_name     = get_text_from_dom(dom, 'trace')
    etalon_result_name    = get_text_from_dom(dom, 'result') 
    compile_log_file_name = get_text_from_dom(dom, 'log_compilation')

    exit_code = int(text_exit_code)

    print 'Project              :', task
    print 'Model file           :', model_name_with_ex
    print 'Target               :', target
    print 'Exit code            :', exit_code
    print 'Trace file           :', etalon_trace_name
    print 'Result file          :', etalon_result_name
    print 'Log compilation file :', compile_log_file_name
    print ''
    
    model         = dirname + model_name_with_ex
    etalon_trace  = dirname + etalon_trace_name
    etalon_result = dirname + etalon_result_name

    model_name = model_name_with_ex.partition('.')[0]

    simulation_trace = dirname + model_name + trace_expansion
    simulation_result = dirname + model_name + result_expansion

    cycle_exit_code = APP_CODE_TERMINATION_ERROR

    # select console target
    if target == TARGET_CONSOLE:

        # run rdo_console app on test model
        command = (rdo_ex + ' -i ' + wrap_the_string_in_quotes(model) + ' >> ' + null_file)
        simulation_code = subprocess.call(command, shell=True)
        print "SIMYLATION EXIT CODE :", simulation_code

        # check simulation exit code
        simulation_exit_code_string = 'ERROR'

        cmp_exit_code = simulation_code == exit_code
        if cmp_exit_code:
            simulation_exit_code_string = "OK"

        print "CHECK SIM EXIT CODE  :", simulation_exit_code_string

        # bad simulation exit code
        if not cmp_exit_code:
            cycle_exit_code = APP_CODE_TERMINATION_ERROR

        # nornal simulation check
        elif simulation_code == RDO_CONSOLE_TERMINATION_NORMAL:
        
            command = (rdo_test_ex + ' -T ' + wrap_the_string_in_quotes(etalon_trace) + ' -R ' + wrap_the_string_in_quotes(etalon_result) 
                                   + ' -t ' + wrap_the_string_in_quotes(simulation_trace) + ' -r ' + wrap_the_string_in_quotes(simulation_result)
                                   + ' >> ' + null_file)

            test_code = subprocess.call(command, shell=True)

            check_exit_code_string = 'ERROR UNKNOWN'

            if test_code == RDO_CHECK_RESULT_TERMINATION_NORMAL:
                cycle_exit_code = APP_CODE_TERMINATION_NORMAL
                check_exit_code_string = 'OK'

            elif test_code == RDO_CHECK_RESULT_TERMINATION_ERROR_FILE_NOT_FOUND:
                check_exit_code_string = 'ERROR FILE NOT FOUND'

            elif test_code == RDO_CHECK_RESULT_TERMINATION_ERROR_INVALID_INPUT:
                check_exit_code_string = 'ERROR INVALID INPUT'

            elif test_code == RDO_CHECK_RESULT_TERMINATION_ERROR_RESULT:
                check_exit_code_string = 'ERROR RESULT'

            elif test_code == RDO_CHECK_RESULT_TERMINATION_ERROR_TRACE:
                check_exit_code_string = 'ERROR TRACE'            

            elif test_code == RDO_CHECK_RESULT_TERMINATION_ERROR_RESULT_AND_TRACE:
                check_exit_code_string = 'ERROR RESULT AND TRACE'            

            print "TEST EXIT CODE       :", test_code
            print "CHECK TEST CODE      :", check_exit_code_string

        # .rdox model not found
        elif simulation_code == RDO_CONSOLE_TERMINATION_WITH_AN_ERROR_NO_MODEL:
            cycle_exit_code = APP_CODE_TERMINATION_NORMAL
            
        # check compile error log
        elif simulation_code == RDO_CONSOLE_TERMINATION_WITH_AN_ERROR_PARSE_ERROR:

            simulation_log_file = dirname + RDO_CONSOLE_COMPILE_LOG_FILE_NAME
            simulation_log_file_etalon = dirname + compile_log_file_name

            string_list_log = open(simulation_log_file, 'r').readlines()
            string_list_log_etalon = open(simulation_log_file_etalon, 'r').readlines()

            cut_slash(string_list_log)
            cut_slash(string_list_log_etalon)

            check_message_cmp_string = 'ERROR'
  
            if cmp(string_list_log, string_list_log_etalon) == 0:
                cycle_exit_code = APP_CODE_TERMINATION_NORMAL
                check_message_cmp_string = 'OK'

            print "CHECK ERROR LIST     :", check_message_cmp_string 

        # runtime error in rdo_console
        elif simulation_code == RDO_CONSOLE_TERMINATION_WITH_AN_ERROR_RUNTIME_ERROR:
            cycle_exit_code = APP_CODE_TERMINATION_NORMAL

    else:
        print 'INVALID TARGET'

    # remove temp file
    #os.remove(simulation_trace)
    #os.remove(simulation_result)

    if cycle_exit_code == APP_CODE_TERMINATION_ERROR:
        G_EXIT_CODE = APP_CODE_TERMINATION_ERROR

os.remove(null_file)

print "\n", "PYTHON EXIT CODE :", G_EXIT_CODE, "\n"

sys.exit(G_EXIT_CODE)