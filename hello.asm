section .data                        ;; Section for initialized data
    a dd 0
    MSIZE dd 0
    MMEM dd 0
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
    mov eax , [RETURN_VALUE]
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
memalloc:
    pop eax                        ;; pop the return address to eax
    call push_call                 ;; store the return address to ra location
    call push_fcall
    mov eax , [pebp]
    call push_base
    mov eax , [pebp]                  ;; store value at pebp to eax
    call push_stack                   ;; push [pebp] to stack
    mov eax , [pesp]                  ;; store the value at pesp to eax
    mov [pebp] , eax                ;; allocate new base pointer
    mov eax , [pebp]              ;; load the address stored in pebp to eax
    mov eax , [eax + (4) ]     ;; size
    push eax
    pop eax
    mov [MSIZE] , eax
    push eax
    mov eax , [MSIZE]
    mov ecx , eax
    call malloc
    mov [MMEM] , eax
    mov  dword [RETURN_VALUE] , 0
    mov eax , [MMEM]
    push eax
    pop eax
    mov [RETURN_VALUE] , eax
    call pop_base                 ;; pop base pointer value of caller
    mov [pebp] , eax                ;; restore the stack pointer
    call pop_fcall
    mov [RETURN_ADDRESS], eax
.loop:                                 ;; function to clean up call stack 
    call pop_call
    mov [POPED_ADDRESS] , eax
    mov eax ,[POPED_ADDRESS]
    cmp eax, [RETURN_ADDRESS]
    jne .loop
    mov eax , [RETURN_ADDRESS]
    jmp eax
    mov eax , [pebp]                   ;; store the value at pebp to eax
    mov [pesp] , eax                ;; restore the stack pointer
    call pop_stack                     ;; pop stack top to eax
    mov [pebp] , eax                    ; restore the base pointer
    call pop_base
    call pop_fcall
    call pop_call                  ;; store the return address to eax
    jmp eax                        ;; jmp to return address
print:
    pop eax                        ;; pop the return address to eax
    call push_call                 ;; store the return address to ra location
    call push_fcall
    mov eax , [pebp]
    call push_base
    mov eax , [pebp]                  ;; store value at pebp to eax
    call push_stack                   ;; push [pebp] to stack
    mov eax , [pesp]                  ;; store the value at pesp to eax
    mov [pebp] , eax                ;; allocate new base pointer
    sub dword [pesp] , 4
    sub dword [pesp] , 4
    sub dword [pesp] , 4
    sub dword [pesp] , 4
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
    mov eax, 1024
    push eax
    pop eax
    mov ebx , [pebp]                    ;; store the address to ebx
    mov [ebx + (-12)] , eax              ;; store the value at location ebx
    push eax
    mov eax , [pebp]                  ;; store value at pebp to eax
    call push_stack                   ;; push [pebp] to stack
    mov eax , [pesp]                  ;; store the value at pesp to eax
    mov [pebp] , eax                ;; allocate new base pointer
    sub dword [pesp] , 4                         ;; allocate space for args on stack
    mov eax, 1024
    push eax
    pop eax
    mov ebx , [pebp]                        ;; store the address value to ebx
    mov [ebx - 4] , eax                   ;; args no:- 1
    call  memalloc
    mov eax , [pebp]                   ;; store the value at pebp to eax
    mov [pesp] , eax                ;; restore the stack pointer
    call pop_stack                     ;; pop stack top to eax
    mov [pebp] , eax                    ; restore the base pointer
    mov eax , [RETURN_VALUE]
    push eax
    pop eax
    mov ebx , [pebp]                    ;; store the address to ebx
    mov [ebx + (-16)] , eax              ;; store the value at location ebx
    push eax
    mov eax , [pebp]              ;; load the address stored in pebp to eax
    mov eax , [eax + (-16) ]     ;; buffer
    push eax
    pop eax
    mov ebx , [pebp]                    ;; store the address to ebx
    mov [ebx + (-20)] , eax              ;; store the value at location ebx
    push eax
    mov eax, 0
    push eax
    pop eax
    mov ebx , [pebp]                    ;; store the address to ebx
    mov [ebx + (-24)] , eax              ;; store the value at location ebx
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
    lea eax , [.label_1681692777]
    push eax
    mov eax , [condition]
    test eax , eax
    jnz label_846930886
    pop eax
    lea eax , [.label_1681692777]         ;; save the false label to eax
    jmp eax
.label_1681692777:                        ;; defination of false label 
    mov eax , [pebp]              ;; load the address stored in pebp to eax
    mov eax , [eax + (-4) ]     ;; length
    push eax
    pop eax
    mov ebx , [pebp]                    ;; store the address to ebx
    mov [ebx + (-28)] , eax              ;; store the value at location ebx
    push eax
    mov eax , [pebp]              ;; load the address stored in pebp to eax
    mov eax , [eax + (-16) ]     ;; buffer
    push eax
    pop eax
    mov ebx , [pebp]                    ;; store the address to ebx
    mov [ebx + (-32)] , eax              ;; store the value at location ebx
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
    call pop_base
    call pop_fcall
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
    mov eax , [eax + (28) ]     ;; temp
    push eax
    pop eax
    mov eax , [eax]             ;; mov value to eax from address stored at eax
    push eax
    pop eax
    mov ebx , [pebp]                    ;; store the address to ebx
    mov ebx , [ebx + (16)]            ;; store the value at location ebx
    mov [ebx] , eax   ;;tempBuffer
    push eax
    mov eax , [pebp]              ;; load the address stored in pebp to eax
    mov eax , [eax + (28) ]     ;; temp
    push eax
    pop eax
    mov eax , [eax]             ;; mov value to eax from address stored at eax
    push eax
    pop eax
    mov ebx , [pebp]                    ;; store the address to ebx
    mov [ebx + (12)] , eax              ;; store the value at location ebx
    push eax
    pop eax
    and eax , 0xFF
    call print_eax
    mov eax , [pebp]              ;; load the address stored in pebp to eax
    mov eax , [eax + (28) ]     ;; temp
    push eax
    mov eax , [pebp]           ;; move address stored in pebp to eax
    inc dword [eax + (28)]     ;; temp
    mov eax , [pebp]              ;; load the address stored in pebp to eax
    mov eax , [eax + (16) ]     ;; tempBuffer
    push eax
    mov eax , [pebp]           ;; move address stored in pebp to eax
    inc dword [eax + (16)]     ;; tempBuffer
    mov eax , [pebp]              ;; load the address stored in pebp to eax
    mov eax , [eax + (32) ]     ;; length
    push eax
    mov eax , [pebp]           ;; move address stored in pebp to eax
    inc dword [eax + (32)]     ;; length
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
    label_1804289383: dd " hello world ", 0
