#!python3

import subprocess, os, argparse, re, sys, difflib

def run_bison(yxPath, yPath, cppPath, name, bison, defines):
    procname = os.path.abspath(bison)+ (" --defines=\"" + defines + "\" ")*(len(defines) and True) + " -p" + name + " \"" + yPath + "\" -o\"" + cppPath + "\""
    
    output, errr = subprocess.Popen(procname,stdout = subprocess.PIPE, stderr = subprocess.PIPE, shell = True).communicate()

    errr = errr.decode(sys.stdout.encoding, 'ignore')
    
    errr = errr.replace(yPath,yxPath)
    
    if sys.platform == 'win32':
        yPath = yPath.replace("\\","\\\\")
        yxPath = yxPath.replace("\\","\\\\")
        errr = errr.replace(yPath,yxPath)

    if not errr.count("error"):
        if os.path.exists(cppPath):
            strGRAM = open(cppPath, encoding = codepage).read()
            fin = open(cppPath, 'w', encoding = codepage)
            fin.write( strGRAM.replace(yPath,yxPath) )
            fin.close()

    if sys.platform == 'win32': 
        return re.sub(r"^(.*)(\.yx)(:)([0-9]*)\.([0-9]*)(-[0-9]*(\.[0-9]*)?)?(:.*)$", r"\1\2(\4)\8",    errr, flags=re.MULTILINE)
    else:
        return re.sub(r"^(.*)(\.yx)(:)([0-9]*)\.([0-9]*)(-[0-9]*(\.[0-9]*)?)?(:.*)$", r"\1\2\3\4:\5\8", errr, flags=re.MULTILINE)

def main():
    parser = argparse.ArgumentParser(usage = argparse.SUPPRESS, description = "run bison twice for multipass compiler grammar files")

    parser.add_argument('inputFile', type = str, help = ".yx input file")

    parser.add_argument('-y1', type = str, default = '', help = "1st output y file",   required = True)
    parser.add_argument('-y2', type = str, default = '', help = "2nd output y file",   required = True)
    parser.add_argument('-n1', type = str, default = '', help = "1st output y name",   required = True)
    parser.add_argument('-n2', type = str, default = '', help = "2nd output y name",   required = True)
    parser.add_argument('-o1', type = str, default = '', help = "1st output cpp file", required = True)
    parser.add_argument('-o2', type = str, default = '', help = "2nd output cpp file", required = True)
    parser.add_argument('-bison', type = str, default = '', help = "bison path", required = True)
    parser.add_argument('-defines', type = str, default = '', help = "bison defines")

    args = parser.parse_args()

    defines = os.path.abspath(args.defines)

    print(toolname + ": " + "Executing bison...", file=sys.stderr)

    if len(defines) > 0:
        print(toolname + ": " + "defines path: " +defines, file=sys.stderr)

    cppPath = os.path.abspath(args.o1)
    if os.path.exists(cppPath):
        os.remove(cppPath)
    yPath   = os.path.abspath(args.y1)
    yxPath  = os.path.abspath(args.inputFile)
    print(toolname + ": " + "parsing " + yPath, file=sys.stderr)

    out1 = run_bison(yxPath, yPath, cppPath, args.n1, args.bison, defines)

    cppPath = os.path.abspath(args.o2)
    if os.path.exists(cppPath):
        os.remove(cppPath)
    yPath   = os.path.abspath(args.y2)
    yxPath  = os.path.abspath(args.inputFile)
    print(toolname + ": " + "parsing " + yPath, file=sys.stderr)

    out2 = run_bison(yxPath, yPath, cppPath, args.n2, args.bison, defines)

    if out1 == out2:
        print(toolname + ": " + "bison output:", file=sys.stderr)
        con = out1.find(": conflicts:")
        if con > -1:

            file_conflict = open(cppPath.replace(".cpp", ".output"), 'r', encoding = codepage)

            conflicts = file_conflict.read()
            conf_pos = conflicts[:conflicts.find("State ")].count("\n") + 1
            
            if sys.platform == 'win32':
                out1 = out1.replace(": conflicts:", "(" + str(conf_pos) + ") : warning C0000:")
            else:
                out1 = out1.replace(": conflicts:", ":" + str(conf_pos) + ": warning:")
            conend = out1[:con].rfind("\n")
            if conend == -1:
                conend = 0
            out1 = out1[:conend] + out1[conend:con].replace(yxPath, cppPath.replace(".cpp", ".output"), 1) + out1[con:]

        if sys.platform == 'win32':
            out1 = out1.replace(": error:"," : error C0000:")
            out1 = out1.replace(": warning:"," : warning C0000:")
            out1 = out1.replace("\\\\","\\")
        print(out1, file=sys.stderr)
        sys.exit(0)
    else:
        text1_lines = out1.split('\n')
        text2_lines = out2.split('\n')

        diff1 = difflib.unified_diff(text1_lines, text2_lines, lineterm='')

        print("Difference in bison output:")
        for d in list(diff1):
            if (d[0] == '+' or d[0] == '-') and d[1] != '+' and d[1] != '-':
                print(d[0] + '\n' + d[1:])
            else:
                print(d)

        sys.exit(yxPath + ": error : bison console output doesn't match for grammar files")

toolname = "run-bison"
codepage = "utf-8"

if __name__ == '__main__':
    main()
