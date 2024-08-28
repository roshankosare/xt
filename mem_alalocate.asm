section .text
    global _start   ; Make the _start symbol available to the linker

_start:
    ; Allocate memory using mmap
    ; Parameters:
    ;   - addr = 0 (let the kernel choose the address)
    ;   - length = 4 (size of one integer)
    ;   - prot = PROT_READ | PROT_WRITE (read and write permissions)
    ;   - flags = MAP_PRIVATE | MAP_ANONYMOUS (private and anonymous mapping)
    ;   - fd = -1 (not used)
    ;   - offset = 0 (not used)
    mov eax, 192     ; syscall number for mmap (on x86-64, it’s 9)
    xor ebx, ebx     ; addr = 0
    mov ecx, 4       ; length = 4 (size of one integer)
    mov edx, 3       ; prot = PROT_READ | PROT_WRITE
    mov esi, 0x22    ; flags = MAP_PRIVATE | MAP_ANONYMOUS
    mov edi, -1      ; fd = -1
    xor ebp, ebp     ; offset = 0
    int 0x80         ; make syscall
    mov [mem], eax   ; Store the address of the allocated memory in mem

    ; Initialize the allocated memory (store value 10)
    mov dword [eax], 10  ; Store the initial value 10 at the allocated memory

    ; Load value of x from the allocated memory into eax
    mov eax, [mem]       ; Load the address of the allocated memory
    mov eax, [eax]       ; Load the value (10) from memory into eax

    ; Add 20 to the value
    add eax, 20          ; eax = eax + 20 (eax now has 30)

    ; Store the result back into the allocated memory
    mov [mem], eax       ; Store the result (30) back into the allocated memory

    ; Exit the program
    mov eax, 1           ; syscall number for sys_exit
    xor ebx, ebx         ; exit code 0
    int 0x80             ; make syscall

section .bss
    mem resd 1           ; Reserve space for storing the memory address
