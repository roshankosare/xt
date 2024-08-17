section .data          ; Section for initialized data
    x dd 0
section .bss           ; Section for uninitialized data
section .text          ; Section for code
    global _start      ; Make the _start symbol available to the linker
 _start: 
    mov eax, 10
    push eax
    pop eax
    mov [x], eax
    push eax
    mov eax, [x]
    mov eax, 10
    push eax
    pop eax
    pop ebx
    cmp ebx,eax
    setg al
    movzx eax, al
    push eax
    pop eax
    test eax eax
    jz .label_1229627179      ;; jump if expression  is zero
    ;; if block start
    push rbp                ; Save base pointer
    mov rbp, rsp            ; Establish new base pointer
    sub esp, 4
    mov eax, 50
    push eax
    pop eax
    mov [ebp + 0 - 4  ], eax   ;;j
    push eax
    mov rsp, rbp            ; Restore the stack pointer
    pop rbp                 ; Restore the base pointer
    ;; if block ends
.label_1229627179    mov eax, 10
    push eax
    mov eax, 20
    push eax
    ;; plus
    pop eax
    pop ebx
    add eax ,ebx
    push eax
    pop eax
    mov [x], eax
    push eax
; Exit the program
    mov eax, 1         ; syscall number for sys_exit
    xor ebx, ebx       ; exit code 0
    int 0x80           ; make syscall
