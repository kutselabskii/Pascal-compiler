import os, re

files = os.listdir('semantic')


for f in files:
    if (re.search(r'^.+\.in$', f)):
        n = re.sub(r'\..*$', '.sample', f)
        os.system ('D:/Homework/Compiler/compiler/Debug/compiler.exe D:/Homework/Compiler/tests/semantic/' + f + ' 3 > D:/Homework/Compiler/tests/semantic/' + n)
