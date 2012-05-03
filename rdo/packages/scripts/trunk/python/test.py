###############################################################################
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
###############################################################################

import os
import sys
import xml.dom.minidom

###############################################################################
#                                 constant                                    #
###############################################################################

directory           = '.'
model_directory     = '.'
test_expansion      = '.rtestx'
trace_expansion     = '.trc'
result_expansion    = '.pmv'
build_dir_substr    = 'build'
rdo_ex_substr       = 'rdo'
rdo_test_ex_substr  = 'rdo_test'

null_file = 'null_temp_file'

if sys.platform == 'win32':
    rdo_ex_substr      = 'rdo.exe'
    rdo_test_ex_substr = 'rdo_test.exe'

dividing_line = '-------------------------------------------------------------------------------'

TARGET_CONSOLE   = 'CONSOLE'
TAGRET_CONVERTER = 'CONVERTOR'
TARGET_GUI       = 'GUI'

EXIT_CODE_TERMINATION_NORMAL = 0
EXIT_CODE_TERMINATION_ERROR  = 1

# global exit code variable
G_EXIT_CODE = EXIT_CODE_TERMINATION_NORMAL

###############################################################################
#                                 functions                                   #
###############################################################################

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


def get_text_from_node_list(nodelist):
    rc = []
    for node in nodelist:
        if node.nodeType == node.TEXT_NODE:
            rc.append(node.data)
    return ''.join(rc)

###############################################################################
#                                 main code                                   #
###############################################################################

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

print '\nDEBUG INFO\n'

print 'Find RDO executables    : ', executables, '\n'
print 'Find test project files : ', files, '\n'

# parse xml and start tests
print 'STARTED TEST CYCLE'

for task in files:
    print dividing_line
  
    dom = xml.dom.minidom.parse(task)
  
    dirname = os.path.dirname(task) + '/'
  
    model_name_with_ex = get_text_from_node_list(dom.getElementsByTagName('model')[0].childNodes)
  
    target = get_text_from_node_list(dom.getElementsByTagName('target')[0].childNodes)
    exit_code = get_text_from_node_list(dom.getElementsByTagName('exit_code')[0].childNodes)
    etalon_trace_name = get_text_from_node_list(dom.getElementsByTagName('trace')[0].childNodes)
    etalon_result_name =  get_text_from_node_list(dom.getElementsByTagName('result')[0].childNodes)
  
    print 'Project     :', task
    print 'Model file  :', model_name_with_ex
    print 'Target      :', target
    print 'Exit code   :', exit_code
    print 'Trace file  :', etalon_trace_name
    print 'Result file :', etalon_result_name
    print ''
  
    model = dirname + model_name_with_ex
    etalon_trace = dirname + etalon_trace_name
    etalon_result =  dirname + etalon_result_name

    model_name = model_name_with_ex.partition('.')[0]

    simulation_trace = dirname + model_name + trace_expansion
    simulation_result = dirname + model_name + result_expansion

    if target == TARGET_CONSOLE:
        simulation_code = os.system(rdo_ex + ' -i ' + model 
                                           + ' >> ' + null_file)
        
        print "SIMYLATION EXIT CODE :", simulation_code
        
        simulation_exit_code_string = ''
        if cmp(simulation_code, exit_code):
            simulation_exit_code_string = "OK"
        else:
            simulation_exit_code_string = "ERROR"
          
        print "CHECK SIM EXIT CODE  :", simulation_exit_code_string
        
        check_exit_code_string = ''
        if simulation_code == EXIT_CODE_TERMINATION_NORMAL:
            test_code = os.system(rdo_test_ex + ' -T ' + etalon_trace 
                                              + ' -R ' + etalon_result 
                                              + ' -t ' + simulation_trace 
                                              + ' -r ' + simulation_result 
                                              + ' >> ' + null_file)

            if not test_code == EXIT_CODE_TERMINATION_NORMAL:
                G_EXIT_CODE = EXIT_CODE_TERMINATION_ERROR
                check_exit_code_string = 'ERROR'
            else:
                check_exit_code_string =  'OK'

        print "TEST EXIT CODE       :", test_code
        print "CHECK TEST CODE      :", check_exit_code_string

    else:
        print 'INVALID TARGET'

    # remove temp file
    #os.remove(simulation_trace)
    #os.remove(simulation_result)

print

os.remove(null_file)

sys.exit(G_EXIT_CODE)