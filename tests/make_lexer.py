import os, re

files = os.listdir('lexer')

for f in files:
    if (re.search(r'^.+\.in$', f)):
        n = re.sub(r'\..*$', '.sample', f)
        os.system ('D:/Homework/Compiler/compiler/Debug/compiler.exe D:/Homework/Compiler/tests/lexer/' + f + ' 1 > D:/Homework/Compiler/tests/lexer/' + n)
