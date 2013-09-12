import sys, argparse, re

def divide(expr):
    expr = expr.split('\n')

    f1 = ""
    f2 = ""

    use1 = use2 = last = True
    brackets = i = comment = curbr = 0
    brOmitted = False

    comstep = ""

    lnre = re.compile(r"^(\s*[\|\:].*)$")

    for ln in expr:
        if (i == 0) and not lnre.match(ln):
            sys.exit("faulty token block")
        else:
            if lnre.match(ln):
                if not last:
                    last = True
            else:
                last = False
                if ln.count("#PASS1") > 0:
                    use2 = False
                    curbr = brackets
                    comstep = ln[:ln.find("#PASS1")]
                    ln = ln.replace("#PASS1", "/* ---------- COMPILER 1st PASS ---------- */", 1)
                if ln.count("#PASS2") > 0:
                    use1 = False
                    curbr = brackets
                    comstep = ln[:ln.find("#PASS1")]
                    ln = ln.replace("#PASS2", "/* ---------- COMPILER 2nd PASS ---------- */", 1)
                if ln.count("{") > 0:
                    brackets += ln.count("{")
                    if (not brOmitted and brackets >= curbr + 1) and ((not use1) or (not use2)):
                        brOmitted = True
                        ln = ln.replace("{", "", 1)
                        ln = re.sub(r"^\s*$", r"", ln)
                if ln.count("}") > 0:
                    brackets -= ln.count("}")
                    if (brackets <= curbr) and ((not use1) or (not use2)):
                        comment = 2*use1 + use2
                        use1 = True
                        use2 = True
                        brOmitted = False
                        # some black right-replace magick
                        ln = ln[::-1].replace("}"[::-1], (("\n" + comstep)*((ln.count("}") - 1) and True) + "/* --------------------------------------- */")[::-1], 1)[::-1]
            f1 += (ln + "\n" * ((len(expr) - i - 1) and True)) * ((len(ln) or not ((brackets == curbr + 1) and ((not use1) or (not use2)))) and (comment - 1) and use1)
            f2 += (ln + "\n" * ((len(expr) - i - 1) and True)) * ((len(ln) or not ((brackets == curbr + 1) and ((not use1) or (not use2)))) and (comment - 2) and use2)
            comment = 0
        i += 1

    f1 = re.sub(r'(\s*[\|\:].*)\n\s*{\s*}(.*)', r'\1\2', f1)
    f2 = re.sub(r'(\s*[\|\:].*)\n\s*{\s*}(.*)', r'\1\2', f2)

    return [f1,f2]


def main():
    parser = argparse.ArgumentParser(usage = argparse.SUPPRESS, description ="desc")

    parser.add_argument('inputFile', type = str, help = ".yx input file")
    parser.add_argument('-o1', type = str, default = '', help =\
                        "1st output file", required = True)
    parser.add_argument('-o2', type = str, default = '', help =\
                        "2nd output file", required = True)

    args = parser.parse_args()

    inf = open(args.inputFile,"r", encoding = codepage)
    print(toolname + ": parsing " + args.inputFile)
        
    out1 = open(args.o1, 'w', encoding = codepage)
    out1.truncate()
    
    out2 = open(args.o2, 'w', encoding = codepage)
    out2.truncate()

    blck = ""
    flag = brackets = 0
    curline = 1
    while True:
        sym = inf.read(1)
        
        if sym == ':':
            flag = 1

        if sym == '\n':
            curline += 1
            
        if sym == '{':
            brackets += 1
        if sym == '}':
            brackets -= 1
            if brackets < 0:
                sys.exit("error: braces nesting mismatch in line " + str(curline) + ". check source file")
            
        if sym == ';' and brackets == 0:
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
    print(toolname + ": generated " + args.o1 + ", " + args.o2)
    sys.exit(0)

# some parameters
toolname = "split-bison"
codepage = "utf-8"

if __name__ == '__main__':
    main()
