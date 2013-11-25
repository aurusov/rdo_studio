import sys

enclist = sys.argv[1:]

testPhrase = \
"""\
========================
  это успех, например
  qqq 123 !@#$%^&*()_+
========================
"""

print(testPhrase)

failPhrase = \
"""\
========================
        LOL NOPE!
========================
"""

for left in enclist:
    for right in enclist:
        if left != right:
            print("========================")
            print(" TRYING ", left, ">", right)
            try:
                print(testPhrase.encode(left).decode(right))
            except Exception as ex: 
                print(failPhrase)

