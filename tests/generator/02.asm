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

push 10
; pushing int constant value

lea eax, dword ptr [ebp - 0]
push eax
; pushing variable address

pop eax
pop ebx
mov dword ptr[eax], ebx
; assignment_statement

lea eax, dword ptr [ebp - 0]
push eax
; pushing variable address

pop eax
push dword ptr [eax]
push offset __str@0
call crt_printf
; write_statement


exit
end start
