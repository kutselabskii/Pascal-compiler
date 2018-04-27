include d:\masm32\include\masm32rt.inc

.xmm
.const
__str@0 db 37, 100, 10, 0
.code
start:

push ebp
mov ebp, esp
sub esp, 8
; memory_allocation

push 3
; pushing int constant value

lea eax, dword ptr [ebp - 0]
push eax
; pushing variable address

pop eax
pop ebx
mov dword ptr[eax], ebx
; assignment_statement

push 2
; pushing int constant value

lea eax, dword ptr [ebp - 4]
push eax
; pushing variable address

pop eax
pop ebx
mov dword ptr[eax], ebx
; assignment_statement

lea eax, dword ptr [ebp - 0]
push eax
; pushing variable address

lea eax, dword ptr [ebp - 4]
push eax
; pushing variable address

pop eax
mov eax, dword ptr [eax]
; getting left child value

pop ebx
mov ebx, dword ptr [ebx]
; getting right child value

add eax, ebx
push eax
; addition

push offset __str@0
call crt_printf
; write_statement


exit
end start
