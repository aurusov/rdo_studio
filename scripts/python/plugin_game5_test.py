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

	for file in files:
		if model_path != file:	
			utils.enc_print(file)
			os.remove(file)
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

rdo_ex = get_executables(app_directory)

if not os.path.exists(rdo_ex):
	utils.enc_print ('Critical error! Build app not found.')
	sys.exit(APP_CODE_TERMINATION_ERROR)
	
if not os.path.exists(model_path):
	utils.enc_print ('Critical error! Test nodel not found.')
	sys.exit(APP_CODE_TERMINATION_ERROR)
	
utils.enc_print ('\nFind RDO executable:')
utils.enc_print (rdo_ex)

utils.enc_print ('\nFind test project file:')
utils.enc_print (model_path)

delete_model_data(model_path)

plugin_arg = '1 0 2 4 5 3'
command = (rdo_ex + ' -platform minimal --plugin0=' + utils.wrap_the_string_in_quotes(plugin_arg) + ' -i ' + utils.wrap_the_string_in_quotes(model_path))
utils.enc_print('Run: ', command)
simulation_code = subprocess.call(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
utils.enc_print ('EXIT CODE: ', simulation_code)

utils.enc_print ('PYTHON EXIT CODE :', simulation_code)
sys.exit(simulation_code)
