#include "../../include/asm/stacks.h";
#include "../../include/asm/helper.h"

void print_stakcs_operations(FILE *fp)
{

    call_stack_operations(fp);
    param_stack_operations(fp);
    function_base_stack_operations(fp);
    loop_base_stack_operations(fp);
    function_call_stack_operations(fp);
    loop_call_stack_operations(fp);
}

// void print_stack_operations_init(FILE *fp);

void call_stack_operations(FILE *fp)
{

    print_data_section(fp, "    call_stack_top dd 0\n");
    print_bss_section(fp, "    call_stack resb 1024               ;; Reserve 1024 bytes (4 KB) for the call stack\n");
    // push stack method
    fprintf(fp, ";; this method is used for pushing the return address to stack store in eax register\n");
    fprintf(fp, "push_call:\n");
    fprintf(fp, "   sub dword [call_stack_top] , 4              ;; decrement stack by 4\n");
    fprintf(fp, "   mov ebx , [call_stack_top]                  ;; load value of call_stack_top to ebx\n");
    fprintf(fp, "   mov dword [ebx] , eax                       ;; store the value of eax to location stored in ebx\n ");
    fprintf(fp, "   ret\n");

    // pop stack method
    fprintf(fp, ";; this method is used for poping the return address from stack and  store  it in eax register\n");
    fprintf(fp, "pop_call:\n");
    fprintf(fp, "   mov eax , [call_stack_top]                  ;; load value of call_stack_top to eax\n");
    fprintf(fp, "   mov eax , [eax]                             ;; load value from addressed stored in eax to eax\n ");
    fprintf(fp, "   add dword [call_stack_top] , 4          ;; increment stack by 4\n");
    fprintf(fp, "   ret\n");

    return;
}

void function_base_stack_operations(FILE *fp)
{
    print_data_section(fp, "    base_stack_top dd 0\n");
    print_bss_section(fp, "    base_stack resb 1024              ;; Reserve 1024 bytes (4 KB) for the param stack\n");
    // push stack method
    fprintf(fp, ";; this method is used for pushing the callers base address to stack store in eax register\n");
    fprintf(fp, "push_base:\n");
    fprintf(fp, "   sub dword [base_stack_top] , 4              ;; decrement stack by 4\n");
    fprintf(fp, "   mov ebx , [base_stack_top]                  ;; load value of call_stack_top to ebx\n");
    fprintf(fp, "   mov dword [ebx] , eax                       ;; store the value of eax to location stored in ebx\n ");
    fprintf(fp, "   ret\n");

    // pop stack method
    fprintf(fp, ";; this method is used for poping  callers base address from stack and  store  it in eax register\n");
    fprintf(fp, "pop_base:\n");
    fprintf(fp, "   mov eax , [base_stack_top]                  ;; load value of call_stack_top to eax\n");
    fprintf(fp, "   mov eax , [eax]                             ;; load value from addressed stored in eax to eax\n ");
    fprintf(fp, "   add dword [base_stack_top] , 4          ;; increment stack by 4\n");
    fprintf(fp, "   ret\n");

    return;
}

void loop_base_stack_operations(FILE *fp)
{
    print_data_section(fp, "    loop_base_stack_top dd 0\n");
    print_bss_section(fp, "    loop_base_stack resb 1024              ;; Reserve 1024 bytes (4 KB) for the param stack\n");
    // push stack method
    fprintf(fp, ";; this method is used for pushing the callers base address to stack store in eax register\n");
    fprintf(fp, "push_lbase:\n");
    fprintf(fp, "   sub dword [loop_base_stack_top] , 4              ;; decrement stack by 4\n");
    fprintf(fp, "   mov ebx , [loop_base_stack_top]                  ;; load value of call_stack_top to ebx\n");
    fprintf(fp, "   mov dword [ebx] , eax                       ;; store the value of eax to location stored in ebx\n ");
    fprintf(fp, "   ret\n");

    // pop stack method
    fprintf(fp, ";; this method is used for poping  callers base address from stack and  store  it in eax register\n");
    fprintf(fp, "pop_lbase:\n");
    fprintf(fp, "   mov eax , [loop_base_stack_top]                  ;; load value of call_stack_top to eax\n");
    fprintf(fp, "   mov eax , [eax]                             ;; load value from addressed stored in eax to eax\n ");
    fprintf(fp, "   add dword [loop_base_stack_top] , 4          ;; increment stack by 4\n");
    fprintf(fp, "   ret\n");

    return;
}

