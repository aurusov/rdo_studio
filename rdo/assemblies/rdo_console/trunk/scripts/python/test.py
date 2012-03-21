###############################################################################
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
###############################################################################

import os
import xml.sax

directory = '.'
model_directory = '.'
expansion = '.rtestx'
buid_dir_substr = 'build'
rdo_ex_substr = "rdo"
rdo_test_ex_substr = "rdo_test"

def get_test_files(dir):
  # get all
  dirs  = []
  nfile = []
  files = []
  for dirname, dirnames, filenames in os.walk(dir):
    dirs.append(dirname)
    for subdirname in dirnames:
      dirs.append(os.path.join(dirname, subdirname))
    for filename in filenames:
      files.append(os.path.join(dirname, filename))
  nfile = filter(lambda x: x.endswith(expansion), files)
  return nfile

def get_executables(dir):
  # get rdo_studio_console executable and test_rdo_studio_console executable
  rdo_ex = ""
  test_rdo_ex = ""
  
  dirs = os.listdir(dir)
  fdir = ''
  
  for x in dirs:
    if x.find(buid_dir_substr) != -1:
      fdir = "./" + x
      break
  
  if len(fdir) == 0:
    print "Build directory not found. Critical error !!!"
    exit(-1)
  
  rdo_ex      = ""
  rdo_test_ex = ""
  
  rdo_ex = fdir + "/" + rdo_ex_substr
  rdo_test_ex = fdir + "/" + rdo_test_ex_substr
  
  if not os.path.exists(rdo_ex) or not os.path.exists(rdo_test_ex):
    print "Build directory not found. Critical error !!!"
    exit(-1)
  
  return rdo_ex, rdo_test_ex

###############################################################################
#                                 main code                                   #
###############################################################################

files = get_test_files(model_directory)
executables = get_executables(directory)

print 'files : ', files
print 'executables : ', executables