section .data                        ;; Section for initialized data
    x dd 5 dup(0)
    call_stack_top dd 0
    fcall_stack_top dd 0
    base_stack_top dd 0
    lcall_stack_top dd 0
    pesp dd 0
    pebp dd 0
    buffer db '0000000000', 0       ; Buffer to hold the converted number (10 digits max)
    len equ 10                      ; Length of the buffer
    space db  " "                       ; Space character to print
    condition dd 5 dup(0)
    BASE_ESP dd 0
    BASE_RETURN dd 0
    RETURN_VALUE dd 5 dup(0)
    RETURN_ADDRESS dd 0
    POPED_ADDRESS dd 0
    NUM_REF_ALLO dd 0
section .bss
    call_stack resb 1024               ;; Reserve 1024 bytes (4 KB) for the call stack
    param_stack resb 1024              ;; Reserve 1024 bytes (4 KB) for the param stack
    fcall_stack resb 1024               ;; Reserve 1024 bytes (4 KB) for the call stack
    base_stack resb 1024              ;; Reserve 1024 bytes (4 KB) for the param stack
    lcall_stack resb 1024           ;; Reserve 1024 bytes (4 KB) for the param stack

section .text                        ;; Section for code
    global _start                    ;; Make the _start symbol available to the linker

_start: 
    mov eax , call_stack + 1024
    mov [call_stack_top] , eax                 ;; init call stack 
    mov eax , fcall_stack + 1024
    mov [fcall_stack_top] , eax                 ;; init call stack 
    mov eax , base_stack + 1024
    mov [base_stack_top] , eax                 ;; init call stack 
    mov eax , lcall_stack + 1024
    mov [lcall_stack_top] , eax
    mov eax , param_stack + 1024
    mov [pesp] , eax
    mov [pebp] , eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 50
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 60
    push eax
    mov eax , [pebp]                  ;; store value at pebp to eax
    call push_stack                   ;; push [pebp] to stack
    mov eax , [pesp]                  ;; store the value at pesp to eax
    mov [pebp] , eax                ;; allocate new base pointer
    sub dword [pesp] , 5                         ;; allocate space for args on stack
    sub dword [pesp] , 5                         ;; allocate space for args on stack
    pop eax
    pop bx
    mov ecx , [pebp]                        ;; store the address value to ebx
    mov [ecx - 10] , bl   
    mov [ecx - 10 + 1] , eax
    pop eax
    pop bx
    mov ecx , [pebp]                        ;; store the address value to ebx
    mov [ecx - 5] , bl   
    mov [ecx - 5 + 1] , eax
    call  add
    mov eax , [pebp]                   ;; store the value at pebp to eax
    mov [pesp] , eax                ;; restore the stack pointer
    call pop_stack                     ;; pop stack top to eax
    mov [pebp] , eax                    ; restore the base pointer
    mov bx , [RETURN_VALUE]
    mov eax , [RETURN_VALUE  + 1]
    push bx
    push eax
    pop eax
    pop bx 
    mov [x] , bx
    mov [x + 1] , eax
    push bx
    push eax
    pop eax
    pop bx
    call print_eax
    mov bl , [x]
    movzx bx , bl 
    mov eax , [x + 1]
    push bx
    push eax
    mov eax , [pebp]                  ;; store value at pebp to eax
    call push_stack                   ;; push [pebp] to stack
    mov eax , [pesp]                  ;; store the value at pesp to eax
    mov [pebp] , eax                ;; allocate new base pointer
    sub dword [pesp] , 5                         ;; allocate space for args on stack
    pop eax
    pop bx
    mov ecx , [pebp]                        ;; store the address value to ebx
    mov [ecx - 5] , bl   
    mov [ecx - 5 + 1] , eax
    call  fun
    mov eax , [pebp]                   ;; store the value at pebp to eax
    mov [pesp] , eax                ;; restore the stack pointer
    call pop_stack                     ;; pop stack top to eax
    mov [pebp] , eax                    ; restore the base pointer
    mov bx , [RETURN_VALUE]
    mov eax , [RETURN_VALUE  + 1]
    push bx
    push eax
    pop eax
    pop bx 
    mov [x] , bx
    mov [x + 1] , eax
    push bx
    push eax
    pop eax
    pop bx
    call print_eax
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
;; this method is used for pushing the loop return address to stack store in eax register
push_lcall:
   sub dword [lcall_stack_top] , 4              ;; decrement stack by 4
   mov ebx , [lcall_stack_top]                  ;; load value of call_stack_top to ebx
   mov dword [ebx] , eax                       ;; store the value of eax to location stored in ebx
    ret
