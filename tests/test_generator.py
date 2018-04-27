import os, re

files = os.listdir('generator')

for f in files:
    if (re.search(r'^.+\.exe$', f)):
        n = re.sub(r'\..*$', '.out', f)
        os.system ('D:/Homework/Compiler/tests/generator/' + f + ' > ' + 'D:/Homework/Compiler/tests/generator/' + n)

files1 = os.listdir('generator')

i = 0
j = 0
for f in files1:
    if re.search(r'^.+\.out$', f):
        with open('generator/' + f, 'r') as s1:
            n = re.sub(r'\..*$', '.sample', f)
            with open('generator/' + n, 'r') as s2:
                i += 1
                if (s1.readlines() != s2.readlines()):
                    j += 1
                    print ('Check file ' + f)

print (str(i - j) + ' / ' + str(i) + ' tests passed')

input ('Press a key to continue\n')
