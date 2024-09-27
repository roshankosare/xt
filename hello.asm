section .data
    space db  " "                       ; Space character to print

    len equ 10                      ; Length of the buffer

    buffer db '0000000000', 0       ; Buffer to hold the converted number (10 digits max)

    lcall_stack_top dd 0

    fcall_stack_top dd 0

    loop_base_stack_top dd 0

    base_stack_top dd 0

    pebp dd 0

    pesp dd 0

    call_stack_top dd 0

    i dd 5 dup(0)

    msg dd 5 dup(0)

    TYPE_REF dd 5 dup(0)

    TYPE_STRING dd 5 dup(0)

    TYPE_FLOAT dd 5 dup(0)

    TYPE_INT dd 5 dup(0)

    k dd 5 dup(0)

    STR_POINTER dd 5 dup(0)

    STR_LENGTH dd 5 dup(0)

    MMEM dd 5 dup(0)

    MSIZE dd 5 dup(0)

    a dd 5 dup(0)
                        ;; Section for initialized data
    condition dd 5 dup(0)
    RETURN_VALUE dd 5 dup(0)
    RETURN_ADDRESS dd 0
    POPED_ADDRESS dd 0
section .bss
    lcall_stack resb 1024           ;; Reserve 1024 bytes (4 KB) for the param stack

    fcall_stack resb 1024               ;; Reserve 1024 bytes (4 KB) for the call stack

    loop_base_stack resb 1024              ;; Reserve 1024 bytes (4 KB) for the param stack

    base_stack resb 1024              ;; Reserve 1024 bytes (4 KB) for the param stack

    param_stack resb 1024              ;; Reserve 1024 bytes (4 KB) for the param stack

    call_stack resb 1024               ;; Reserve 1024 bytes (4 KB) for the call stack


section .rodata
    label_233665123: db "\n", 0

    label_278722862: db " hello\t world \n", 0

    label_861021530: db "o\n", 0

    label_336465782: db "e\n", 0

    INT_TYPE: db 1
    FLOAT_TYPE: db 2
    STRING_TYPE: db 3
    REF_TYPE: db 4
section .text                        ;; Section for code
    global _start 
print_series:
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
    mov ecx , [pebp] 
    mov bl , [ ecx + (4) ]
    movzx bx , bl 
    mov eax , [ecx + (4) + 1 ] 
    push bx
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (-5)] , bx              
    mov [ecx + (-5) + 1] , eax
    push bx
    push eax
    sub dword [pesp] , 5
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 0
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (-10)] , bx              
    mov [ecx + (-10) + 1] , eax
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
    lea eax , [.label_521595368]
    push eax
    mov eax , [condition + 1]
    test eax , eax
    jz .label_521595368
.label_35005211:
    pop eax                        ;; pop the return address to eax
    call push_call                 ;; store the return address to ra location
    call push_lcall
    lea eax , [.label_35005211]
    call push_lcall
    mov eax , [pebp]
    call push_lbase
    mov eax , [pesp]
    call push_lbase
    mov eax , [pebp]                  ;; store value at pebp to eax
    call push_stack                   ;; push [pebp] to stack
    mov eax , [pesp]                  ;; store the value at pesp to eax
    mov [pebp] , eax                ;; allocate new base pointer
    mov ecx , [pebp] 
    mov bl , [ ecx + (9) ]
    movzx bx , bl 
    mov eax , [ecx + (9) + 1 ] 
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 2
    push eax
    pop ebx
    pop cx 
    pop eax
    pop cx 
    xor edx ,edx
    div ebx
    push cx
    push edx
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
    sete al
    movzx eax , al
    mov bl , [INT_TYPE]
    movzx bx , bl
    push   bx
    push eax
    pop eax
    test eax , eax
    jz .label_294702567                 ;; jump if expression  is not  zero
    mov eax , [pebp]                  ;; store value at pebp to eax
    call push_stack                   ;; push [pebp] to stack
    mov eax , [pesp]                  ;; store the value at pesp to eax
    mov [pebp] , eax                ;; allocate new base pointer
    mov bl , [STRING_TYPE]
    movzx bx , bl
    push bx
    lea eax, [label_336465782]              ;; Load the address of litral into eax
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
    call  print
    mov eax , [pebp]                   ;; store the value at pebp to eax
    mov [pesp] , eax                ;; restore the stack pointer
    call pop_stack                     ;; pop stack top to eax
    mov [pebp] , eax                    ; restore the base pointer
    mov bx , [RETURN_VALUE]
    mov eax , [RETURN_VALUE  + 1]
    push bx
    push eax
    mov eax , [pebp]                   ;; store the value at pebp to eax
    mov [pesp] , eax                ;; restore the stack pointer
    call pop_stack                     ;; pop stack top to eax
    mov [pebp] , eax                    ; restore the base pointer
     jmp .label_1726956429                        ;; defination of false label 
