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
    push ebp                ; Save base pointer
    mov ebp, esp            ; Establish new base pointer
    mov eax, 10
    push eax
    mov eax, 20
    push eax
    pop eax
    mov [ebp - 4] , eax       ;; args no:- 1
    mov eax, 20
    push eax
    pop eax
    mov [ebp - 8] , eax       ;; args no:- 2
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
