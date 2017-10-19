extern printf
; extern _format

        SECTION .data
msg:        db "Hello World", 0
_format:    db "a = %lld b = %lld c = %lld d = %lld e = %lld sum = %lld", 10, 0
; _format db "a = %lld", 0AH, 00H
; _format db "a = %lld b = %lld c = %lld d = %lld e = %lld sum = %lld\n", 0AH, 00H
        SECTION .txt
        global _asm_q
_asm_q:
        ; entry
        push rbp        ; store ebp
        mov rbp, rsp    ; new stackframe
        sub rsp, 56     ; allocate shadow space

        ; body
        mov rbx, rcx            ; sum = a 16
        add rbx, rdx            ; + b 24
        add rbx, r8             ; + c 32 
        add rbx, r9             ; + d 40
        add rbx, [rbp + 48]     ; + e 48

        lea rdi, [rel _format]
        mov rsi, rcx 
        mov rdx, rdx
        mov rcx, r8
        mov r8, r9
        mov r9, [rbp + 48]
        mov [rsp], rbx
        mov rax, 0
        call printf

;         ;call printf as ms
;         mov r10, [rsp]
;         mov [rsp], rdi
;         mov [rsp + 8], rsi
;         mov [rsp + 16], rdx
;         mov [rsp + 24], rcx
;         mov [rsp + 32], r8 
;         mov [rsp + 40], r9 
;         mov [rsp + 48], r10
; 
;         mov rcx, [rsp]
;         mov rdx, [rsp + 8]
;         mov r8,  [rsp + 16]
;         mov r9,  [rsp + 24]
;         call printf
        
        mov rax, rbx
.done:
        ; return
        add rsp, 56     ; free shadow space
        pop rbp ; restore ebp
        ret 0 ; return from function (0 is default)