.label_294702567:
    mov eax , [pebp]                  ;; store value at pebp to eax
    call push_stack                   ;; push [pebp] to stack
    mov eax , [pesp]                  ;; store the value at pesp to eax
    mov [pebp] , eax                ;; allocate new base pointer
    mov bl , [STRING_TYPE]
    movzx bx , bl
    push bx
    lea eax, [label_861021530]              ;; Load the address of litral into eax
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
    call  print
    mov eax , [pebp]                   ;; store the value at pebp to eax
    mov [pesp] , eax                ;; restore the stack pointer
    call pop_stack                     ;; pop stack top to eax
    mov [pebp] , eax                    ; restore the base pointer
    mov bx , [RETURN_VALUE]
    mov eax , [RETURN_VALUE  + 1]
    push bx
    push eax
    mov eax , [pebp]                   ;; store the value at pebp to eax
    mov [pesp] , eax                ;; restore the stack pointer
    call pop_stack                     ;; pop stack top to eax
    mov [pebp] , eax                    ; restore the base pointer
.label_1726956429:
    mov ecx , [pebp] 
    mov bl , [ ecx + (9) ]
    movzx bx , bl 
    mov eax , [ecx + (9) + 1 ] 
    push bx
    push eax
    mov eax , [pebp] 
    dec dword [eax + (9) + 1  ]     ;; i
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
    mov [condition] , bx
    mov [condition + 1] , eax
    call pop_lbase
    mov [pesp] , eax
    call pop_lbase
    mov [pebp] , eax 
    call pop_lcall
    call pop_lcall
    call pop_call                  ;; store the return address to eax
    push eax
    mov eax , [condition + 1]
    test eax , eax
    jnz .label_35005211
.label_521595368:                        ;; defination of false label 
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

print:
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
    mov ecx , [pebp] 
    mov bl , [ ecx + (4) ]
    movzx bx , bl 
    mov eax , [ecx + (4) + 1 ] 
    push bx
    push eax
    pop eax
    pop bx
    movzx eax , bx 
    mov bx , [INT_TYPE]
    push bx
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
    pop eax
    pop bx
    movzx eax , bx 
    mov bx , [INT_TYPE]
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 1
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
    jz .label_1189641421                 ;; jump if expression  is not  zero
    mov eax , [pebp]                  ;; store value at pebp to eax
    call push_stack                   ;; push [pebp] to stack
    mov eax , [pesp]                  ;; store the value at pesp to eax
    mov [pebp] , eax                ;; allocate new base pointer
    sub dword [pesp] , 5
    mov ecx , [pebp] 
    mov bl , [ ecx + (13) ]
    movzx bx , bl 
    mov eax , [ecx + (13) + 1 ] 
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
    call  print_int
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
    mov ecx , [pebp]                   
    mov [ecx + (-5)] , bx              
    mov [ecx + (-5) + 1] , eax
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 10
    push eax
    pop eax
    pop bx 
    mov [STR_LENGTH] , bx
    mov [STR_LENGTH + 1] , eax
    push bx
    push eax
    mov ecx , [pebp] 
    mov bl , [ ecx + (-5) ]
    movzx bx , bl 
    mov eax , [ecx + (-5) + 1 ] 
    push bx
    push eax
    pop eax
    pop bx 
    mov [STR_POINTER] , bx
    mov [STR_POINTER + 1] , eax
    push bx
    push eax
    mov ecx , [STR_POINTER + 1 ]   ;; file str pointer
    mov edx , [STR_LENGTH  + 1]    ;; str length
    mov eax, 4                
    mov ebx, 1                        ; file descriptor 1 (stdout)
    int 0x80                          ; make syscall to write
    mov  byte [RETURN_VALUE] , 0
    mov  dword [RETURN_VALUE + 1] , 0
    call pop_base
    mov [pesp] , eax
    call pop_base
    mov [pebp] , eax 
    call pop_fcall
    mov [RETURN_ADDRESS], eax
.label_1350490027:                                 ;; function to clean up call stack 
    call pop_call
    mov [POPED_ADDRESS] , eax
    mov eax ,[POPED_ADDRESS]
    cmp eax, [RETURN_ADDRESS]
    jne .label_1350490027
    mov eax , [RETURN_ADDRESS]
    jmp eax
    mov eax , [pebp]                   ;; store the value at pebp to eax
    mov [pesp] , eax                ;; restore the stack pointer
    call pop_stack                     ;; pop stack top to eax
    mov [pebp] , eax                    ; restore the base pointer
     jmp .label_1025202362                        ;; defination of false label 
