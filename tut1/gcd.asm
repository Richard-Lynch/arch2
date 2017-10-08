        global _asm_gcd
_asm_gcd:
        ; entry
        push ebp        ; store ebp
        mov ebp, esp    ; new stackframe

        ; body
        mov eax, [ebp + 8]  ; eax = a
; if (b == 0) 
        mov ecx, [ebp + 12] ; ecx = b
        cmp ecx, 0
        je .done ; b == 0
; else ( b != 0 )
        cdq         ; convert eax to 64bit in edx:eax
;         xor edx, edx ; [for div]
        idiv ecx    ; eax = ecx % ebx = a % b
        push edx    ; a % b 
        mov eax, [ebp  + 12] ; eax = b
        push eax    ; b
        call _asm_gcd    ; gcd ( b, a%b ) = gcd ( eax, edx )
        add esp, 8 ; free space for params
.done:
        ; return
        mov esp, ebp ; free locals
        pop ebp ; restore ebp
        ret 0 ; return from function (0 is default)
