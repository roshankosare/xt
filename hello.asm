section .data                        ;; Section for initialized data
    a dd 0
    call_stack_top dd 0
    pesp dd 0
    pebp dd 0
    buffer db '0000000000', 0       ; Buffer to hold the converted number (10 digits max)
    len equ 10                      ; Length of the buffer
    space db  " "                       ; Space character to print
    condition dd 0
section .bss
    call_stack resb 4096               ;; Reserve 4096 bytes (4 KB) for the call stack
    param_stack resb 4096              ;; Reserve 4096 bytes (4 KB) for the param stack

section .text                        ;; Section for code
    global _start                    ;; Make the _start symbol available to the linker

_start: 
    mov eax , call_stack + 4096
    mov [call_stack_top] , eax                 ;; init call stack 
    mov eax , param_stack + 4096
    mov [pesp] , eax
    mov [pebp] , eax
    mov eax, 0
    push eax
    pop eax
    mov [a] , eax
    push eax
    mov eax , [pebp]                  ;; store value at pebp to eax
    call push_stack                   ;; push [pebp] to stack
    mov eax , [pesp]                  ;; store the value at pesp to eax
    mov [pebp] , eax                ;; allocate new base pointer
    sub dword [pesp] , 4                         ;; allocate space for args on stack
    lea eax, [label_1804289383]              ;; Load the address of litral into eax
    push eax
    pop eax
    mov ebx , [pebp]                        ;; store the address value to ebx
    mov [ebx - 4] , eax                   ;; args no:- 1
    call  print
    mov eax , [pebp]                   ;; store the value at pebp to eax
    mov [pesp] , eax                ;; restore the stack pointer
    call pop_stack                     ;; pop stack top to eax
    mov [pebp] , eax                    ; restore the base pointer
; Exit the program
    mov eax, 1                       ;; syscall number for sys_exit
    xor ebx, ebx                     ;; exit code 0
    int 0x80                         ;; make syscall
;; this method is used for pushing the return address to stack store in eax register
push_call:
   sub dword [call_stack_top] , 4              ;; decrement stack by 4
   mov ebx , [call_stack_top]                  ;; load value of call_stack_top to ebx
   mov dword [ebx] , eax                       ;; store the value of eax to location stored in ebx
    ret
;; this method is used for poping the return address from stack and  store  it in eax register
pop_call:
   mov eax , [call_stack_top]                  ;; load value of call_stack_top to eax
   mov eax , [eax]                             ;; load value from addressed stored in eax to eax
    add dword [call_stack_top] , 4          ;; increment stack by 4
   ret
;; this method is used for pushing the return address to stack store in eax register
push_stack:
   sub dword [pesp] , 4                         ;; decrement stack by 4
   mov ebx , [pesp]                             ;; load value of stack_top to ebx
   mov dword [ebx] , eax                       ;; store the value of eax to location stored in ebx
    ret
;; this method is used for poping the return address from stack and  store  it in eax register
pop_stack:
   mov eax , [pesp]                             ;; load value of call_stack_top to eax
   mov eax , [eax]                             ;; load value from addressed stored in eax to eax
    add dword [pesp] , 4          ;; increment stack by 4
   ret
print_eax:
    ; Convert number in eax to string
    mov edi, len                    ; Point to the end of the buffer
    mov ecx, 10                     ; Base 10 for division
    lea esi, [buffer + len]         ; Point esi to the last digit in buffer
.print_loop:
    xor edx, edx                    ; Clear edx for division
    div ecx                         ; Divide eax by 10, quotient in eax, remainder in edx
    add dl, '0'                     ; Convert remainder to ASCII
    dec esi                         ; Move the buffer pointer backwards
    mov [esi], dl                   ; Store the ASCII character in buffer
    dec edi                         ; Decrease digit count
    test eax, eax                   ; Check if eax is 0
    jnz .print_loop                 ; If not, continue the loop
    ; Check if the number was zero
    cmp edi, len
    jne .not_zero
    mov byte [esi - 1], '0'         ; Handle case where eax was 0
    dec esi
.not_zero:
    ; Print the string
    mov eax, 4                      ; sys_write system call number
    mov ebx, 1                      ; File descriptor 1 (stdout)
    mov edx, len                    ; Number of bytes to write
    sub edx, edi                    ; Adjust the length to the actual number length
    lea ecx, [buffer + edi]         ; Adjust buffer pointer to start of the number string
    int 0x80                        ; Interrupt to make system call
    ; Print a space after the number
    mov eax, 4                      ; sys_write system call number
    mov ebx, 1                      ; File descriptor 1 (stdout)
    mov ecx, space                  ; Address of the space character
    mov edx, 1                      ; Length of the space character (1 byte)
    int 0x80                        ; Interrupt to make system call
    ret                             ; Return to caller
delay:
    ; Assumes the delay duration is passed in ecx
    mov eax, ecx          ; Load the delay duration into eax