.label_1189641421:
.label_1025202362:
    sub dword [pesp] , 5
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 0
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (-10)] , bx              
    mov [ecx + (-10) + 1] , eax
    push bx
    push eax
    sub dword [pesp] , 5
    mov ecx , [pebp] 
    mov bl , [ ecx + (4) ]
    movzx bx , bl 
    mov eax , [ecx + (4) + 1 ] 
    push bx
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (-15)] , bx              
    mov [ecx + (-15) + 1] , eax
    push bx
    push eax
    sub dword [pesp] , 5
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 1024
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (-20)] , bx              
    mov [ecx + (-20) + 1] , eax
    push bx
    push eax
    sub dword [pesp] , 5
    mov ecx , [pebp] 
    mov bl , [ ecx + (-20) ]
    movzx bx , bl 
    mov eax , [ecx + (-20) + 1 ] 
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
    call  memalloc
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
    mov ecx , [pebp]                   
    mov [ecx + (-25)] , bx              
    mov [ecx + (-25) + 1] , eax
    push bx
    push eax
    sub dword [pesp] , 5
    mov ecx , [pebp] 
    mov bl , [ ecx + (-25) ]
    movzx bx , bl 
    mov eax , [ecx + (-25) + 1 ] 
    push bx
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (-30)] , bx              
    mov [ecx + (-30) + 1] , eax
    push bx
    push eax
    sub dword [pesp] , 5
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 0
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (-35)] , bx              
    mov [ecx + (-35) + 1] , eax
    push bx
    push eax
    mov ecx , [pebp] 
    mov bl , [ ecx + (-15) ]
    movzx bx , bl 
    mov eax , [ecx + (-15) + 1 ] 
    push bx
    push eax
    pop eax
    pop bx
    mov eax , [eax]             ;; mov value to eax from address stored at eax
    push bx
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (-35)] , bx              
    mov [ecx + (-35) + 1] , eax
    push bx
    push eax
    mov ecx , [pebp] 
    mov bl , [ ecx + (-35) ]
    movzx bx , bl 
    mov eax , [ecx + (-35) + 1 ] 
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 0xFF
    push eax
    pop eax
    pop cx 
    pop ebx
    pop dx 
    and eax ,ebx
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (-35)] , bx              
    mov [ecx + (-35) + 1] , eax
    push bx
    push eax
    sub dword [pesp] , 5
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 10
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (-40)] , bx              
    mov [ecx + (-40) + 1] , eax
    push bx
    push eax
    mov ecx , [pebp] 
    mov bl , [ ecx + (-35) ]
    movzx bx , bl 
    mov eax , [ecx + (-35) + 1 ] 
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
    setne al
    movzx eax , al
    mov bl , [INT_TYPE]
    movzx bx , bl
    push   bx
    push eax
    push eax
    pop eax
    pop bx
    mov [condition] , bx
    mov [condition + 1] , eax
    lea eax , [.label_1102520059]
    push eax
    mov eax , [condition + 1]
    test eax , eax
    jz .label_1102520059
.label_783368690:
    pop eax                        ;; pop the return address to eax
    call push_call                 ;; store the return address to ra location
    call push_lcall
    lea eax , [.label_783368690]
    call push_lcall
    mov eax , [pebp]
    call push_lbase
    mov eax , [pesp]
    call push_lbase
    mov eax , [pebp]                  ;; store value at pebp to eax
    call push_stack                   ;; push [pebp] to stack
    mov eax , [pesp]                  ;; store the value at pesp to eax
    mov [pebp] , eax                ;; allocate new base pointer
    mov ecx , [pebp] 
    mov bl , [ ecx + (29) ]
    movzx bx , bl 
    mov eax , [ecx + (29) + 1 ] 
    push bx
    push eax
    pop eax
    pop bx
    mov eax , [eax]             ;; mov value to eax from address stored at eax
    push bx
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (9)] , bx              
    mov [ecx + (9) + 1] , eax
    push bx
    push eax
    mov ecx , [pebp] 
    mov bl , [ ecx + (9) ]
    movzx bx , bl 
    mov eax , [ecx + (9) + 1 ] 
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 0xFF
    push eax
    pop eax
    pop cx 
    pop ebx
    pop dx 
    and eax ,ebx
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (9)] , bx              
    mov [ecx + (9) + 1] , eax
    push bx
    push eax
    mov ecx , [pebp] 
    mov bl , [ ecx + (9) ]
    movzx bx , bl 
    mov eax , [ecx + (9) + 1 ] 
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 92
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
    jz .label_2044897763                 ;; jump if expression  is not  zero
    mov eax , [pebp]                  ;; store value at pebp to eax
    call push_stack                   ;; push [pebp] to stack
    mov eax , [pesp]                  ;; store the value at pesp to eax
    mov [pebp] , eax                ;; allocate new base pointer
    mov ecx , [pebp] 
    mov bl , [ ecx + (33) ]
    movzx bx , bl 
    mov eax , [ecx + (33) + 1 ] 
    push bx
    push eax
    mov eax , [pebp]           ;; move address stored in pebp to eax
    inc dword [eax + (33) + 1]     ;; temp
    mov ecx , [pebp] 
    mov bl , [ ecx + (33) ]
    movzx bx , bl 
    mov eax , [ecx + (33) + 1 ] 
    push bx
    push eax
    pop eax
    pop bx
    mov eax , [eax]             ;; mov value to eax from address stored at eax
    push bx
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (13)] , bx              
    mov [ecx + (13) + 1] , eax
    push bx
    push eax
    mov ecx , [pebp] 
    mov bl , [ ecx + (13) ]
    movzx bx , bl 
    mov eax , [ecx + (13) + 1 ] 
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 0xFF
    push eax
    pop eax
    pop cx 
    pop ebx
    pop dx 
    and eax ,ebx
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (13)] , bx              
    mov [ecx + (13) + 1] , eax
    push bx
    push eax
    mov ecx , [pebp] 
    mov bl , [ ecx + (13) ]
    movzx bx , bl 
    mov eax , [ecx + (13) + 1 ] 
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 110
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
    jz .label_1365180540                 ;; jump if expression  is not  zero
    mov eax , [pebp]                  ;; store value at pebp to eax
    call push_stack                   ;; push [pebp] to stack
    mov eax , [pesp]                  ;; store the value at pesp to eax
    mov [pebp] , eax                ;; allocate new base pointer
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 10
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (17)] , bx              
    mov [ecx + (17) + 1] , eax
    push bx
    push eax
    mov eax , [pebp]                   ;; store the value at pebp to eax
    mov [pesp] , eax                ;; restore the stack pointer
    call pop_stack                     ;; pop stack top to eax
    mov [pebp] , eax                    ; restore the base pointer
     jmp .label_1540383426                        ;; defination of false label 
