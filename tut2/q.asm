        global _asm_q
_asm_q:
        ; entry
        push rbp        ; store ebp
        mov rbp, rsp    ; new stackframe
        sub rsp, 40     ; allocate shadow space

        ; body
        add rax, rdi    ; sum = a
        add rax, rdi    ; + b
        add rax, rsi    ; + c
        add rax, rdx    ; + d
        add rax, rcx    ; fuck
        mov rax, rdi ; rax = a
        mov r8, rsi ; r8  = b
; if (b == 0) 
        cmp rsi, 0
        je .done ; b == 0
        ; else ( b != 0 )
        cqo         ; convert rax to 128bit in rdx:rax
        idiv r8    ; rdx = a % b  & rax = a/b
        mov rsi, rdx ; rsi = a % b
        mov rdi, r8     ; rdi = b 
        call _asm_q    ; rax = gcd ( b, a%b ) = gcd ( rcx, rdx )
.done:
        ; return
        add rsp, 40     ; free shadow space
        pop rbp ; restore ebp
        ret 0 ; return from function (0 is default)
