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

push offset __str@0
call crt_printf
; write_statement


exit
end start
