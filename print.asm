section .data
    message1 db "Hello, World!", 0xA  ; First string to print with a newline
    len1 equ $ - message1             ; Length of the first string

    message2 db "Goodbye, World!", 0xA  ; Second string to print with a newline
    len2 equ $ - message2             ; Length of the second string
section .bss

section .text
    global _start




_start:
    ; Call print with the first message
    push len1                         ; Push the length of the first message
    push message1                     ; Push the address of the first message
    call print                        ; Call print subroutine
    add esp, 8                        ; Clean up the stack (2 * 4 bytes)

    ; Call print with the second message
    push len2                         ; Push the length of the second message
    push message2                     ; Push the address of the second message
    call print                        ; Call print subroutine
    add esp, 8                        ; Clean up the stack (2 * 4 bytes)

    ; Exit the program
    mov eax, 1                        ; syscall number for sys_exit
    xor ebx, ebx                      ; exit code 0
    int 0x80                          ; make syscall to exit

print:
    ; Arguments:
    ; [esp+4] = length of the string
    ; [esp] = address of the string

    mov eax, 4                        ; syscall number for sys_write
    mov ebx, 1                        ; file descriptor 1 (stdout)
    mov ecx, [esp+4]                  ; load the address of the string
    mov edx, [esp+8]                  ; load the length of the string
    int 0x80                          ; make syscall to write
    ret                               ; return from subroutine
