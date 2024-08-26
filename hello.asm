section .data          ; Section for initialized data
    x dd 0

section .text          ; Section for code
    global _start      ; Make the _start symbol available to the linker

_start: 
    mov eax, 10
    push eax
    pop eax
    mov [x] , eax
    push eax
    mov eax , [x]
    push eax
    mov eax, 10
    push eax
    pop eax
    pop ebx
    cmp ebx , eax
    setg al
    movzx eax , al
    push eax
    pop eax
    test eax , eax
    lea eax , [label_846930886]
    push eax
    jnz label_1804289383      ;; jump if expression  is zero
label_846930886:
    mov eax , [x]
    push eax
    mov eax, 10
    push eax
    ;; plus
    pop eax
    pop ebx
    add eax , ebx
    push eax
; Exit the program
    mov eax, 1         ; syscall number for sys_exit
    xor ebx, ebx       ; exit code 0
    int 0x80           ; make syscall
fun:
    push ebp                ; Save base pointer
    mov ebp, esp            ; Establish new base pointer
    mov eax , [x]
    push eax
    mov eax, 10
    push eax
    ;; plus
    pop eax
    pop ebx
    add eax , ebx
    push eax
    pop eax
    mov [x] , eax
    push eax
    mov eax , [ebp + 12 - 4  ]     ;; a
    push eax
    mov eax , [ebp + 12 - 8  ]     ;; b
    push eax
    ;; plus
    pop eax
    pop ebx
    add eax , ebx
    push eax
    mov esp, ebp            ; Restore the stack pointer
    pop ebp                 ; Restore the base pointer
    pop eax
    jmp eax
label_1804289383:
    push ebp                ; Save base pointer
    mov ebp, esp            ; Establish new base pointer
    sub esp , 4
    mov eax, 10
    push eax
    pop eax
    mov [ebp + 0 - 4  ] , eax   ;;k
    push eax
    sub esp , 4
    mov eax, 8
    push eax
    pop eax
    mov [ebp + 0 - 8  ] , eax   ;;j
    push eax
    mov eax , [x]
    push eax
    mov eax, 10
    push eax
    pop eax
    pop ebx
    cmp ebx , eax
    setl al
    movzx eax , al
    push eax
    pop eax
    test eax , eax
    lea eax , [label_1714636915]
    push eax
    jnz label_1681692777      ;; jump if expression  is zero
label_1714636915:
    mov esp, ebp            ; Restore the stack pointer
    pop ebp                 ; Restore the base pointer
    pop eax
    jmp eax
label_1681692777:
    push ebp                ; Save base pointer
    mov ebp, esp            ; Establish new base pointer
    mov eax, 22
    push eax
    pop eax
    mov [ebp + 8 - 4  ] , eax   ;;k
    push eax
    mov eax , [ebp + 8 - 8  ]     ;; j
    push eax
    mov eax, 10
    push eax
    pop eax
    pop ebx
    cmp ebx , eax
    setl al
    movzx eax , al
    push eax
    pop eax
    test eax , eax
    lea eax , [label_424238335]
    push eax
    jnz label_1957747793      ;; jump if expression  is zero
label_424238335:
    mov esp, ebp            ; Restore the stack pointer
    pop ebp                 ; Restore the base pointer
    pop eax
    jmp eax
label_1957747793:
    push ebp                ; Save base pointer
    mov ebp, esp            ; Establish new base pointer
    mov eax, 78
    push eax
    pop eax
    mov [ebp + 8 - 8  ] , eax   ;;j
    push eax
    mov esp, ebp            ; Restore the stack pointer
    pop ebp                 ; Restore the base pointer
    pop eax
    jmp eax
