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
    push ebp                ; Save base pointer
    mov ebp, esp            ; Establish new base pointer
    sub esp , 4           ;; allocate space for args on stack
    lea eax, [label_1804289383]  ; Load the address of litral into eax
    push eax
    pop eax
    mov [ebp - 4] , eax       ;; args no:- 1
    call  print
    mov esp, ebp            ; Restore the stack pointer
    pop ebp                 ; Restore the base pointer
; Exit the program
    mov eax, 1         ; syscall number for sys_exit
    xor ebx, ebx       ; exit code 0
    int 0x80           ; make syscall
print:
    push ebp                ; Save base pointer
    mov ebp, esp            ; Establish new base pointer
    sub esp , 4
    sub esp , 4
    sub esp , 4
    mov eax, 50
    push eax
    pop eax
    mov [ebp + 0 - 4  ] , eax   ;;length
    push eax
    mov eax , [ebp + 0 - 4  ]     ;; length
    push eax
    pop eax
    mov [ebp + 0 - 8  ] , eax   ;;strlength
    push eax
    mov eax , [ebp + 12 - 4  ]     ;; x
    push eax
    pop eax
    mov [ebp + 0 - 12  ] , eax   ;;strpointer
    push eax
    pop ecx                           ; load the address of the string
    pop edx                           ; load the length of the string
    mov eax, 4                        ; syscall number for sys_write
    mov ebx, 1                        ; file descriptor 1 (stdout)
    int 0x80                          ; make syscall to write
    mov esp, ebp            ; Restore the stack pointer
    pop ebp                 ; Restore the base pointer
    pop eax
    jmp eax
section .rodata
    label_1804289383: dd "hello world my name is roshan ksoare  and i am from wadsa desaiganj", 0
