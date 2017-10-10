        global _asm_gcd
_asm_gcd:
        ; entry
        push rbp            ; store ebp
        mov rbp, rsp        ; new stackframe

        ; body
        mov rax, rcx        ; rax = a
        mov r8, rdx         ; r8  = b
; if (b == 0) 
        cmp rdx, 0
        je .done ; b == 0
        ; else ( b != 0 )
        cqo                 ; convert rax to 128bit in rdx:rax
        idiv r8             ; rdx = a % b  & rax = a/b
        mov rcx, r8         ; rcx = b 
        call _asm_gcd       ; rax = gcd ( b, a%b ) = gcd ( rcx, rdx )
.done:
        ; return
        pop rbp             ; restore ebp
        ret 0               ; return from function (0 is default)