.label_1365180540:
.label_1540383426:
    mov ecx , [pebp] 
    mov bl , [ ecx + (13) ]
    movzx bx , bl 
    mov eax , [ecx + (13) + 1 ] 
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 116
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
    jz .label_304089172                 ;; jump if expression  is not  zero
    mov eax , [pebp]                  ;; store value at pebp to eax
    call push_stack                   ;; push [pebp] to stack
    mov eax , [pesp]                  ;; store the value at pesp to eax
    mov [pebp] , eax                ;; allocate new base pointer
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 9
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (17)] , bx              
    mov [ecx + (17) + 1] , eax
    push bx
    push eax
    mov eax , [pebp]                   ;; store the value at pebp to eax
    mov [pesp] , eax                ;; restore the stack pointer
    call pop_stack                     ;; pop stack top to eax
    mov [pebp] , eax                    ; restore the base pointer
     jmp .label_1303455736                        ;; defination of false label 
.label_304089172:
.label_1303455736:
    mov eax , [pebp]                   ;; store the value at pebp to eax
    mov [pesp] , eax                ;; restore the stack pointer
    call pop_stack                     ;; pop stack top to eax
    mov [pebp] , eax                    ; restore the base pointer
     jmp .label_1967513926                        ;; defination of false label 
.label_2044897763:
.label_1967513926:
    mov ecx , [pebp] 
    mov bl , [ ecx + (9) ]
    movzx bx , bl 
    mov eax , [ecx + (9) + 1 ] 
    push bx
    push eax
    pop eax
    pop bx 
    mov ebx , [pebp]                    
    mov ebx , [ebx + (14) + 1]           
    mov [ebx] , eax   ;;tempBuffer
    push eax
    mov ecx , [pebp] 
    mov bl , [ ecx + (29) ]
    movzx bx , bl 
    mov eax , [ecx + (29) + 1 ] 
    push bx
    push eax
    mov eax , [pebp]           ;; move address stored in pebp to eax
    inc dword [eax + (29) + 1]     ;; temp
    mov ecx , [pebp] 
    mov bl , [ ecx + (14) ]
    movzx bx , bl 
    mov eax , [ecx + (14) + 1 ] 
    push bx
    push eax
    mov eax , [pebp]           ;; move address stored in pebp to eax
    inc dword [eax + (14) + 1]     ;; tempBuffer
    mov ecx , [pebp] 
    mov bl , [ ecx + (34) ]
    movzx bx , bl 
    mov eax , [ecx + (34) + 1 ] 
    push bx
    push eax
    mov eax , [pebp]           ;; move address stored in pebp to eax
    inc dword [eax + (34) + 1]     ;; length
    mov eax , [pebp]                   ;; store the value at pebp to eax
    mov [pesp] , eax                ;; restore the stack pointer
    call pop_stack                     ;; pop stack top to eax
    mov [pebp] , eax                    ; restore the base pointer
    mov ecx , [pebp] 
    mov bl , [ ecx + (-35) ]
    movzx bx , bl 
    mov eax , [ecx + (-35) + 1 ] 
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
    setne al
    movzx eax , al
    mov bl , [INT_TYPE]
    movzx bx , bl
    push   bx
    push eax
    push eax
    pop eax
    pop bx
    mov [condition] , bx
    mov [condition + 1] , eax
    call pop_lbase
    mov [pesp] , eax
    call pop_lbase
    mov [pebp] , eax 
    call pop_lcall
    call pop_lcall
    call pop_call                  ;; store the return address to eax
    push eax
    mov eax , [condition + 1]
    test eax , eax
    jnz .label_783368690
.label_1102520059:                        ;; defination of false label 
    mov ecx , [pebp] 
    mov bl , [ ecx + (-10) ]
    movzx bx , bl 
    mov eax , [ecx + (-10) + 1 ] 
    push bx
    push eax
    pop eax
    pop bx 
    mov [STR_LENGTH] , bx
    mov [STR_LENGTH + 1] , eax
    push bx
    push eax
    mov ecx , [pebp] 
    mov bl , [ ecx + (-25) ]
    movzx bx , bl 
    mov eax , [ecx + (-25) + 1 ] 
    push bx
    push eax
    pop eax
    pop bx 
    mov [STR_POINTER] , bx
    mov [STR_POINTER + 1] , eax
    push bx
    push eax
    mov ecx , [STR_POINTER + 1 ]   ;; file str pointer
    mov edx , [STR_LENGTH  + 1]    ;; str length
    mov eax, 4                
    mov ebx, 1                        ; file descriptor 1 (stdout)
    int 0x80                          ; make syscall to write
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

