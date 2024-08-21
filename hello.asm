section .data          ; Section for initialized data
    x dd 0
section .bss           ; Section for uninitialized data
section .text          ; Section for code
    global _start      ; Make the _start symbol available to the linker
 _start: 
    mov eax, 10
    push eax
    pop eax
    mov [x] , eax
    push eax
; Exit the program
    mov eax, 1         ; syscall number for sys_exit
    xor ebx, ebx       ; exit code 0
    int 0x80           ; make syscall
fun
    push rbp                ; Save base pointer
    mov rbp, rsp            ; Establish new base pointer
    sub esp , 4
    mov eax , [ebp + 4 - 0  ]     ;; a
    push eax
    mov eax , [ebp + 4 - 0  ]     ;; b
    push eax
    ;; plus
    pop eax
    pop ebx
    add eax , ebx
    push eax
    pop eax
    mov [ebp + 0 - 4  ] , eax   ;;z
    push eax
    mov eax , [ebp + 0 - 4  ]     ;; z
    push eax
    mov eax, 19
    push eax
    pop eax
    pop ebx
    cmp ebx , eax
    setg al
    movzx eax , al
    push eax
    pop eax
    test eax , eax
    jnz .label_1804289383      ;; jump if expression  is zero
    mov rsp, rbp            ; Restore the stack pointer
    pop rbp                 ; Restore the base pointer
.label_1804289383
    push rbp                ; Save base pointer
    mov rbp, rsp            ; Establish new base pointer
    mov eax, 59
    push eax
    mov eax , [ebp + 8 - 0  ]     ;; b
    push eax
    ;; mul
    pop eax
    pop ebx
    imul eax , ebx
    push eax
    pop eax
    mov [ebp + 8 - 0  ] , eax   ;;a
    push eax
    mov rsp, rbp            ; Restore the stack pointer
    pop rbp                 ; Restore the base pointer
