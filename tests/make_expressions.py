import os, re

files = os.listdir('expressions')

for f in files:
    if (re.search(r'^.+\.in$', f)):
        n = re.sub(r'\..*$', '.sample', f)
        os.system ('D:/Homework/Compiler/compiler/Debug/compiler.exe D:/Homework/Compiler/tests/expressions/' + f + ' 2 > D:/Homework/Compiler/tests/expressions/' + n)
