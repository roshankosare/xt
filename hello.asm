section .data                        ;; Section for initialized data
    a dd 0
    MSIZE dd 0
    MMEM dd 0
    x_t dd 0
    g dd 0
    call_stack_top dd 0
    fcall_stack_top dd 0
    base_stack_top dd 0
    pesp dd 0
    pebp dd 0
    buffer db '0000000000', 0       ; Buffer to hold the converted number (10 digits max)
    len equ 10                      ; Length of the buffer
    space db  " "                       ; Space character to print
    condition dd 0
    BASE_ESP dd 0
    BASE_RETURN dd 0
    RETURN_VALUE dd 0
    RETURN_ADDRESS dd 0
    POPED_ADDRESS dd 00
section .bss
    call_stack resb 1024               ;; Reserve 1024 bytes (4 KB) for the call stack
    param_stack resb 1024              ;; Reserve 1024 bytes (4 KB) for the param stack
    fcall_stack resb 1024               ;; Reserve 1024 bytes (4 KB) for the call stack
    base_stack resb 1024              ;; Reserve 1024 bytes (4 KB) for the param stack

section .text                        ;; Section for code
    global _start                    ;; Make the _start symbol available to the linker

_start: 
    mov eax , call_stack + 1024
    mov [call_stack_top] , eax                 ;; init call stack 
    mov eax , fcall_stack + 1024
    mov [fcall_stack_top] , eax                 ;; init call stack 
    mov eax , base_stack + 1024
    mov [base_stack_top] , eax                 ;; init call stack 
    mov eax , param_stack + 1024
    mov [pesp] , eax
    mov [pebp] , eax
    mov eax, 0
    push eax
    pop eax
    mov [a] , eax
    push eax
    mov eax, 0
    push eax
    pop eax
    mov [MSIZE] , eax
    push eax
    mov eax, 0
    push eax
    pop eax
    mov [MMEM] , eax
    push eax
    mov eax, 3
    push eax
    pop eax
    mov [x_t] , eax
    push eax
    mov eax, 5
    push eax
    mov eax , [a]
    push eax
    pop eax
    pop ebx
    and eax ,ebx
    push eax
    pop eax
    mov [g] , eax
    push eax
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
;; this method is used for pushing the callers base address to stack store in eax register
push_base:
   sub dword [base_stack_top] , 4              ;; decrement stack by 4
   mov ebx , [base_stack_top]                  ;; load value of call_stack_top to ebx
   mov dword [ebx] , eax                       ;; store the value of eax to location stored in ebx
    ret
;; this method is used for poping  callers base address from stack and  store  it in eax register
pop_base:
   mov eax , [base_stack_top]                  ;; load value of call_stack_top to eax
   mov eax , [eax]                             ;; load value from addressed stored in eax to eax
    add dword [base_stack_top] , 4          ;; increment stack by 4
   ret
;; this method is used for pushing the callers return address to stack store in eax register
push_fcall:
   sub dword [fcall_stack_top] , 4              ;; decrement stack by 4
   mov ebx , [fcall_stack_top]                  ;; load value of call_stack_top to ebx
   mov dword [ebx] , eax                       ;; store the value of eax to location stored in ebx
    ret
;; this method is used for poping the callers return address from stack and  store  it in eax register
pop_fcall:
   mov eax , [fcall_stack_top]                  ;; load value of call_stack_top to eax
   mov eax , [eax]                             ;; load value from addressed stored in eax to eax
    add dword [fcall_stack_top] , 4          ;; increment stack by 4
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
malloc:
     push esi           ; Save esi on the stack
     push ebp           ; Save ebp on the stack
     xor eax, eax       ; Clear eax
     mov al, 192        ; sys_mmap system call number (9)
     xor ebx, ebx       ; addr = NULL (let kernel choose the address)
     mov edx, 3         ; prot = PROT_READ | PROT_WRITE (read/write access)
     mov esi, 0x22      ; flags = MAP_PRIVATE | MAP_ANONYMOUS
     mov edi, -1        ; fd = -1 (no file descriptor since MAP_ANONYMOUS)
     xor ebp, ebp       ; offset = 0 (not applicable since no file is mapped)
     ; At this point, ecx contains the size for mmap, which should be passed as a parameter
     int 0x80           ; Call kernel to perform mmap
     pop ebp            ; Restore ebp
     pop esi            ; Restore esi
     ret
section .rodata
