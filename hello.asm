section .data          ; Section for initialized data
    x dd 0
    z dd 0
section .bss           ; Section for uninitialized data
section .text          ; Section for code
    global _start      ; Make the _start symbol available to the linker
 _start: 
    mov eax, 10
    push eax
    mov eax, 12
    push eax
    ;; plus
    pop eax
    pop ebx
    add eax ,ebx
    push eax
    pop eax
    mov [x], eax
    push eax
    mov eax, 20
    push eax
    pop eax
    mov [z], eax
    push eax
    mov eax, 5
    push eax
    pop eax
    mov [x], eax
    push eax
    rbp                     ; Save base pointer
    mov rbp, rsp            ; Establish new base pointer
    sub esp, 4
    sub esp, 4
    mov eax, 5
    push eax
    pop eax
    mov [x], eax
    push eax
    rbp                     ; Save base pointer
    mov rbp, rsp            ; Establish new base pointer
    sub esp, 4
    mov rsp, rbp            ; Restore the stack pointer
    pop rbp                 ; Restore the base pointer
    mov rsp, rbp            ; Restore the stack pointer
    pop rbp                 ; Restore the base pointer
; Exit the program
    mov eax, 1         ; syscall number for sys_exit
    xor ebx, ebx       ; exit code 0
    int 0x80           ; make syscall
