extern _g
extern _asm_min
        global _asm_p
_asm_p:
        ; entry
        push ebp ; store ebp
        mov ebp, esp ; new stackframe

        ; body
        ; inner _asm_min
        mov ecx, [ebp + 12]
        push ecx         ; push j
        mov ecx, [ebp + 8]  
        push ecx        ; push i
        mov ecx, [_g]
        push ecx        ; push g ( global )
        call _asm_min        ; eax = _asm_min(g, i, j)
        add esp, 12     ; free space for pushed args
        
        ; outer _asm_min 
        mov ecx, [ebp + 20] 
        push ecx        ; push l
        mov ecx, [ebp + 16] 
        push ecx        ; push k
        mov ecx, eax
        push ecx        ; push _asm_min(g, i, j)
        call _asm_min        ; eax = _asm_min (_asm_min(g,i,j), k,l)
        add esp, 12     ; free space for pushed vars

        ; return
        mov esp, ebp    ; free locals
        pop ebp         ; restore ebp
        ret 0           ; return from function
