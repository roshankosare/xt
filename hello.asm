section .data          ; Section for initialized data
    x dd 0

section .text          ; Section for code
    global _start      ; Make the _start symbol available to the linker

_start: 
    mov eax, 30
    push eax
    pop eax
    mov [x] , eax
    push eax
    mov eax , 10
    mov eax , 20
    mov eax , 30
; Exit the program
    mov eax, 1         ; syscall number for sys_exit
    xor ebx, ebx       ; exit code 0
    int 0x80           ; make syscall
