#include "../../include/asm/system.h"
#include "../../include/asm/helper.h"

void print_system_functions(FILE *fp)
{
    print_eax(fp);
    printDelay(fp);
    printMemAlloc(fp);
    printFreeMem(fp);
}

void print_eax(FILE *fp)
{
    print_data_section(fp, "    buffer db '0000000000', 0       ; Buffer to hold the converted number (10 digits max)\n");
    print_data_section(fp, "    len equ 10                      ; Length of the buffer\n");
    print_data_section(fp, "    space db  \" \"                       ; Space character to print\n");

    fprintf(fp, "print_eax:\n");
    fprintf(fp, "    ; Convert number in eax to string\n");
    fprintf(fp, "    mov edi, len                    ; Point to the end of the buffer\n");
    fprintf(fp, "    mov ecx, 10                     ; Base 10 for division\n");
    fprintf(fp, "    lea esi, [buffer + len]         ; Point esi to the last digit in buffer\n");
    fprintf(fp, ".print_loop:\n");
    fprintf(fp, "    xor edx, edx                    ; Clear edx for division\n");
    fprintf(fp, "    div ecx                         ; Divide eax by 10, quotient in eax, remainder in edx\n");
    fprintf(fp, "    add dl, '0'                     ; Convert remainder to ASCII\n");
    fprintf(fp, "    dec esi                         ; Move the buffer pointer backwards\n");
    fprintf(fp, "    mov [esi], dl                   ; Store the ASCII character in buffer\n");
    fprintf(fp, "    dec edi                         ; Decrease digit count\n");
    fprintf(fp, "    test eax, eax                   ; Check if eax is 0\n");
    fprintf(fp, "    jnz .print_loop                 ; If not, continue the loop\n");
    fprintf(fp, "    ; Check if the number was zero\n");
    fprintf(fp, "    cmp edi, len\n");
    fprintf(fp, "    jne .not_zero\n");
    fprintf(fp, "    mov byte [esi - 1], '0'         ; Handle case where eax was 0\n");
    fprintf(fp, "    dec esi\n");
    fprintf(fp, ".not_zero:\n");
    fprintf(fp, "    ; Print the string\n");
    fprintf(fp, "    mov eax, 4                      ; sys_write system call number\n");
    fprintf(fp, "    mov ebx, 1                      ; File descriptor 1 (stdout)\n");
    fprintf(fp, "    mov edx, len                    ; Number of bytes to write\n");
    fprintf(fp, "    sub edx, edi                    ; Adjust the length to the actual number length\n");
    fprintf(fp, "    lea ecx, [buffer + edi]         ; Adjust buffer pointer to start of the number string\n");
    fprintf(fp, "    int 0x80                        ; Interrupt to make system call\n");

    fprintf(fp, "    ; Print a space after the number\n");
    fprintf(fp, "    mov eax, 4                      ; sys_write system call number\n");
    fprintf(fp, "    mov ebx, 1                      ; File descriptor 1 (stdout)\n");
    fprintf(fp, "    mov ecx, space                  ; Address of the space character\n");
    fprintf(fp, "    mov edx, 1                      ; Length of the space character (1 byte)\n");
    fprintf(fp, "    int 0x80                        ; Interrupt to make system call\n");
    fprintf(fp, "    ret                             ; Return to caller\n");
}

void printDelay(FILE *fp)
{

    fprintf(fp, "delay:\n");
    fprintf(fp, "    ; Assumes the delay duration is passed in ecx\n");
    fprintf(fp, "    mov eax, ecx          ; Load the delay duration into eax\n");
    fprintf(fp, ".loop:\n");
    fprintf(fp, "    dec eax               ; Decrement eax\n");
    fprintf(fp, "    jnz .loop             ; If eax is not zero, repeat the loop\n");
    fprintf(fp, "    ret                   ; Return to the caller\n");

    return;
}

void printMemAlloc(FILE *fp)
{

    fprintf(fp, "malloc:\n");
    fprintf(fp, "     push esi           ; Save esi on the stack\n");
    fprintf(fp, "     push ebp           ; Save ebp on the stack\n");
    fprintf(fp, "     xor eax, eax       ; Clear eax\n");
    fprintf(fp, "     mov al, 192        ; sys_mmap system call number (9)\n");
    fprintf(fp, "     xor ebx, ebx       ; addr = NULL (let kernel choose the address)\n");
    fprintf(fp, "     mov edx, 3         ; prot = PROT_READ | PROT_WRITE (read/write access)\n");
    fprintf(fp, "     mov esi, 0x22      ; flags = MAP_PRIVATE | MAP_ANONYMOUS\n");
    fprintf(fp, "     mov edi, -1        ; fd = -1 (no file descriptor since MAP_ANONYMOUS)\n");
    fprintf(fp, "     xor ebp, ebp       ; offset = 0 (not applicable since no file is mapped)\n");
    fprintf(fp, "     ; At this point, ecx contains the size for mmap, which should be passed as a parameter\n");
    fprintf(fp, "     int 0x80           ; Call kernel to perform mmap\n");
    fprintf(fp, "     pop ebp            ; Restore ebp\n");
    fprintf(fp, "     pop esi            ; Restore esi\n");
    fprintf(fp, "     ret\n");
}

void printFreeMem(FILE *fp)
{

    fprintf(fp, "free:\n");
    fprintf(fp, "    push esi            ; Save esi on the stack\n");
    fprintf(fp, "    push ebp            ; Save ebp on the stack\n");
    fprintf(fp, "    xor eax, eax        ; Clear eax\n");
    fprintf(fp, "    mov al, 11          ; sys_munmap system call number (11)\n");
    fprintf(fp, "    ; ecx should contain the address of the memory to be freed\n");
    fprintf(fp, "    ; edx should contain the size of the memory region\n");
    fprintf(fp, "    int 0x80            ; Call kernel to perform munmap\n");
    fprintf(fp, "    pop ebp             ; Restore ebp\n");
    fprintf(fp, "    pop esi             ; Restore esi\n");
    fprintf(fp, "    ret\n");
}