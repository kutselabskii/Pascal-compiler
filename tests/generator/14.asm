include d:\masm32\include\masm32rt.inc

.xmm
.const
__str@0 db 37, 100, 10, 0
.code
start:

push ebp
mov ebp, esp
sub esp, 0
; memory_allocation

push 1
; pushing int constant value

push 2
; pushing int constant value

pop eax
; getting left child value

pop ebx
; getting right child value

cmp eax, ebx
setne al
movsx eax, al
push eax
; operation not_equal

push offset __str@0
call crt_printf
; write_statement


exit
end start
