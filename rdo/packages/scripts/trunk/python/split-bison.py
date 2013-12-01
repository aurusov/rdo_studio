#!python3

import sys, re, os, argparse, subprocess

toolname = "split-bison"
codepage = "utf-8"

def divide(expr):

    f1 = expr
    f2 = expr

    # isle PASS1 blocks if file 1
    while f1.count("#PASS1") > 0:
        pos = f1.find("{", f1.find("#PASS1"))
        brackets = 1
        posend = pos
        while brackets > 0:
            posend += 1
            if f1[posend] == '{':
                brackets += 1
            if f1[posend] == '}':
                brackets -= 1
        f1 = f1[: posend] +       "/* -------------- END PASS 1 ------------- */" + f1[posend + 1 :]
        f1 = f1[: pos] + f1[pos + 1 :]
        f1 = f1.replace("#PASS1", "/* ---------- COMPILER 1st PASS ---------- */", 1)
 
    # clean PASS2 blocks from file 1
    while f1.count("#PASS2") > 0:
        Ppos = f1.find("#PASS2")
        pos = f1.find("{", Ppos)
        brackets = 1
        posend = pos
        while brackets > 0:
            posend += 1
            if f1[posend] == '{':
                brackets += 1
            if f1[posend] == '}':
                brackets -= 1
        cut = f1[Ppos + 1 : posend]
        f1 = f1[: Ppos] + "\n" * cut.count("\n") + f1[posend + 1 :]

    # isle PASS2 blocks if file 2
    while f2.count("#PASS2") > 0:
        pos = f2.find("{", f2.find("#PASS2"))
        brackets = 1
        posend = pos
        while brackets > 0:
            posend += 1
            if f2[posend] == '{':
                brackets += 1
            if f2[posend] == '}':
                brackets -= 1
        f2 = f2[: posend] +       "/* -------------- END PASS 2 ------------- */" + f2[posend + 1 :]
        f2 = f2[: pos] + f2[pos + 1 :]
        f2 = f2.replace("#PASS2", "/* ---------- COMPILER 2st PASS ---------- */", 1)

    # clean PASS1 blocks from file 2
    while f2.count("#PASS1") > 0:
        Ppos = f2.find("#PASS1")
        pos = f2.find("{", Ppos)
        brackets = 1
        posend = pos
        while brackets > 0:
            posend += 1
            if f2[posend] == '{':
                brackets += 1
            if f2[posend] == '}':
                brackets -= 1
        cut = f2[Ppos + 1 : posend]
        f2 = f2[: Ppos] + "\n" * cut.count("\n") + f2[posend + 1 :]

    return [f1,f2]


def main():
    parser = argparse.ArgumentParser(usage = argparse.SUPPRESS, description ="split multipass compiler yx file into a pair of bison grammar files")

    parser.add_argument('inputFile', type = str, help = ".yx input file")
    parser.add_argument('-y1', type = str, default = '', help =\
                        "1st output y file", required = True)
    parser.add_argument('-y2', type = str, default = '', help =\
                        "2nd output y file", required = True)
    
    args = parser.parse_args()

    inf = open(args.inputFile,"r", encoding = codepage)
    print(toolname + ": parsing " + args.inputFile)
        
    out1 = open(args.y1, 'w', encoding = codepage)
    out1.truncate()
    
    out2 = open(args.y2, 'w', encoding = codepage)
    out2.truncate()

    blck = ""
    flag = brackets = 0
    curline = 1
    while True:
        sym = inf.read(1)
        symread = True
        
        if sym == ':' and brackets == 0:
            flag = 1
            symread = False

        if sym == '\n':
            curline += 1
            
        if sym == '{':
            brackets += 1
        if sym == '}':
            brackets -= 1
            if brackets < 0:
                sys.exit(args.inputFile + "(" + str(curline) + "): error: braces nesting mismatch")
            
        if sym == ':' and brackets == 0 and flag == 1 and symread or not sym:
            flag = 0
            blck = divide(blck)
            out1.write(blck[0])
            out2.write(blck[1])
            blck = ""
            
        if flag == 0:
            out1.write(sym)
            out2.write(sym)
        else:
            blck += sym

        if not sym:
            break
    print(toolname + ": generated " + args.y1 + ", " + args.y2)

    sys.exit(0)

if __name__ == '__main__':
    main()
