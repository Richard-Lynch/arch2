;  hello.asm  a first program for nasm for Linux, Intel, gcc
;
; assemble: nasm -f elf -l hello.lst  hello.asm
; link:     gcc -o hello  hello.o
; run:          hello 
; output is:    Hello World 

extern printf

        SECTION .data       ; data section
msg:    db "Hello World",10 ; the string to print, 10=cr
len:    equ $-msg       ; "$" means "here"
d: dd 0

;g:      equ 4
; len is a value, not an address

        SECTION .text       ; code section
        global main     ; make label available to linker 
main:               ; standard  gcc  entry point
; printf
        mov edx,len     ; arg3, length of string to print
        mov ecx,msg     ; arg2, pointer to string
        mov ebx,1       ; arg1, where to write, screen
        mov eax,4       ; write sysout command to int 80 hex
        int 0x80        ; interrupt 80 hex, call kernel
; call min
        push 3 ; ebp + 16
        push 2 ; ebp + 12
        push 1 ; ebp + 8
        call min
        add esp, 12
; exit 
        mov ebx,0       ; exit code, 0=normal
        mov eax,1       ; exit command to kernel
        int 0x80        ; interrupt 80 hex, call kernel

min:
        ; entry
        push ebp ; store ebp
        mov ebp, esp ; new stackframe
        sub esp, 4 ; ebp -4
        ;push ebx ; not needed, never used

        ;body
        mov eax, [ebp+8] ; eax = a
        mov [ebp-4], eax ; v = eax = a
        
        ; b < v
        mov eax, [ebp+12] ; eax = b
        cmp [ebp-4], eax ; v > b      
        jge skip1_min
        mov [ebp-4], eax ; v = b
skip1_min:  
        ; c < v
        mov eax, [ebp+16] ; eax = c
        cmp [ebp-4], eax ; v > c
        jge skip2_min 
        mov [ebp-4], eax ; v = c
skip2_min: 
; printf
        mov eax, [ebp-4] ; load v
        add eax, 0x30 ; convert to ascii
        mov [d], eax ; store in v
        mov edx, 1     ; arg3, length of string to print
        mov ecx, d     ; arg2, pointer to string
        mov ebx, 1       ; arg1, where to write, screen
        mov eax, 4       ; write sysout command to int 80 hex
        int 0x80        ; interrupt 80 hex, call kernel
        ; exit 
        ;pop ebx ; not needed never used     
        mov esp, ebp ; free locals
        pop ebp ; restore ebp
        ret 0 ; return from function (0 is default)
