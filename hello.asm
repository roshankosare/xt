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
    push rbp                ; Save base pointer
    mov rbp, rsp            ; Establish new base pointer
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
; Exit the program
    mov eax, 1         ; syscall number for sys_exit
    xor ebx, ebx       ; exit code 0
    int 0x80           ; make syscall
fun
    push rbp                ; Save base pointer
    mov rbp, rsp            ; Establish new base pointer
    mov eax , [ebp + 8 - 4  ]     ;; a
    push eax
    mov eax , [ebp + 8 - 8  ]     ;; b
    push eax
    ;; plus
    pop eax
    pop ebx
    add eax , ebx
    push eax
    mov rsp, rbp            ; Restore the stack pointer
    pop rbp                 ; Restore the base pointer
