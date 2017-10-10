    section .text
global _asm_min
_asm_min:
        ; entry
        push rbp            ; store ebp
        mov rbp, rsp        ; new stackframe
;         sub esp, 8          ; allocate local vars ( v ) NOT USED
; 
;         mov rcx, 1
;         mov rdx, 2
;         mov r8, 3
        ;body
;         xor rax, rax
        mov rax, rcx        ; v = a
        
        ; b < v
        cmp rax, rdx        ; v > b      
        jle .skip1_min      ; 
        mov rax, rdx        ; v = b
.skip1_min:  
        ; c < v
        cmp rax, r8         ; v > c
        jle .skip2_min 
        mov rax, r8         ; v = c
.skip2_min:  
        ; return
;         mov rax, 23
        ;         mov esp, ebp        ; free locals NOT USED

;         mov rax, r8

        pop rbp             ; restore ebp
        ret               ; return from function
