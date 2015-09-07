#!/usr/bin/env python3
# -*- coding: utf-8 -*-

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

# data about rdo apps
app_directory      = '.'
model_path         = '.\plugin_test.rdox'
build_dir_substr   = 'build'
rdo_ex_substr      = 'rdo_studio'
test_expansion     = '.rtestx'
project_expansion  = '.rdox'

if sys.platform == 'win32':
	rdo_ex_substr += '.exe'

# python script exit code
APP_CODE_TERMINATION_NORMAL = 0
APP_CODE_TERMINATION_ERROR  = 1	
	
###############################################################################

def get_executables(dir):
	files = utils.get_files_list(dir)

	rdo_ex = None
	for file in files:
		res = file.endswith(rdo_ex_substr)
		if res == True:
			rdo_ex = file
		if rdo_ex:
		   break

	return rdo_ex

###############################################################################

def delete_model_data(model_path):
	dir = os.path.dirname(model_path)
	files = utils.get_files_list(dir)
	utils.enc_print ('\nFound and deleted model data:')

	deleted = 0	
	for file in os.listdir(dir):
		file_path = os.path.join(dir, file)
		if (os.path.isfile(file_path)):
			if (not file_path.endswith(test_expansion) and not file_path.endswith(project_expansion)):
				utils.enc_print(file_path)
				os.remove(file_path)
				deleted = deleted + 1
	
	if deleted == 0:
		utils.enc_print('nothing deleted')
		
	utils.enc_print('\n')

###############################################################################

###############################################################################
#                                 main code                                   #
###############################################################################

# parse console options
parser = argparse.ArgumentParser(description = 'rdo executor of plugin tests')
parser.add_argument('-ad', action = 'store', dest = 'app_directory',   default = app_directory,   help = 'application directory')
parser.add_argument('-mp', action = 'store', dest = 'model_path', default = model_path, help = 'model directory')

args = parser.parse_args()

app_directory   = '' + args.app_directory
model_path = '' + args.model_path

utils.enc_print("App directory:", os.path.abspath(app_directory))
utils.enc_print("Test project file path:", os.path.abspath(model_path))

rdo_ex = get_executables(app_directory)

if rdo_ex is None:
	utils.enc_print ('Critical error! Build app not found.')
	sys.exit(APP_CODE_TERMINATION_ERROR)
	
if not os.path.exists(model_path):
	utils.enc_print ('Critical error! Test nodel not found.')
	sys.exit(APP_CODE_TERMINATION_ERROR)
	
utils.enc_print ('\nFind RDO executable:')
utils.enc_print (rdo_ex)

delete_model_data(model_path)

plugin_arg = '1 0 2 4 5 3'
command = (rdo_ex + ' -platform minimal --plugin0=' + utils.wrap_the_string_in_quotes(plugin_arg) + ' -i ' + model_path)
utils.enc_print('Run: ', command)
simulation_code = subprocess.call(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
utils.enc_print ('EXIT CODE: ', simulation_code)

utils.enc_print ('PYTHON EXIT CODE :', simulation_code)
sys.exit(simulation_code)