print_int:
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
    mov ecx , [pebp] 
    mov bl , [ ecx + (4) ]
    movzx bx , bl 
    mov eax , [ecx + (4) + 1 ] 
    push bx
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (-5)] , bx              
    mov [ecx + (-5) + 1] , eax
    push bx
    push eax
    sub dword [pesp] , 5
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 12
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (-10)] , bx              
    mov [ecx + (-10) + 1] , eax
    push bx
    push eax
    sub dword [pesp] , 5
    mov ecx , [pebp] 
    mov bl , [ ecx + (-10) ]
    movzx bx , bl 
    mov eax , [ecx + (-10) + 1 ] 
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
    call  memalloc
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
    mov ecx , [pebp]                   
    mov [ecx + (-15)] , bx              
    mov [ecx + (-15) + 1] , eax
    push bx
    push eax
    sub dword [pesp] , 5
    mov ecx , [pebp] 
    mov bl , [ ecx + (-15) ]
    movzx bx , bl 
    mov eax , [ecx + (-15) + 1 ] 
    push bx
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (-20)] , bx              
    mov [ecx + (-20) + 1] , eax
    push bx
    push eax
    mov ecx , [pebp] 
    mov bl , [ ecx + (4) ]
    movzx bx , bl 
    mov eax , [ecx + (4) + 1 ] 
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
    mov eax, 0
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
    jz .label_846930886                 ;; jump if expression  is not  zero
    mov eax , [pebp]                  ;; store value at pebp to eax
    call push_stack                   ;; push [pebp] to stack
    mov eax , [pesp]                  ;; store the value at pesp to eax
    mov [pebp] , eax                ;; allocate new base pointer
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 48
    push eax
    pop eax
    pop bx 
    mov ebx , [pebp]                    
    mov ebx , [ebx + (4) + 1]           
    mov [ebx] , eax   ;;temp_buffer
    push eax
    mov ecx , [pebp] 
    mov bl , [ ecx + (4) ]
    movzx bx , bl 
    mov eax , [ecx + (4) + 1 ] 
    push bx
    push eax
    mov eax , [pebp]           ;; move address stored in pebp to eax
    inc dword [eax + (4) + 1]     ;; temp_buffer
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 0
    push eax
    pop eax
    pop bx 
    mov ebx , [pebp]                    
    mov ebx , [ebx + (4) + 1]           
    mov [ebx] , eax   ;;temp_buffer
    push eax
    mov  byte [RETURN_VALUE] , 0
    mov  dword [RETURN_VALUE + 1] , 0
    mov ecx , [pebp] 
    mov bl , [ ecx + (9) ]
    movzx bx , bl 
    mov eax , [ecx + (9) + 1 ] 
    push bx
    push eax
    pop eax
    pop bx
    mov [RETURN_VALUE] , bl
    mov [RETURN_VALUE + 1] , eax
    call pop_base
    mov [pesp] , eax
    call pop_base
    mov [pebp] , eax 
    call pop_fcall
    mov [RETURN_ADDRESS], eax
.label_1714636915:                                 ;; function to clean up call stack 
    call pop_call
    mov [POPED_ADDRESS] , eax
    mov eax ,[POPED_ADDRESS]
    cmp eax, [RETURN_ADDRESS]
    jne .label_1714636915
    mov eax , [RETURN_ADDRESS]
    jmp eax
    mov eax , [pebp]                   ;; store the value at pebp to eax
    mov [pesp] , eax                ;; restore the stack pointer
    call pop_stack                     ;; pop stack top to eax
    mov [pebp] , eax                    ; restore the base pointer
     jmp .label_1681692777                        ;; defination of false label 
.label_846930886:
.label_1681692777:
    sub dword [pesp] , 5
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 0
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (-25)] , bx              
    mov [ecx + (-25) + 1] , eax
    push bx
    push eax
    sub dword [pesp] , 5
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 0
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (-30)] , bx              
    mov [ecx + (-30) + 1] , eax
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
    lea eax , [.label_424238335]
    push eax
    mov eax , [condition + 1]
    test eax , eax
    jz .label_424238335
