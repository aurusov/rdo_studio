import subprocess, os, argparse, re, sys

def run_bison(yxPath, yPath, cppPath, name):
    if sys.platform == 'win32':
        procname = "set CYGWIN=nodosfilewarning& set BISON_PKGDATADIR=" + os.environ["BISON_PKGDATADIR"] + "&" + os.environ["BISON_PATH"] + " -p" + name + " \"" + yPath + "\" -o\"" + cppPath + "\""
    else:
        procname = "set BISON_PKGDATADIR=" + os.environ["BISON_PKGDATADIR"] + ";" + os.environ["BISON_PATH"] + " -p" + name + "\"" + yPath + "\" -o\"" + cppPath + "\""

    output, errr = subprocess.Popen(procname,stdout = subprocess.PIPE, stderr = subprocess.PIPE, shell = True).communicate()

    errr = errr.decode(codepage)

    errr = errr.replace(yPath,yxPath)
    
    if sys.platform == 'win32':
        yPath = yPath.replace("\\","\\\\")   # ну это какой-то грязный хак, конечно
        yxPath = yxPath.replace("\\","\\\\")
        errr = errr.replace(yPath,yxPath)

    if not errr.count("error"):
        strGRAM = open(cppPath, encoding = codepage).read()
        fin = open(cppPath, 'w', encoding = codepage)
        fin.write( strGRAM.replace(yPath,yxPath) )
        fin.close()

    return re.sub(r"^([^:]*:)((\\\\?[^:\\]*)*):(([0-9]*).([0-9]*)-[0-9.]*)(.*)$", r"\1\2(\5)\7", errr, flags=re.MULTILINE)

def main():
    parser = argparse.ArgumentParser(usage = argparse.SUPPRESS, description = "run bison twice for multipass compiler grammar files")

    parser.add_argument('inputFile', type = str, help = ".yx input file")

    parser.add_argument('-y1', type = str, default = '', help = "1st output y file",   required = True)
    parser.add_argument('-y2', type = str, default = '', help = "2nd output y file",   required = True)
    parser.add_argument('-n1', type = str, default = '', help = "1st output y name",   required = True)
    parser.add_argument('-n2', type = str, default = '', help = "2nd output y name",   required = True)
    parser.add_argument('-o1', type = str, default = '', help = "1st output cpp file", required = True)
    parser.add_argument('-o2', type = str, default = '', help = "2nd output cpp file", required = True)

    args = parser.parse_args()

    print(toolname + ": " + "Executing bison...")
    cppPath = os.path.abspath(args.o1)
    if os.path.exists(cppPath):
        os.remove(cppPath)
    yPath   = os.path.abspath(args.y1)
    yxPath  = os.path.abspath(args.inputFile)
    print(toolname + ": " + "parsing " + yPath)
    
    out1 = run_bison(yxPath, yPath, cppPath, args.n1)

    cppPath = os.path.abspath(args.o2)
    if os.path.exists(cppPath):
        os.remove(cppPath)
    yPath   = os.path.abspath(args.y2)
    yxPath  = os.path.abspath(args.inputFile)
    print(toolname + ": " + "parsing " + yPath)

    out2 = run_bison(yxPath, yPath, cppPath, args.n2)

    if out1 == out2:
        print(toolname + ": " + "bison output:")
        con = out1.find(": conflicts:")
        if con > -1:

            file_conflict = open(cppPath.replace(".cpp", ".output"), 'r', encoding = codepage)

            conflicts = file_conflict.read()
            conf_pos = conflicts[:conflicts.find("State ")].count("\n") + 1
            
            out1 = out1.replace(": conflicts:", "(" + str(conf_pos) + ") : warning C0000:")
            conend = out1[:con].rfind("\n")
            if conend == -1:
                conend = 0
            out1 = out1[:conend] + out1[conend:con].replace(yxPath, cppPath.replace(".cpp", ".output"), 1) + out1[con:]

        out1 = out1.replace(": error:"," : error C0000:")
        out1 = out1.replace("\\\\","\\")
        print(out1)
        sys.exit(0)
    else:
        sys.exit(yxPath + ": error : bison console output doesn't match for grammar files")

toolname = "run-bison"
codepage = "utf-8"

if __name__ == '__main__':
    main()