;; this method is used for poping the callers return address from stack and  store  it in eax register
pop_lcall:
   mov eax , [lcall_stack_top]                  ;; load value of call_stack_top to eax
   mov eax , [eax]                             ;; load value from addressed stored in eax to eax
    add dword [lcall_stack_top] , 4          ;; increment stack by 4
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
add:
    pop eax                        ;; pop the return address to eax
    call push_call                 ;; store the return address to ra location
    call push_fcall
    mov eax , [pebp]
    call push_base
    mov eax , [pesp]
    call push_base
    mov eax , [pebp]                  ;; store value at pebp to eax
    call push_stack                   ;; push [pebp] to stack
    mov eax , [pesp]                  ;; store the value at pesp to eax
    mov [pebp] , eax                ;; allocate new base pointer
    sub dword [pesp] , 5
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 10
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (-5)] , bx              
    mov [ecx + (-5) + 1] , eax
    push bx
    push eax
    mov ecx , [pebp] 
    mov bl , [ ecx + (-5) ]
    movzx bx , bl 
    mov eax , [ecx + (-5) + 1 ] 
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 0
    push eax
    pop ebx
    pop dx 
    pop eax
    pop cx 
    cmp eax , ebx
    setg al
    movzx eax , al
    mov bl , [INT_TYPE]
    movzx bx , bl
    push   bx
    push eax
    pop eax
    pop bx
    mov [condition] , bx
    mov [condition + 1] , eax
    lea eax , [.label_846930886]
    push eax
    mov eax , [condition + 1]
    test eax , eax
    jnz label_1804289383
    pop eax
    lea eax , [.label_846930886]         ;; save the false label to eax
    jmp eax
.label_846930886:                        ;; defination of false label 
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 50
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (-5)] , bx              
    mov [ecx + (-5) + 1] , eax
    push bx
    push eax
    mov  byte [RETURN_VALUE] , 0
    mov  dword [RETURN_VALUE + 1] , 0
    mov ecx , [pebp] 
    mov bl , [ ecx + (-5) ]
    movzx bx , bl 
    mov eax , [ecx + (-5) + 1 ] 
    push bx
    push eax
    pop eax
    pop bx
    mov [RETURN_VALUE] , bl
    mov [RETURN_VALUE + 1] , eax
    call pop_fcall
    mov [RETURN_ADDRESS], eax
.label_1681692777:                                 ;; function to clean up call stack 
    call pop_call
    mov [POPED_ADDRESS] , eax
    mov eax ,[POPED_ADDRESS]
    cmp eax, [RETURN_ADDRESS]
    jne .label_1681692777
    mov eax , [RETURN_ADDRESS]
    jmp eax
    mov eax , [pebp]                   ;; store the value at pebp to eax
    mov [pesp] , eax                ;; restore the stack pointer
    call pop_stack                     ;; pop stack top to eax
    mov [pebp] , eax                    ; restore the base pointer
    call pop_base
    mov [pesp] , eax
    call pop_base
    mov [pebp] , eax 
    call pop_call                  ;; store the return address to eax
    call pop_fcall
    jmp eax                        ;; jmp to return address