.label_1957747793:
    pop eax                        ;; pop the return address to eax
    call push_call                 ;; store the return address to ra location
    call push_lcall
    lea eax , [.label_1957747793]
    call push_lcall
    mov eax , [pebp]
    call push_lbase
    mov eax , [pesp]
    call push_lbase
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
    mov eax , [pebp]           ;; move address stored in pebp to eax
    inc dword [eax + (4) + 1]     ;; length
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 0
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (9)] , bx              
    mov [ecx + (9) + 1] , eax
    push bx
    push eax
    mov ecx , [pebp] 
    mov bl , [ ecx + (29) ]
    movzx bx , bl 
    mov eax , [ecx + (29) + 1 ] 
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 10
    push eax
    pop ebx
    pop cx 
    pop eax
    pop cx 
    xor edx ,edx
    div ebx
    push cx
    push edx
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (9)] , bx              
    mov [ecx + (9) + 1] , eax
    push bx
    push eax
    mov ecx , [pebp] 
    mov bl , [ ecx + (9) ]
    movzx bx , bl 
    mov eax , [ecx + (9) + 1 ] 
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 48
    push eax
    ;; plus
    pop eax
    pop cx 
    pop ebx
    pop dx 
    add eax , ebx
    push cx
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (9)] , bx              
    mov [ecx + (9) + 1] , eax
    push bx
    push eax
    mov ecx , [pebp] 
    mov bl , [ ecx + (9) ]
    movzx bx , bl 
    mov eax , [ecx + (9) + 1 ] 
    push bx
    push eax
    pop eax
    pop bx 
    mov ebx , [pebp]                    
    mov ebx , [ebx + (14) + 1]           
    mov [ebx] , eax   ;;temp_buffer
    push eax
    mov ecx , [pebp] 
    mov bl , [ ecx + (14) ]
    movzx bx , bl 
    mov eax , [ecx + (14) + 1 ] 
    push bx
    push eax
    mov eax , [pebp]           ;; move address stored in pebp to eax
    inc dword [eax + (14) + 1]     ;; temp_buffer
    mov ecx , [pebp] 
    mov bl , [ ecx + (29) ]
    movzx bx , bl 
    mov eax , [ecx + (29) + 1 ] 
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 10
    push eax
    ;; DIV
    pop ebx
    pop cx 
    pop eax
    pop cx 
    cdq
    idiv ebx
    push cx
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (29)] , bx              
    mov [ecx + (29) + 1] , eax
    push bx
    push eax
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
    mov [condition] , bx
    mov [condition + 1] , eax
    call pop_lbase
    mov [pesp] , eax
    call pop_lbase
    mov [pebp] , eax 
    call pop_lcall
    call pop_lcall
    call pop_call                  ;; store the return address to eax
    push eax
    mov eax , [condition + 1]
    test eax , eax
    jnz .label_1957747793
.label_424238335:                        ;; defination of false label 
    sub dword [pesp] , 5
    mov ecx , [pebp] 
    mov bl , [ ecx + (-10) ]
    movzx bx , bl 
    mov eax , [ecx + (-10) + 1 ] 
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
    call  memalloc
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
    mov ecx , [pebp]                   
    mov [ecx + (-35)] , bx              
    mov [ecx + (-35) + 1] , eax
    push bx
    push eax
    mov ecx , [pebp] 
    mov bl , [ ecx + (-35) ]
    movzx bx , bl 
    mov eax , [ecx + (-35) + 1 ] 
    push bx
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (-15)] , bx              
    mov [ecx + (-15) + 1] , eax
    push bx
    push eax
    mov ecx , [pebp] 
    mov bl , [ ecx + (-30) ]
    movzx bx , bl 
    mov eax , [ecx + (-30) + 1 ] 
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
    setge al
    movzx eax , al
    mov bl , [INT_TYPE]
    movzx bx , bl
    push   bx
    push eax
    pop eax
    pop bx
    mov [condition] , bx
    mov [condition + 1] , eax
    lea eax , [.label_1649760492]
    push eax
    mov eax , [condition + 1]
    test eax , eax
    jz .label_1649760492
