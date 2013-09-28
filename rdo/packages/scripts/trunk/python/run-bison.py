import subprocess,os,argparse,re,sys


def main():
    parser = argparse.ArgumentParser(usage = argparse.SUPPRESS, description = "run bison twice for multipass compiler grammar files")

    parser.add_argument('inputFile', type = str, help = ".yx input file")

    parser.add_argument('-y1', type = str, default = '', help =\
                        "1st output y file", required = True)
    parser.add_argument('-y2', type = str, default = '', help =\
                        "2nd output y file", required = True)
    parser.add_argument('-o1', type = str, default = '', help =\
                        "1st output cpp file", required = True)
    parser.add_argument('-o2', type = str, default = '', help =\
                        "2nd output cpp file", required = True)

    args = parser.parse_args()

    print("Executing bison...")
    print("STEP1")
    cppPath = os.path.abspath(args.o1)
    yPath = os.path.abspath(args.y1)
    yxPath = os.path.abspath(args.inputFile)
        
    procname = "\
set CYGWIN=nodosfilewarning\
&set BISON_PKGDATADIR=" + os.environ["BISON_PKGDATADIR"] + "\
&" + os.environ["BISON_PATH"] + " -g -pdpt \"" + yPath + "\" -o\"" + cppPath + "\""

    output, errr = subprocess.Popen(procname,stdout = subprocess.PIPE, stderr = subprocess.PIPE, shell = True).communicate()

    errr = errr.decode('utf-8')

    errr = errr.replace(yPath,yxPath)
    
    yPath = yPath.replace("\\","\\\\")   # ну это какой-то грязный хак, конечно
    yxPath = yxPath.replace("\\","\\\\")
    
    errr = errr.replace(yPath,yxPath)
    
    out1 = re.sub(r"^([^:]*:)((\\\\?[^:\\]*)*):(([0-9]*).([0-9]*)-[0-9.]*)(.*)$", r"\1\2(\5,\6)\7", errr, flags=re.MULTILINE) # треш, да, а что поделать

    strGRAM = open(cppPath, encoding = 'utf-8').read()

    fin = open(cppPath, 'w', encoding = 'utf-8')
    fin.write( strGRAM.replace(yPath,yxPath) )
    fin.close()

    print("STEP2")
    cppPath = os.path.abspath(args.o2)
    yPath = os.path.abspath(args.y2)
    yxPath = os.path.abspath(args.inputFile)

    procname = "\
set CYGWIN=nodosfilewarning\
&set BISON_PKGDATADIR=" + os.environ["BISON_PKGDATADIR"] + "\
&" + os.environ["BISON_PATH"] + " -g -pdpt \"" + yPath + "\" -o\"" + cppPath + "\""
    
    output, errr = subprocess.Popen(procname,stdout = subprocess.PIPE, stderr= subprocess.PIPE, shell = True).communicate()

    errr = errr.decode('utf-8')

    errr = errr.replace(yPath,yxPath)
    
    yPath = yPath.replace("\\","\\\\")   # ну это какой-то грязный хак, конечно
    yxPath = yxPath.replace("\\","\\\\")
    
    errr = errr.replace(yPath,yxPath)
    
    out2 = re.sub(r"^([^:]*:)((\\\\?[^:\\]*)*):(([0-9]*).([0-9]*)-[0-9.]*)(.*)$", r"\1\2(\5,\6)\7", errr, flags=re.MULTILINE) # треш, да, а что поделать

    strGRAM = open(cppPath, encoding = 'utf-8').read()

    fin = open(cppPath, 'w', encoding = 'utf-8')
    fin.write( strGRAM.replace(yPath,yxPath) )
    fin.close()

    if out1==out2:
        print(out1)
        sys.exit(0)
    else:
        sys.exit(yxPath+": error : bison console output doesn't match for grammar files")

toolname = "split-bison"
codepage = "utf-8"

if __name__ == '__main__':
    main()