.loop:
    dec eax               ; Decrement eax
    jnz .loop             ; If eax is not zero, repeat the loop
    ret                   ; Return to the caller
print:
    pop eax                        ;; pop the return address to eax
    call push_call                 ;; store the return address to ra location
    mov eax , [pebp]                  ;; store value at pebp to eax
    call push_stack                   ;; push [pebp] to stack
    mov eax , [pesp]                  ;; store the value at pesp to eax
    mov [pebp] , eax                ;; allocate new base pointer
    sub dword [pesp] , 4
    sub dword [pesp] , 4
    sub dword [pesp] , 4
    sub dword [pesp] , 4
    mov eax, 0
    push eax
    pop eax
    mov ebx , [pebp]                    ;; store the address to ebx
    mov [ebx + (-4)] , eax              ;; store the value at location ebx
    push eax
    mov eax , [pebp]              ;; load the address stored in pebp to eax
    mov eax , [eax + (4) ]     ;; x
    push eax
    pop eax
    mov ebx , [pebp]                    ;; store the address to ebx
    mov [ebx + (-8)] , eax              ;; store the value at location ebx
    push eax
    mov eax , [pebp]              ;; load the address stored in pebp to eax
    mov eax , [eax + (-8) ]     ;; temp
    push eax
    pop eax
    mov eax , [eax]             ;; mov value to eax from address stored at eax
    push eax
    mov eax, 0
    push eax
    pop eax
    pop ebx
    cmp eax , ebx
    setne al
    movzx eax , al
    push eax
    pop eax
    mov [condition] , eax
    lea eax , [label_1681692777]
    push eax
    mov eax , [condition]
    test eax , eax
    jnz label_846930886
    pop eax
    lea eax , [label_1681692777]         ;; save the false label to eax
    jmp eax
label_1681692777:                        ;; defination of false label 
    mov eax , [pebp]              ;; load the address stored in pebp to eax
    mov eax , [eax + (-4) ]     ;; length
    push eax
    pop eax
    mov ebx , [pebp]                    ;; store the address to ebx
    mov [ebx + (-12)] , eax              ;; store the value at location ebx
    push eax
    mov eax , [pebp]              ;; load the address stored in pebp to eax
    mov eax , [eax + (4) ]     ;; x
    push eax
    pop eax
    mov ebx , [pebp]                    ;; store the address to ebx
    mov [ebx + (-16)] , eax              ;; store the value at location ebx
    push eax
    pop ecx                           ; load the address of the string
    pop edx                           ; load the length of the string
    mov eax, 4                        ; syscall number for sys_write
    mov ebx, 1                        ; file descriptor 1 (stdout)
    int 0x80                          ; make syscall to write
    mov eax , [pebp]                   ;; store the value at pebp to eax
    mov [pesp] , eax                ;; restore the stack pointer
    call pop_stack                     ;; pop stack top to eax
    mov [pebp] , eax                    ; restore the base pointer
    call pop_call                  ;; store the return address to eax
    jmp eax                        ;; jmp to return address
label_846930886:
    pop eax                        ;; pop the return address to eax
    call push_call                 ;; store the return address to ra location
    mov eax , [pebp]                  ;; store value at pebp to eax
    call push_stack                   ;; push [pebp] to stack
    mov eax , [pesp]                  ;; store the value at pesp to eax
    mov [pebp] , eax                ;; allocate new base pointer
    mov eax , [pebp]              ;; load the address stored in pebp to eax
    mov eax , [eax + (12) ]     ;; temp
    push eax
    mov eax , [pebp]           ;; move address stored in pebp to eax
    inc dword [eax + (12)]     ;; temp
    mov eax , [pebp]              ;; load the address stored in pebp to eax
    mov eax , [eax + (16) ]     ;; length
    push eax
    mov eax , [pebp]           ;; move address stored in pebp to eax
    inc dword [eax + (16)]     ;; length
    mov eax , [pebp]                   ;; store the value at pebp to eax
    mov [pesp] , eax                ;; restore the stack pointer
    call pop_stack                     ;; pop stack top to eax
    mov [pebp] , eax                    ; restore the base pointer
    mov eax , [pebp]              ;; load the address stored in pebp to eax
    mov eax , [eax + (-8) ]     ;; temp
    push eax
    pop eax
    mov eax , [eax]             ;; mov value to eax from address stored at eax
    push eax
    mov eax, 0
    push eax
    pop eax
    pop ebx
    cmp eax , ebx
    setne al
    movzx eax , al
    push eax
    pop eax
    mov [condition] ,  eax
    call pop_call                  ;; store the return address to eax
    push eax
    mov eax , [condition]
    test eax , eax
    jnz label_846930886
    pop eax
    jmp eax                        ;; jmp to return address
section .rodata
    label_1804289383: dd "hello world oisdhfoi popsod jfpospf pojsdofpjspdojfpsdpfjsdpfj ", 0
