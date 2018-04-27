include d:\masm32\include\masm32rt.inc

.xmm
.const
__str@0 db 37, 100, 10, 0
.code
start:

push ebp
mov ebp, esp
sub esp, 4
; memory_allocation

push 5
; pushing int constant value

lea eax, dword ptr [ebp - 0]
push eax
; pushing variable address

pop eax
pop ebx
mov dword ptr[eax], ebx
; assignment_statement


exit
end start
