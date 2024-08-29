section .data                        ;; Section for initialized data
    x dd 0
    call_stack_top dd 0
section .bss
   call_stack resb 4096              ;; Reserve 4096 bytes (4 KB) for the custom stack

section .text                        ;; Section for code
    global _start                    ;; Make the _start symbol available to the linker

_start: 
    mov eax , call_stack + 4096
    mov [call_stack_top] , eax        ;; init call stack 
    mov eax, 30
    push eax
    pop eax
    mov [x] , eax
    push eax
    push ebp                               ; Save base pointer
    mov ebp, esp                           ; Establish new base pointer
    sub esp , 4                            ;; allocate space for args on stack
    lea eax, [label_1804289383]              ; Load the address of litral into eax
    push eax
    pop eax
    mov [ebp - 4] , eax                   ;; args no:- 1
    call  print
    mov esp, ebp                          ;; Restore the stack pointer
    pop ebp                               ;; Restore the base pointer
; Exit the program
    mov eax, 1                       ;; syscall number for sys_exit
    xor ebx, ebx                     ;; exit code 0
    int 0x80                         ;; make syscall
;; this method is used for pushing the return address to stack store in eax register
push_call:
   sub dword [call_stack_top] , 4        ;; decrement stack by 4
   mov ebx , [call_stack_top]            ;; load value of call_stack_top to ebx
   mov dword [ebx] , eax                 ;; store the value of eax to location stored in ebx
    ret
;; this method is used for poping the return address from stack and  store  it in eax register
pop_call:
   mov eax , [call_stack_top]            ;; load value of call_stack_top to eax
   mov eax , [eax]                       ;; load value from addressed stored in eax to eax
    add dword [call_stack_top] , 4        ;; increment stack by 4
   ret
print:
    pop eax             ;; pop the return address to eax
    call push_call      ;; store the return address to ra location
    push ebp                ; Save base pointer
    mov ebp, esp            ; Establish new base pointer
    sub esp , 4
    sub esp , 4
    sub esp , 4
    mov eax, 50
    push eax
    pop eax
    mov [ebp + 0 - 4  ] , eax   ;;length
    push eax
    mov eax , [ebp + 0 - 4 ]     ;; length
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
    jnz label_846930886      ;; jump if expression  is not  zero
    lea eax , [label_1681692777]         ;; save the false label to eax
    jmp eax
label_1681692777:         ;; defination of false label 
    mov eax , [ebp + 0 - 4 ]     ;; length
    push eax
    pop eax
    mov [ebp + 0 - 8  ] , eax   ;;strlength
    push eax
    mov eax , [ebp + 4 - 4 ]     ;; x
    push eax
    pop eax
    mov [ebp + 0 - 12  ] , eax   ;;strpointer
    push eax
    pop ecx                           ; load the address of the string
    pop edx                           ; load the length of the string
    mov eax, 4                        ; syscall number for sys_write
    mov ebx, 1                        ; file descriptor 1 (stdout)
    int 0x80                          ; make syscall to write
    mov eax, 0
    push eax
    pop eax
    mov [ebp + 4 - 4  ] , eax   ;;x
    push eax
    mov eax, 50
    push eax
    pop eax
    mov [ebp + 0 - 4  ] , eax   ;;length
    push eax
    mov eax , [ebp + 4 - 4 ]     ;; x
    push eax
    mov eax, 50
    push eax
    ;; plus
    pop eax
    pop ebx
    add eax , ebx
    push eax
    mov esp, ebp            ; Restore the stack pointer
    pop ebp                 ; Restore the base pointer
    call pop_call      ;; store the return address to eax
    jmp eax             ;; jmp to return address
label_846930886:
    pop eax             ;; pop the return address to eax
    call push_call      ;; store the return address to ra location
    push ebp                ; Save base pointer
    mov ebp, esp            ; Establish new base pointer
    mov eax, 200
    push eax
    pop eax
    mov [ebp + 16 - 4  ] , eax   ;;length
    push eax
    mov esp, ebp            ; Restore the stack pointer
    pop ebp                 ; Restore the base pointer
    call pop_call      ;; store the return address to eax
    jmp eax             ;; jmp to return address
section .rodata
    label_1804289383: dd "hello world ", 0
