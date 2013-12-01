#!python3

import difflib, argparse, subprocess, sys

parser = argparse.ArgumentParser(usage = argparse.SUPPRESS, description ="split-bison test script")

parser.add_argument('inputFile', type = str, help = ".yx input file")
parser.add_argument('-y1', type = str, default = '', help =\
                        "1st output y file", required = True)
parser.add_argument('-y2', type = str, default = '', help =\
                        "2nd output y file", required = True)
parser.add_argument('-ref1', type = str, default = '', help =\
                        "2nd reference y file", required = True)
parser.add_argument('-ref2', type = str, default = '', help =\
                        "2nd reference y file", required = True)
parser.add_argument('-exec', type = str, default = '', help =\
                        "command to execute split-bison", required = True)
    
args = parser.parse_args()

procname = args.exec + " -y1 " + args.y1 + " -y2 " + args.y2 + " " + args.inputFile

out, err = subprocess.Popen(procname, stdout = subprocess.PIPE, shell = True).communicate()

out = out.decode('utf-8')

print(out.replace("split-bison:", "split-test:") + "split-test: reference files: " + args.ref1 + ", " + args.ref2)

inp = open(args.y1, 'r', encoding = 'utf-8')
text1_lines = inp.read().split('\n')
inp = open(args.ref1, 'r', encoding = 'utf-8')
text2_lines = inp.read().split('\n')

inp = open(args.y2, 'r', encoding = 'utf-8')
text3_lines = inp.read().split('\n')
inp = open(args.ref2, 'r', encoding = 'utf-8')
text4_lines = inp.read().split('\n')

diff1 = difflib.unified_diff(text1_lines, text2_lines, lineterm='')
diff2 = difflib.unified_diff(text3_lines, text4_lines, lineterm='')

if len(list(diff1)) + len(list(diff2)) > 0:
    print("split-test: error: generated files don't match reference ones")
    print("split-test: Diff1:")
    print('\n'.list(diff1))
    print("split-test: Diff2:")
    print('\n'.list(diff2))
    sys.exit(1)
else:
    print("split-test: TEST PASS")
    sys.exit(0)
