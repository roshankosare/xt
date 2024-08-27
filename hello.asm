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
    call  fun
    mov esp, ebp            ; Restore the stack pointer
    pop ebp                 ; Restore the base pointer
; Exit the program
    mov eax, 1         ; syscall number for sys_exit
    xor ebx, ebx       ; exit code 0
    int 0x80           ; make syscall
fun:
    push ebp                ; Save base pointer
    mov ebp, esp            ; Establish new base pointer
    sub esp , 4
    lea eax, [label_1804289383]  ; Load the address of litral into eax
    push eax
    pop eax
    mov [ebp + 0 - 4  ] , eax   ;;msg
    push eax
    mov esp, ebp            ; Restore the stack pointer
    pop ebp                 ; Restore the base pointer
    pop eax
    jmp eax
section .rodata
    label_1804289383: dd "hello world", 0
