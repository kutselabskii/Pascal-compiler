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

push 12
; pushing int constant value

lea eax, dword ptr [ebp - 0]
push eax
; pushing variable address

pop eax
pop ebx
mov dword ptr[eax], ebx
; assignment_statement

push 4
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

mov ecx, eax
mov eax, ebx
mov ebx, ecx
cdq
idiv ebx
push eax
; integer division

push offset __str@0
call crt_printf
; write_statement

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

mov ecx, eax
mov eax, ebx
mov ebx, ecx
cdq
idiv ebx
push edx
; remainder

push offset __str@0
call crt_printf
; write_statement


exit
end start
