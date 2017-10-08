    section .text
global _asm_min
_asm_min:
        ; entry
        push ebp            ; store ebp
        mov ebp, esp        ; new stackframe
        sub esp, 4          ; allocate local vars ( v )

        ;body
        mov eax, [ebp+8]    ; eax = a
        mov [ebp-4], eax    ; v = eax = a
        
        ; b < v
        mov eax, [ebp+12]   ; eax = b
        cmp [ebp-4], eax    ; v > b      
        jle .skip1_min      ; 
        mov [ebp-4], eax    ; v = b
.skip1_min:  
        ; c < v
        mov eax, [ebp+16]   ; eax = c
        cmp [ebp-4], eax    ; v > c
        jle .skip2_min 
        mov [ebp-4], eax    ; v = c
.skip2_min: 
        ; return v
        mov eax, [ebp-4]    ; load v into eax
.finished:  
        ; return
        mov esp, ebp        ; free locals
        pop ebp             ; restore ebp
        ret 0               ; return from function
