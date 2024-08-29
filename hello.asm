section .data                        ;; Section for initialized data
    x dd 0
    call_stack_top dd 0
    pesp dd 0
    pebp dd 0
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
    mov eax, 30
    push eax
    pop eax
    mov [x] , eax
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
    mov eax, 50
    push eax
    pop eax
   mov ebx , [pebp]                    ;; store the address to ebx
   mov [ebx + (-4)] , eax              ;; store the value at location ebx
    push eax
    mov eax , [pebp]
              ;; load the address stored in pebp to eax
    mov eax , [eax + (-4) ]     ;; length
    push eax
    mov eax, 100
    push eax
    pop eax
    pop ebx
    cmp ebx , eax
    setl al
    movzx eax , al
    push eax
    pop eax
    test eax , eax
    lea eax , [label_1681692777]
    push eax
    jnz label_846930886                 ;; jump if expression  is not  zero
    lea eax , [label_1681692777]         ;; save the false label to eax
    jmp eax
label_1681692777:                        ;; defination of false label 
    mov eax , [pebp]
              ;; load the address stored in pebp to eax
    mov eax , [eax + (-4) ]     ;; length
    push eax
    pop eax
   mov ebx , [pebp]                    ;; store the address to ebx
   mov [ebx + (-8)] , eax              ;; store the value at location ebx
    push eax
    mov eax , [pebp]
              ;; load the address stored in pebp to eax
    mov eax , [eax + (4) ]     ;; x
    push eax
    pop eax
   mov ebx , [pebp]                    ;; store the address to ebx
   mov [ebx + (-12)] , eax              ;; store the value at location ebx
    push eax
    pop ecx                           ; load the address of the string
    pop edx                           ; load the length of the string
    mov eax, 4                        ; syscall number for sys_write
    mov ebx, 1                        ; file descriptor 1 (stdout)
    int 0x80                          ; make syscall to write
    mov eax, 0
    push eax
    pop eax
   mov ebx , [pebp]                    ;; store the address to ebx
   mov [ebx + (4)] , eax              ;; store the value at location ebx
    push eax
    mov eax, 50
    push eax
    pop eax
   mov ebx , [pebp]                    ;; store the address to ebx
   mov [ebx + (-4)] , eax              ;; store the value at location ebx
    push eax
    mov eax , [pebp]
              ;; load the address stored in pebp to eax
    mov eax , [eax + (4) ]     ;; x
    push eax
    mov eax, 50
    push eax
    ;; plus
    pop eax
    pop ebx
    add eax , ebx
    push eax
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
    mov eax, 5
    push eax
    pop eax
   mov ebx , [pebp]                    ;; store the address to ebx
   mov [ebx + (8)] , eax              ;; store the value at location ebx
    push eax
    mov eax , [pebp]                   ;; store the value at pebp to eax
    mov [pesp] , eax                ;; restore the stack pointer
    call pop_stack                     ;; pop stack top to eax
    mov [pebp] , eax                    ; restore the base pointer
    call pop_call                  ;; store the return address to eax
    jmp eax                        ;; jmp to return address
section .rodata
    label_1804289383: dd "hello world ", 0