label_1804289383:
    pop eax                        ;; pop the return address to eax
    call push_call                 ;; store the return address to ra location
    call push_lcall
    lea eax , [label_1804289383]
    call push_lcall
    mov eax , [pebp]
    call push_base
    mov eax , [pesp]
    call push_base
    mov eax , [pebp]                  ;; store value at pebp to eax
    call push_stack                   ;; push [pebp] to stack
    mov eax , [pesp]                  ;; store the value at pesp to eax
    mov [pebp] , eax                ;; allocate new base pointer
    mov ecx , [pebp] 
    mov bl , [ ecx + (4) ]
    movzx bx , bl 
    mov eax , [ecx + (4) + 1 ] 
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 5
    push eax
    pop ebx
    pop dx 
    pop eax
    pop cx 
    cmp eax , ebx
    sete al
    movzx eax , al
    mov bl , [INT_TYPE]
    movzx bx , bl
    push   bx
    push eax
    pop eax
    test eax , eax
    lea eax , [.label_1957747793]
    push eax
    jnz label_1714636915                 ;; jump if expression  is not  zero
    pop eax
    lea eax , [.label_1957747793]         ;; save the false label to eax
    jmp eax
.label_1957747793:                        ;; defination of false label 
    mov ecx , [pebp] 
    mov bl , [ ecx + (4) ]
    movzx bx , bl 
    mov eax , [ecx + (4) + 1 ] 
    push bx
    push eax
    mov eax , [pebp] 
    dec dword [eax + (4) + 1  ]     ;; length
    pop eax
    pop bx
    call print_eax
    mov eax , [pebp]                   ;; store the value at pebp to eax
    mov [pesp] , eax                ;; restore the stack pointer
    call pop_stack                     ;; pop stack top to eax
    mov [pebp] , eax                    ; restore the base pointer
    mov ecx , [pebp] 
    mov bl , [ ecx + (-5) ]
    movzx bx , bl 
    mov eax , [ecx + (-5) + 1 ] 
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 0
    push eax
    pop ebx
    pop dx 
    pop eax
    pop cx 
    cmp eax , ebx
    setg al
    movzx eax , al
    mov bl , [INT_TYPE]
    movzx bx , bl
    push   bx
    push eax
    pop eax
    pop bx
    mov [condition] , bl
    mov [condition + 1] ,  eax
    call pop_base
    mov [pesp] , eax
    call pop_base
    mov [pebp] , eax 
    call pop_lcall
    call pop_lcall
    call pop_call                  ;; store the return address to eax
    push eax
    mov eax , [condition + 1]
    test eax , eax
    jnz label_1804289383
    pop eax
    jmp eax                        ;; jmp to return address
label_1714636915:
    pop eax                        ;; pop the return address to eax
    call push_call                 ;; store the return address to ra location
    mov eax , [pebp]                  ;; store value at pebp to eax
    call push_stack                   ;; push [pebp] to stack
    mov eax , [pesp]                  ;; store the value at pesp to eax
    mov [pebp] , eax                ;; allocate new base pointer
    mov ecx , [pebp] 
    mov bl , [ ecx + (8) ]
    movzx bx , bl 
    mov eax , [ecx + (8) + 1 ] 
    push bx
    push eax
    mov eax , [pebp] 
    dec dword [eax + (8) + 1  ]     ;; length
    call pop_base
    mov [pesp] , eax                ;; restore the base pointer
    call pop_base
    mov [pebp] , eax                ;; restore the base pointer
    call pop_lcall
    call pop_lcall
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
    call pop_call                  ;; store the return address to eax
    jmp eax                        ;; jmp to return address
