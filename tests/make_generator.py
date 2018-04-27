import os, re

files = os.listdir('generator')


for f in files:
    if (re.search(r'^.+\.in$', f)):
        n = re.sub(r'\..*$', '.asm', f)
        os.system ('D:/Homework/Compiler/compiler/Debug/compiler.exe D:/Homework/Compiler/tests/generator/' + f + ' 4')
