extern _g
extern _asm_min
        global _asm_p
_asm_p:
        ; entry
        push rbp        ; store ebp
        mov rbp, rsp    ; new stackframe
        sub rsp, 32     ; allocate shadow space for whole function
        
        ; body
        ; inner _asm_min
        mov r10, r9     ; store l
        mov r9, r8      ; store k
    
        mov r8, rdx     ; j in param 3
        mov rdx, rcx    ; i in param 2
        lea rax, [rel _g]
        mov rcx, [rax]   ; g in param 1
        call _asm_min   ; eax = min(g, i, j)

        mov r8, r10     ; l in param 3
        mov rdx, r9     ; k in param 2
        mov rcx, rax    ; min(g, i, j) in param 1
        call _asm_min   ; eax = min(min(g,i,j),k,l)
        
        ; return
        add rsp, 32    ; free shadow space
        pop rbp         ; restore ebp
        ret 0           ; return from function