fun:
    pop eax                        ;; pop the return address to eax
    call push_call                 ;; store the return address to ra location
    call push_fcall
    mov eax , [pebp]
    call push_base
    mov eax , [pesp]
    call push_base
    mov eax , [pebp]                  ;; store value at pebp to eax
    call push_stack                   ;; push [pebp] to stack
    mov eax , [pesp]                  ;; store the value at pesp to eax
    mov [pebp] , eax                ;; allocate new base pointer
    sub dword [pesp] , 5
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 6
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (-5)] , bx              
    mov [ecx + (-5) + 1] , eax
    push bx
    push eax
    mov ecx , [pebp] 
    mov bl , [ ecx + (4) ]
    movzx bx , bl 
    mov eax , [ecx + (4) + 1 ] 
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 3
    push eax
    pop ebx
    pop dx 
    pop eax
    pop cx 
    cmp eax , ebx
    setg al
    movzx eax , al
    mov bl , [INT_TYPE]
    movzx bx , bl
    push   bx
    push eax
    pop eax
    pop bx
    mov [condition] , bx
    mov [condition + 1] , eax
    lea eax , [.label_719885386]
    push eax
    mov eax , [condition + 1]
    test eax , eax
    jnz label_424238335
    pop eax
    lea eax , [.label_719885386]         ;; save the false label to eax
    jmp eax
.label_719885386:                        ;; defination of false label 
    mov  byte [RETURN_VALUE] , 0
    mov  dword [RETURN_VALUE + 1] , 0
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 99
    push eax
    pop eax
    pop bx
    mov [RETURN_VALUE] , bl
    mov [RETURN_VALUE + 1] , eax
    call pop_fcall
    mov [RETURN_ADDRESS], eax
.label_1649760492:                                 ;; function to clean up call stack 
    call pop_call
    mov [POPED_ADDRESS] , eax
    mov eax ,[POPED_ADDRESS]
    cmp eax, [RETURN_ADDRESS]
    jne .label_1649760492
    mov eax , [RETURN_ADDRESS]
    jmp eax
    mov eax , [pebp]                   ;; store the value at pebp to eax
    mov [pesp] , eax                ;; restore the stack pointer
    call pop_stack                     ;; pop stack top to eax
    mov [pebp] , eax                    ; restore the base pointer
    call pop_base
    mov [pesp] , eax
    call pop_base
    mov [pebp] , eax 
    call pop_call                  ;; store the return address to eax
    call pop_fcall
    jmp eax                        ;; jmp to return address
label_424238335:
    pop eax                        ;; pop the return address to eax
    call push_call                 ;; store the return address to ra location
    call push_lcall
    lea eax , [label_424238335]
    call push_lcall
    mov eax , [pebp]
    call push_base
    mov eax , [pesp]
    call push_base
    mov eax , [pebp]                  ;; store value at pebp to eax
    call push_stack                   ;; push [pebp] to stack
    mov eax , [pesp]                  ;; store the value at pesp to eax
    mov [pebp] , eax                ;; allocate new base pointer
    mov ecx , [pebp] 
    mov bl , [ ecx + (13) ]
    movzx bx , bl 
    mov eax , [ecx + (13) + 1 ] 
    push bx
    push eax
    mov eax , [pebp] 
    dec dword [eax + (13) + 1  ]     ;; x
    mov eax , [pebp]                   ;; store the value at pebp to eax
    mov [pesp] , eax                ;; restore the stack pointer
    call pop_stack                     ;; pop stack top to eax
    mov [pebp] , eax                    ; restore the base pointer
    mov ecx , [pebp] 
    mov bl , [ ecx + (4) ]
    movzx bx , bl 
    mov eax , [ecx + (4) + 1 ] 
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 3
    push eax
    pop ebx
    pop dx 
    pop eax
    pop cx 
    cmp eax , ebx
    setg al
    movzx eax , al
    mov bl , [INT_TYPE]
    movzx bx , bl
    push   bx
    push eax
    pop eax
    pop bx
    mov [condition] , bl
    mov [condition + 1] ,  eax
    call pop_base
    mov [pesp] , eax
    call pop_base
    mov [pebp] , eax 
    call pop_lcall
    call pop_lcall
    call pop_call                  ;; store the return address to eax
    push eax
    mov eax , [condition + 1]
    test eax , eax
    jnz label_424238335
    pop eax
    jmp eax                        ;; jmp to return address
section .rodata
    INT_TYPE: db 1
    FLOAT_TYPE: db 2
    STRING_TYPE: db 3
    REF_TYPE: db 4