.label_719885386:
    pop eax                        ;; pop the return address to eax
    call push_call                 ;; store the return address to ra location
    call push_lcall
    lea eax , [.label_719885386]
    call push_lcall
    mov eax , [pebp]
    call push_lbase
    mov eax , [pesp]
    call push_lbase
    mov eax , [pebp]                  ;; store value at pebp to eax
    call push_stack                   ;; push [pebp] to stack
    mov eax , [pesp]                  ;; store the value at pesp to eax
    mov [pebp] , eax                ;; allocate new base pointer
    mov ecx , [pebp] 
    mov bl , [ ecx + (19) ]
    movzx bx , bl 
    mov eax , [ecx + (19) + 1 ] 
    push bx
    push eax
    pop eax
    pop bx
    mov eax , [eax]             ;; mov value to eax from address stored at eax
    push bx
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (14)] , bx              
    mov [ecx + (14) + 1] , eax
    push bx
    push eax
    mov ecx , [pebp] 
    mov bl , [ ecx + (14) ]
    movzx bx , bl 
    mov eax , [ecx + (14) + 1 ] 
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 0xFF
    push eax
    pop eax
    pop cx 
    pop ebx
    pop dx 
    and eax ,ebx
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    push eax
    pop eax
    pop bx 
    mov ecx , [pebp]                   
    mov [ecx + (14)] , bx              
    mov [ecx + (14) + 1] , eax
    push bx
    push eax
    mov ecx , [pebp] 
    mov bl , [ ecx + (14) ]
    movzx bx , bl 
    mov eax , [ecx + (14) + 1 ] 
    push bx
    push eax
    pop eax
    pop bx 
    mov ebx , [pebp]                    
    mov ebx , [ebx + (4) + 1]           
    mov [ebx] , eax   ;;string_number
    push eax
    mov ecx , [pebp] 
    mov bl , [ ecx + (4) ]
    movzx bx , bl 
    mov eax , [ecx + (4) + 1 ] 
    push bx
    push eax
    mov eax , [pebp]           ;; move address stored in pebp to eax
    inc dword [eax + (4) + 1]     ;; string_number
    mov ecx , [pebp] 
    mov bl , [ ecx + (19) ]
    movzx bx , bl 
    mov eax , [ecx + (19) + 1 ] 
    push bx
    push eax
    mov eax , [pebp] 
    dec dword [eax + (19) + 1  ]     ;; temp_buffer
    mov ecx , [pebp] 
    mov bl , [ ecx + (9) ]
    movzx bx , bl 
    mov eax , [ecx + (9) + 1 ] 
    push bx
    push eax
    mov eax , [pebp] 
    dec dword [eax + (9) + 1  ]     ;; length
    mov eax , [pebp]                   ;; store the value at pebp to eax
    mov [pesp] , eax                ;; restore the stack pointer
    call pop_stack                     ;; pop stack top to eax
    mov [pebp] , eax                    ; restore the base pointer
    mov ecx , [pebp] 
    mov bl , [ ecx + (-30) ]
    movzx bx , bl 
    mov eax , [ecx + (-30) + 1 ] 
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
    setge al
    movzx eax , al
    mov bl , [INT_TYPE]
    movzx bx , bl
    push   bx
    push eax
    pop eax
    pop bx
    mov [condition] , bx
    mov [condition + 1] , eax
    call pop_lbase
    mov [pesp] , eax
    call pop_lbase
    mov [pebp] , eax 
    call pop_lcall
    call pop_lcall
    call pop_call                  ;; store the return address to eax
    push eax
    mov eax , [condition + 1]
    test eax , eax
    jnz .label_719885386
.label_1649760492:                        ;; defination of false label 
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 0
    push eax
    pop eax
    pop bx 
    mov ebx , [pebp]                    
    mov ebx , [ebx + (-35) + 1]           
    mov [ebx] , eax   ;;string_number
    push eax
    mov  byte [RETURN_VALUE] , 0
    mov  dword [RETURN_VALUE + 1] , 0
    mov ecx , [pebp] 
    mov bl , [ ecx + (-15) ]
    movzx bx , bl 
    mov eax , [ecx + (-15) + 1 ] 
    push bx
    push eax
    pop eax
    pop bx
    mov [RETURN_VALUE] , bl
    mov [RETURN_VALUE + 1] , eax
    call pop_base
    mov [pesp] , eax
    call pop_base
    mov [pebp] , eax 
    call pop_fcall
    mov [RETURN_ADDRESS], eax
.label_596516649:                                 ;; function to clean up call stack 
    call pop_call
    mov [POPED_ADDRESS] , eax
    mov eax ,[POPED_ADDRESS]
    cmp eax, [RETURN_ADDRESS]
    jne .label_596516649
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

memalloc:
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
    mov ecx , [pebp] 
    mov bl , [ ecx + (4) ]
    movzx bx , bl 
    mov eax , [ecx + (4) + 1 ] 
    push bx
    push eax
    pop eax
    pop bx 
    mov [MSIZE] , bx
    mov [MSIZE + 1] , eax
    push bx
    push eax
    mov eax , [MSIZE]
    mov ecx , eax
    call malloc
    mov bl , [REF_TYPE]
    movzx bx , bl
    mov [MMEM] , bx
    mov [MMEM + 1] , eax
    mov  byte [RETURN_VALUE] , 0
    mov  dword [RETURN_VALUE + 1] , 0
    mov bl , [MMEM]
    movzx bx , bl 
    mov eax , [MMEM + 1]
    push bx
    push eax
    pop eax
    pop bx
    mov [RETURN_VALUE] , bl
    mov [RETURN_VALUE + 1] , eax
    call pop_base
    mov [pesp] , eax
    call pop_base
    mov [pebp] , eax 
    call pop_fcall
    mov [RETURN_ADDRESS], eax
.label_1804289383:                                 ;; function to clean up call stack 
    call pop_call
    mov [POPED_ADDRESS] , eax
    mov eax ,[POPED_ADDRESS]
    cmp eax, [RETURN_ADDRESS]
    jne .label_1804289383
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
                   ;; Make the _start symbol available to the linker