void function_call_stack_operations(FILE *fp)
{
    print_data_section(fp, "    fcall_stack_top dd 0\n");
    print_bss_section(fp, "    fcall_stack resb 1024               ;; Reserve 1024 bytes (4 KB) for the call stack\n");
    // push stack method
    fprintf(fp, ";; this method is used for pushing the callers return address to stack store in eax register\n");
    fprintf(fp, "push_fcall:\n");
    fprintf(fp, "   sub dword [fcall_stack_top] , 4              ;; decrement stack by 4\n");
    fprintf(fp, "   mov ebx , [fcall_stack_top]                  ;; load value of call_stack_top to ebx\n");
    fprintf(fp, "   mov dword [ebx] , eax                       ;; store the value of eax to location stored in ebx\n ");
    fprintf(fp, "   ret\n");

    // pop stack method
    fprintf(fp, ";; this method is used for poping the callers return address from stack and  store  it in eax register\n");
    fprintf(fp, "pop_fcall:\n");
    fprintf(fp, "   mov eax , [fcall_stack_top]                  ;; load value of call_stack_top to eax\n");
    fprintf(fp, "   mov eax , [eax]                             ;; load value from addressed stored in eax to eax\n ");
    fprintf(fp, "   add dword [fcall_stack_top] , 4          ;; increment stack by 4\n");
    fprintf(fp, "   ret\n");

    return;
}

void loop_call_stack_operations(FILE *fp)
{

    print_data_section(fp, "    lcall_stack_top dd 0\n");
    print_bss_section(fp, "    lcall_stack resb 1024           ;; Reserve 1024 bytes (4 KB) for the param stack\n");
    // push stack method
    fprintf(fp, ";; this method is used for pushing the loop return address to stack store in eax register\n");
    fprintf(fp, "push_lcall:\n");
    fprintf(fp, "   sub dword [lcall_stack_top] , 4              ;; decrement stack by 4\n");
    fprintf(fp, "   mov ebx , [lcall_stack_top]                  ;; load value of call_stack_top to ebx\n");
    fprintf(fp, "   mov dword [ebx] , eax                       ;; store the value of eax to location stored in ebx\n ");
    fprintf(fp, "   ret\n");

    // pop stack method
    fprintf(fp, ";; this method is used for poping the callers return address from stack and  store  it in eax register\n");
    fprintf(fp, "pop_lcall:\n");
    fprintf(fp, "   mov eax , [lcall_stack_top]                  ;; load value of call_stack_top to eax\n");
    fprintf(fp, "   mov eax , [eax]                             ;; load value from addressed stored in eax to eax\n ");
    fprintf(fp, "   add dword [lcall_stack_top] , 4          ;; increment stack by 4\n");
    fprintf(fp, "   ret\n");

    return;
}

void param_stack_operations(FILE *fp)
{
    print_data_section(fp, "    pesp dd 0\n");
    print_data_section(fp, "    pebp dd 0\n");
    print_bss_section(fp, "    param_stack resb 1024              ;; Reserve 1024 bytes (4 KB) for the param stack\n");

    // push stack method
    fprintf(fp, ";; this method is used for pushing the return address to stack store in eax register\n");
    fprintf(fp, "push_stack:\n");
    fprintf(fp, "   sub dword [pesp] , 4                         ;; decrement stack by 4\n");
    fprintf(fp, "   mov ebx , [pesp]                             ;; load value of stack_top to ebx\n");
    fprintf(fp, "   mov dword [ebx] , eax                       ;; store the value of eax to location stored in ebx\n ");
    fprintf(fp, "   ret\n");

    // pop stack method
    fprintf(fp, ";; this method is used for poping the return address from stack and  store  it in eax register\n");
    fprintf(fp, "pop_stack:\n");
    fprintf(fp, "   mov eax , [pesp]                             ;; load value of call_stack_top to eax\n");
    fprintf(fp, "   mov eax , [eax]                             ;; load value from addressed stored in eax to eax\n ");
    fprintf(fp, "   add dword [pesp] , 4          ;; increment stack by 4\n");
    fprintf(fp, "   ret\n");

    return;
}
