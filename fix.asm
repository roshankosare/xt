section .data
    x dd 0

section .text
    global _start

_start:
    ; Initial setup
    mov eax, 10
    push eax
    pop eax
    mov [x], eax
    push eax
    mov eax, [x]
    push eax
    mov eax, 10
    push eax
    pop eax
    pop ebx
    cmp ebx, eax
    setg al
    movzx eax, al
    push eax
    pop eax
    test eax, eax

    ; Push the address of the instruction after `jnz` using a `CALL` instruction
    call next_instruction

    ; Conditional jump to label
    jnz .label_1804289383  

    ; The address of this instruction will be on the stack due to `CALL`
next_instruction:
    ret

.label_1804289383:
    ; Code to execute if `jnz` jumps
    ; Perform operations for the label

    ; Return to the address that was saved on the stack
    ret  ; Return to the instruction after `jnz`