_start: 
    mov eax , call_stack + 1024
    mov [call_stack_top] , eax                 ;; init call stack 
    mov eax , fcall_stack + 1024
    mov [fcall_stack_top] , eax                 ;; init call stack 
    mov eax , base_stack + 1024
    mov [base_stack_top] , eax                 ;; init call stack 
    mov eax , lcall_stack + 1024
    mov [lcall_stack_top] , eax
    mov eax , loop_base_stack + 1024
    mov [loop_base_stack_top] , eax
    mov eax , param_stack + 1024
    mov [pesp] , eax
    mov [pebp] , eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 0
    push eax
    pop eax
    pop bx 
    mov [a] , bx
    mov [a + 1] , eax
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 0
    push eax
    pop eax
    pop bx 
    mov [MSIZE] , bx
    mov [MSIZE + 1] , eax
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 0
    push eax
    pop eax
    pop bx 
    mov [MMEM] , bx
    mov [MMEM + 1] , eax
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 0
    push eax
    pop eax
    pop bx 
    mov [STR_LENGTH] , bx
    mov [STR_LENGTH + 1] , eax
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 0
    push eax
    pop eax
    pop bx 
    mov [STR_POINTER] , bx
    mov [STR_POINTER + 1] , eax
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 505
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 10
    push eax
    pop ebx
    pop cx 
    pop eax
    pop cx 
    xor edx ,edx
    div ebx
    push cx
    push edx
    pop eax
    pop bx 
    mov [k] , bx
    mov [k + 1] , eax
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 1
    push eax
    pop eax
    pop bx 
    mov [TYPE_INT] , bx
    mov [TYPE_INT + 1] , eax
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 2
    push eax
    pop eax
    pop bx 
    mov [TYPE_FLOAT] , bx
    mov [TYPE_FLOAT + 1] , eax
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 3
    push eax
    pop eax
    pop bx 
    mov [TYPE_STRING] , bx
    mov [TYPE_STRING + 1] , eax
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 4
    push eax
    pop eax
    pop bx 
    mov [TYPE_REF] , bx
    mov [TYPE_REF + 1] , eax
    push bx
    push eax
    mov bl , [STRING_TYPE]
    movzx bx , bl
    push bx
    lea eax, [label_278722862]              ;; Load the address of litral into eax
    push eax
    pop eax
    pop bx 
    mov [msg] , bx
    mov [msg + 1] , eax
    push bx
    push eax
    mov bl , [msg]
    movzx bx , bl 
    mov eax , [msg + 1]
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
    call  print
    mov eax , [pebp]                   ;; store the value at pebp to eax
    mov [pesp] , eax                ;; restore the stack pointer
    call pop_stack                     ;; pop stack top to eax
    mov [pebp] , eax                    ; restore the base pointer
    mov bx , [RETURN_VALUE]
    mov eax , [RETURN_VALUE  + 1]
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 12345
    push eax
    pop eax
    pop bx 
    mov [i] , bx
    mov [i + 1] , eax
    push bx
    push eax
    mov bl , [i]
    movzx bx , bl 
    mov eax , [i + 1]
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
    call  print
    mov eax , [pebp]                   ;; store the value at pebp to eax
    mov [pesp] , eax                ;; restore the stack pointer
    call pop_stack                     ;; pop stack top to eax
    mov [pebp] , eax                    ; restore the base pointer
    mov bx , [RETURN_VALUE]
    mov eax , [RETURN_VALUE  + 1]
    push bx
    push eax
    mov bl , [STRING_TYPE]
    movzx bx , bl
    push bx
    lea eax, [label_233665123]              ;; Load the address of litral into eax
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
    call  print
    mov eax , [pebp]                   ;; store the value at pebp to eax
    mov [pesp] , eax                ;; restore the stack pointer
    call pop_stack                     ;; pop stack top to eax
    mov [pebp] , eax                    ; restore the base pointer
    mov bx , [RETURN_VALUE]
    mov eax , [RETURN_VALUE  + 1]
    push bx
    push eax
    mov bl , [INT_TYPE]
    movzx bx , bl
    push bx
    mov eax, 50
    push eax
    pop eax
    pop bx 
    mov [i] , bx
    mov [i + 1] , eax
    push bx
    push eax
    mov bl , [i]
    movzx bx , bl 
    mov eax , [i + 1]
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
    call  print_series
    mov eax , [pebp]                   ;; store the value at pebp to eax
    mov [pesp] , eax                ;; restore the stack pointer
    call pop_stack                     ;; pop stack top to eax
    mov [pebp] , eax                    ; restore the base pointer
    mov bx , [RETURN_VALUE]
    mov eax , [RETURN_VALUE  + 1]
    push bx
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
;; this method is used for pushing the callers base address to stack store in eax register
push_lbase:
   sub dword [loop_base_stack_top] , 4              ;; decrement stack by 4
   mov ebx , [loop_base_stack_top]                  ;; load value of call_stack_top to ebx
   mov dword [ebx] , eax                       ;; store the value of eax to location stored in ebx
    ret
;; this method is used for poping  callers base address from stack and  store  it in eax register
pop_lbase:
   mov eax , [loop_base_stack_top]                  ;; load value of call_stack_top to eax
   mov eax , [eax]                             ;; load value from addressed stored in eax to eax
    add dword [loop_base_stack_top] , 4          ;; increment stack by 4
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
free:
    push esi            ; Save esi on the stack
    push ebp            ; Save ebp on the stack
    xor eax, eax        ; Clear eax
    mov al, 11          ; sys_munmap system call number (11)
    ; ecx should contain the address of the memory to be freed
    ; edx should contain the size of the memory region
    int 0x80            ; Call kernel to perform munmap
    pop ebp             ; Restore ebp
    pop esi             ; Restore esi
    ret
