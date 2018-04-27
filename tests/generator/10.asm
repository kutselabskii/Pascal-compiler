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

push 1
; pushing int constant value

lea eax, dword ptr [ebp - 4]
push eax
; pushing variable address

pop eax
pop ebx
mov dword ptr[eax], ebx
; assignment_statement

push 5
; pushing int constant value

lea eax, dword ptr [ebp - 4]
push eax
; pushing variable address

push 7
; pushing int constant value

pop eax
; getting left child value

pop ebx
mov ebx, dword ptr [ebx]
; getting right child value

add eax, ebx
push eax
; addition

push 4
; pushing int constant value

pop eax
; getting left child value

pop ebx
; getting right child value

mov ecx, eax
mov eax, ebx
mov ebx, ecx
cdq
idiv ebx
push eax
; integer division

pop eax
; getting left child value

pop ebx
; getting right child value

add eax, ebx
push eax
; addition

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
