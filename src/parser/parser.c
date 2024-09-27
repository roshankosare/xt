#include <stdio.h>
#include <stdlib.h>

#include "../../include/parser/ast.h"
#include "../../include/symboltable/functiontable.h"
#include "../../include/symboltable/symboltable.h"
#include "../../include/context/context.h"
#include "../../include/parser/program.h"
#include "../../include/parser/helper.h"
#include "../../include/tokens/tokenizer.h"
#include "../../include/lexer/lexer.h"

void parseProgram(Context *context, FILE *fp)
{
    fillEntrys(context->tokenTable);
    if (compile_token_patterns())
    {

        fprintf(stderr, "Failed to compile token patterns\n");
        exit(-1);
    }
    getCurrentToken(context);

    fprintf(fp, "section .data                        ;; Section for initialized data\n");
    fprintf(fp, "    condition dd 5 dup(0)\n");
    fprintf(fp, "    RETURN_VALUE dd 5 dup(0)\n");
    fprintf(fp, "    RETURN_ADDRESS dd 0\n");
    fprintf(fp, "    POPED_ADDRESS dd 0\n");

    fprintf(fp, "section .bss\n");
    fprintf(fp, "\n");

    fprintf(fp, "section .rodata\n");
    fprintf(fp, "    INT_TYPE: db 1\n");
    fprintf(fp, "    FLOAT_TYPE: db 2\n");
    fprintf(fp, "    STRING_TYPE: db 3\n");
    fprintf(fp, "    REF_TYPE: db 4\n");
    fprintf(fp, "section .text                        ;; Section for code\n");
    fprintf(fp, "    global _start                    ;; Make the _start symbol available to the linker\n");
    fprintf(fp, "\n");
    fprintf(fp, "_start: \n");
    fprintf(fp, "    mov eax , call_stack + 1024\n");
    fprintf(fp, "    mov [call_stack_top] , eax                 ;; init call stack \n");

    fprintf(fp, "    mov eax , fcall_stack + 1024\n");
    fprintf(fp, "    mov [fcall_stack_top] , eax                 ;; init call stack \n");

    fprintf(fp, "    mov eax , base_stack + 1024\n");
    fprintf(fp, "    mov [base_stack_top] , eax                 ;; init call stack \n");

    fprintf(fp, "    mov eax , lcall_stack + 1024\n");
    fprintf(fp, "    mov [lcall_stack_top] , eax\n");

    fprintf(fp, "    mov eax , loop_base_stack + 1024\n");
    fprintf(fp, "    mov [loop_base_stack_top] , eax\n");

    fprintf(fp, "    mov eax , param_stack + 1024\n");
    fprintf(fp, "    mov [pesp] , eax\n");

    fprintf(fp, "    mov [pebp] , eax\n");

    int count = 0;
    while (context->current.value != TEOF)
    {
        program(context, fp);
    }

    fprintf(fp, "; Exit the program\n");
    fprintf(fp, "    mov eax, 1                       ;; syscall number for sys_exit\n");
    fprintf(fp, "    xor ebx, ebx                     ;; exit code 0\n");
    fprintf(fp, "    int 0x80                         ;; make syscall\n");

    print_stakcs_operations(fp);
    print_system_functions(fp);
}

void printAST(ASTNode *node, int depth)
{

    if (node == NULL)
    {
        return;
    }

    // Print indentation
    for (int i = 0; i < depth; ++i)
    {
        printf("  ");
    }

    // Print the current node's token
    printf("%s -> %s", node->token.lexeme, getTokenStringValue(node->token.value));
    printf("\n");

    // Recursively print the left and right children
    if (node->left)
    {

        printAST(node->left, depth + 1);
    }
    if (node->right)
    {

        printAST(node->right, depth + 1);
    }
    if (node->next)
    {
        printAST(node->next, depth);
    }
}