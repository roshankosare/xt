#include "../../include/asm/asm.h"
#include "../../include/parser/helper.h"
#include "../../include/symboltable/functiontable.h"
#include "../../include/symboltable/symboltable.h"
#include "../../include/litrals/litrals.h"
#include "../../include/asm/asmcontext.h"
#include "../../include/asm/stacks.h"
#include <assert.h>
#include <string.h>

void translateGlobalVar(ASTNode *ast, FILE *fp);
void generateLabels(Context *context, FILE *fp);
void tranlateLocalVar(ASTNode *ast, FILE *fp);
void createLitralConstansts(Context *Context, FILE *fp);
void print_eax(FILE *fp);
void printDelay(FILE *fp);
void printMemAlloc(FILE *fp);
void printFreeMem(FILE *fp);
void createASMFile(ASTNode *ast, Context *context, FILE *fp)
{
    fprintf(fp, "section .data                        ;; Section for initialized data\n");
    translateGlobalVar(ast, fp);

    fprintf(fp, "    buffer db '0000000000', 0       ; Buffer to hold the converted number (10 digits max)\n");
    fprintf(fp, "    len equ 10                      ; Length of the buffer\n");
    fprintf(fp, "    space db  \" \"                       ; Space character to print\n");
    fprintf(fp, "    condition dd 5 dup(0)\n");
    fprintf(fp, "    BASE_ESP dd 0\n");
    fprintf(fp, "    BASE_RETURN dd 0\n");
    fprintf(fp, "    RETURN_VALUE dd 5 dup(0)\n");
    fprintf(fp, "    RETURN_ADDRESS dd 0\n");
    fprintf(fp, "    POPED_ADDRESS dd 0\n");
    fprintf(fp, "    NUM_REF_ALLO dd 0\n");
    // fprintf(fp, "    REF_LIST_TO:\n");
    // fprintf(fp, "        size db 0\n");
    // fprintf(fp, "        alloc_num db 0\n");
    // fprintf(fp, "        address_value dd 0\n");
    // fprintf(fp, "        next dd 0\n");

    fprintf(fp, "section .bss\n");
    fprintf(fp, "\n");

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
    translate(ast, context, fp); // translate code;
    fprintf(fp, "; Exit the program\n");

    fprintf(fp, "    mov eax, 1                       ;; syscall number for sys_exit\n");
    fprintf(fp, "    xor ebx, ebx                     ;; exit code 0\n");
    fprintf(fp, "    int 0x80                         ;; make syscall\n");

    print_stakcs_operations(fp);
    print_eax(fp);
    printDelay(fp);
    printMemAlloc(fp);

    generateLabels(context, fp);

    fprintf(fp, "section .rodata\n");
    fprintf(fp, "    INT_TYPE: db 1\n");
    fprintf(fp, "    FLOAT_TYPE: db 2\n");
    fprintf(fp, "    STRING_TYPE: db 3\n");
    fprintf(fp, "    REF_TYPE: db 4\n");
    createLitralConstansts(context, fp);
}

void translateGlobalVar(ASTNode *ast, FILE *fp)
{
    if (ast == NULL)
    {
        return;
    }
    if (ast->token.value == UNKNOWN)
    {
        translateGlobalVar(ast->right, fp);
    }
    if (ast->token.value == VAR)
    {
        if (ast->right->token.value == ASSIGN)
        {
            fprintf(fp, "    %s dd 5 dup(0)\n", ast->right->left->token.lexeme);
        }
        else
        {
            fprintf(fp, "    %s dd 5 dup(0)\n", ast->right->token.lexeme);
        }
        // for int
    }
    translateGlobalVar(ast->next, fp);
}

void tranlateLocalVar(ASTNode *ast, FILE *fp)
{
    if (ast == NULL)
    {
        return;
    }
    if (ast->token.value == UNKNOWN)
    {
        tranlateLocalVar(ast->right, fp);
    }
    if (ast->token.value == VAR)
    {
        if (ast->right->token.value == ASSIGN)
        {
            fprintf(fp, "    sub dword [pesp] , 5\n");
            // fprintf(fp, "    sub esp , 4\n");
        }
        else
        {
            fprintf(fp, "    sub dword [pesp] , 5\n");
            // fprintf(fp, "    sub esp , 4\n");
        }
        // for int
    }
    tranlateLocalVar(ast->next, fp);
}

void createLitralConstansts(Context *context, FILE *fp)
{

    Litral *entry;
    entry = popLitlralTable(context->litralTable);
    while (entry != NULL)
    {
        fprintf(fp, "    %s: db %s, 0\n", entry->label, entry->value);
        entry = popLitlralTable(context->litralTable);
    }
}

void translate(ASTNode *ast, Context *context, FILE *fp)
{
    if (ast == NULL)
    {
        return;
    }
    switch (ast->token.value)
    {
    case UNKNOWN:
        translate(ast->right, context, fp);
        break;
    case PLUS:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        fprintf(fp, "    ;; plus\n");
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop cx \n");
        fprintf(fp, "    pop ebx\n");
        fprintf(fp, "    pop dx \n");
        fprintf(fp, "    add eax , ebx\n");
        fprintf(fp, "    push cx\n");
        fprintf(fp, "    push eax\n");
        break;
    case MUL:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        fprintf(fp, "    ;; MUL\n");
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop cx \n");
        fprintf(fp, "    pop ebx\n");
        fprintf(fp, "    pop dx \n");
        fprintf(fp, "    imul eax , ebx\n");
        fprintf(fp, "    push cx\n");
        fprintf(fp, "    push eax\n");
        break;
    case DIV:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        fprintf(fp, "    ;; DIV\n");
        fprintf(fp, "    pop ebx\n");
        fprintf(fp, "    pop cx \n");
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop cx \n");
        fprintf(fp, "    cdq\n");
        fprintf(fp, "    idiv ebx\n");
        fprintf(fp, "    push cx\n");
        fprintf(fp, "    push eax\n");
        break;
    case MINUS:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        fprintf(fp, "    ;; MINUS\n");
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop cx \n");
        fprintf(fp, "    pop ebx\n");
        fprintf(fp, "    pop dx \n");
        fprintf(fp, "    sub eax , ebx\n");
        fprintf(fp, "    push cx\n");
        fprintf(fp, "    push eax\n");
        break;
    case MODULAS:
    {
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        fprintf(fp, "    pop ebx\n");
        fprintf(fp, "    pop cx \n");
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop cx \n");
        // fprintf(fp, "    cdq\n");
        fprintf(fp, "    xor edx ,edx\n");
        fprintf(fp, "    div ebx\n");
        fprintf(fp, "    push cx\n");
        fprintf(fp, "    push edx\n");
        break;
    }
    case ASSIGN:
    {
        translate(ast->right, context, fp); // convert expression to asm which will store value of evaluated expresion to stack
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop bx \n");
        // fprintf(fp, "    mov ebx, eax\n");
        // fprintf(fp, "    push ebx\n");
        // fprintf(fp, "    call print_eax\n");
        // fprintf(fp, "    pop ebx\n");
        // fprintf(fp, "    mov eax , ebx\n");
        if (ast->left->token.value == VALUE_AT)
        {

            SymbolTableEntry *entry = checkSymbolEntry(context, ast->left->left->token);

            if (entry->scope == 1)
            {
                // this is global var
                fprintf(fp, "    mov ebx , [%s + 1]\n", ast->left->left->token.lexeme);
                fprintf(fp, "    mov [ebx] , eax\n");
                fprintf(fp, "    push eax\n"); // store the to address of identifier
            }
            else
            {
                int offset = getSymbolOffset(context, entry);

                fprintf(fp, "    mov ebx , [pebp]                    \n");
                fprintf(fp, "    mov ebx , [ebx + (%d) + 1]           \n", offset);
                fprintf(fp, "    mov [ebx] , eax   ;;%s\n", entry->token.lexeme);
                fprintf(fp, "    push eax\n");
            }
            break;
        }
        SymbolTableEntry *entry = checkSymbolEntry(context, ast->left->token);
        if (entry->scope == 1)
        {
            fprintf(fp, "    mov [%s] , bx\n", ast->left->token.lexeme);
            fprintf(fp, "    mov [%s + 1] , eax\n", ast->left->token.lexeme);

            fprintf(fp, "    push bx\n");
            fprintf(fp, "    push eax\n"); // store the to address of identifier
        }
        else
        {
            int offset = getSymbolOffset(context, entry);
            fprintf(fp, "    mov ecx , [pebp]                   \n");
            fprintf(fp, "    mov [ecx + (%d)] , bx              \n", offset);
            fprintf(fp, "    mov [ecx + (%d) + 1] , eax\n", offset);
            fprintf(fp, "    push bx\n");
            fprintf(fp, "    push eax\n");
        }
    }
    break;

    case INC:
    {
        if (ast->left != NULL)
        {
            SymbolTableEntry *entry = checkSymbolEntry(context, ast->left->token);
            if (entry->scope == 1)
            { // this is global var
                fprintf(fp, "     inc dword [%s+1]\n", ast->left->token.lexeme);
                translate(ast->left, context, fp);
                // store the value of identifer to eax
            }
            else
            {
                int offset = getSymbolOffset(context, entry);
                fprintf(fp, "    mov eax , [pebp]           ;; move address stored in pebp to eax\n");
                fprintf(fp, "    inc dword [eax + (%d) + 1]     ;; %s\n", offset, entry->token.lexeme);
                translate(ast->left, context, fp);
            }
            break;
        }

        if (ast->right != NULL)
        {
            SymbolTableEntry *entry = checkSymbolEntry(context, ast->right->token);
            if (entry->scope == 1)
            { // this is global var
                translate(ast->right, context, fp);
                fprintf(fp, "     inc dword [%s + 1]\n", ast->right->token.lexeme);

                // store the value of identifer to eax
            }
            else
            {
                int offset = getSymbolOffset(context, entry);
                translate(ast->right, context, fp);
                fprintf(fp, "    mov eax , [pebp]           ;; move address stored in pebp to eax\n");
                fprintf(fp, "    inc dword [eax + (%d) + 1]     ;; %s\n", offset, entry->token.lexeme);
                // fprintf(fp, "    inc dword [ebp + (%d)]     ;; %s\n", offset, entry->token.lexeme);
            }
            break;
        }
    }

    case DEC:
    {
        if (ast->left != NULL)
        {
            SymbolTableEntry *entry = checkSymbolEntry(context, ast->left->token);
            if (entry->scope == 1)
            { // this is global var
                fprintf(fp, "     dec dword [%s + 1]\n", ast->left->token.lexeme);
                translate(ast->left, context, fp);
                // store the value of identifer to eax
            }
            else
            {
                int offset = getSymbolOffset(context, entry);
                fprintf(fp, "    mov eax , [pebp] \n");
                fprintf(fp, "    dec dword [eax + (%d) + 1 ]     ;; %s\n", offset, entry->token.lexeme);
                translate(ast->left, context, fp);
            }
            break;
        }

        if (ast->right != NULL)
        {
            SymbolTableEntry *entry = checkSymbolEntry(context, ast->right->token);
            if (entry->scope == 1)
            { // this is global var
                translate(ast->right, context, fp);
                fprintf(fp, "     dec dword [%s + 1]\n", ast->right->token.lexeme);

                // store the value of identifer to eax
            }
            else
            {
                int offset = getSymbolOffset(context, entry);
                translate(ast->right, context, fp);
                fprintf(fp, "    mov eax , [pebp] \n");
                fprintf(fp, "    dec dword [eax + (%d) + 1  ]     ;; %s\n", offset, entry->token.lexeme);
            }
            break;
        }
    }
    case VAR:
    {
        if (ast->right->token.value == ASSIGN) // decleration with assignment
        {

            SymbolTableEntry *entry = checkSymbolEntry(context, ast->right->left->token);
            translate(ast->right, context, fp);
            break;
        }
    }

    case TYPEOF:
    {
        translate(ast->left, context, fp);
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop bx\n");
        fprintf(fp, "    movzx eax , bx \n");
        fprintf(fp, "    mov bx , [INT_TYPE]\n");
        fprintf(fp, "    push bx\n");
        fprintf(fp, "    push eax\n");
    }
    break;
    case IF:
        translate(ast->left, context, fp);
        // prase  condition first
        char *label_true = label_generate(); // generate label for  this if block
        char *label_false = label_generate();
        fprintf(fp, "    pop eax\n");        // get value of expression from stack
        fprintf(fp, "    test eax , eax\n"); // check if value is non zero
        fprintf(fp, "    lea eax , [.%s]\n", label_false);
        fprintf(fp, "    push eax\n");
        fprintf(fp, "    jnz %s                 ;; jump if expression  is not  zero\n", label_true);
        pushASTQnodeInQueue(context->astQueue, ast->right, label_true);
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    lea eax , [.%s]         ;; save the false label to eax\n", label_false);
        fprintf(fp, "    jmp eax\n");
        fprintf(fp, ".%s:                        ;; defination of false label \n", label_false);
        break;
    case ELSE:
        assert(0 && "TODO: ELSE is not implemented");
    case FOR:
        assert(0 && "TODO: FOR is not implemented");
    case RETURN:
    {
        fprintf(fp, "    mov  byte [RETURN_VALUE] , 0\n");
        fprintf(fp, "    mov  dword [RETURN_VALUE + 1] , 0\n");
        if (ast->left != NULL)
        {
            translate(ast->left, context, fp);
            fprintf(fp, "    pop eax\n");
            fprintf(fp, "    pop bx\n");
            fprintf(fp, "    mov [RETURN_VALUE] , bl\n");
            fprintf(fp, "    mov [RETURN_VALUE + 1] , eax\n");
        }
        char *label_loop = label_generate();
        fprintf(fp, "    call pop_base\n"); // stack pointer
        fprintf(fp, "    mov [pesp] , eax\n");
        fprintf(fp, "    call pop_base\n"); // base pointer
        fprintf(fp, "    mov [pebp] , eax \n");
        fprintf(fp, "    call pop_fcall\n");
        fprintf(fp, "    mov [RETURN_ADDRESS], eax\n");
        fprintf(fp, ".%s:                                 ;; function to clean up call stack \n", label_loop);
        fprintf(fp, "    call pop_call\n");
        fprintf(fp, "    mov [POPED_ADDRESS] , eax\n");
        fprintf(fp, "    mov eax ,[POPED_ADDRESS]\n");
        fprintf(fp, "    cmp eax, [RETURN_ADDRESS]\n");
        fprintf(fp, "    jne .%s\n", label_loop);
        fprintf(fp, "    mov eax , [RETURN_ADDRESS]\n");
        fprintf(fp, "    jmp eax\n");
        break;
    }

    case WHILE:
    {
        char *label_true = label_generate(); // generate label for  this if block
        char *label_false = label_generate();

        translate(ast->left, context, fp);

        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop bx\n");
        fprintf(fp, "    mov [condition] , bx\n");
        fprintf(fp, "    mov [condition + 1] , eax\n");

        fprintf(fp, "    lea eax , [.%s]\n", label_false);
        fprintf(fp, "    push eax\n");
        // fprintf(fp, "    call print_eax\n");
        fprintf(fp, "    mov eax , [condition + 1]\n");
        fprintf(fp, "    test eax , eax\n");
        fprintf(fp, "    jnz %s\n", label_true);
        pushASTQnodeInQueue(context->astQueue, ast, label_true);
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    lea eax , [.%s]         ;; save the false label to eax\n", label_false);
        fprintf(fp, "    jmp eax\n");
        fprintf(fp, ".%s:                        ;; defination of false label \n", label_false);
        // fprintf(fp, "    mov dword eax , 10\n");
        // fprintf(fp, "    call print_eax\n");

        // assert(0 && "TODO: WHILE is not implemented");
    }
    break;

    case CONTINUE:
    {
        fprintf(fp, "    call pop_lbase\n");
        fprintf(fp, "    mov [pesp] , eax\n");
        fprintf(fp, "    call pop_lbase\n");
        fprintf(fp, "    mov [pebp] , eax\n");

        // fprintf(fp, "    lea eax , [%s]\n", );
        fprintf(fp, "    call pop_lcall\n"); // pop while loop address
        fprintf(fp, "    push eax\n");
        fprintf(fp, "    call pop_lcall\n"); // address of instruction after while loop
        fprintf(fp, "    mov [RETURN_ADDRESS] , eax\n");
        fprintf(fp, ".loop:                                 ;; function to clean up call stack \n");
        fprintf(fp, "    call pop_call\n");
        fprintf(fp, "    mov [POPED_ADDRESS] , eax\n");
        fprintf(fp, "    mov eax ,[POPED_ADDRESS]\n");
        fprintf(fp, "    cmp eax, [RETURN_ADDRESS]\n");
        fprintf(fp, "    jne .loop\n");
        fprintf(fp, "    pop eax\n");                    // loop addresss
        fprintf(fp, "    mov ebx , [RETURN_ADDRESS]\n"); // return address
        fprintf(fp, "    mov [RETURN_ADDRESS] , eax\n");
        fprintf(fp, "    push ebx\n");
        fprintf(fp, "    mov eax , [RETURN_ADDRESS]\n");
        fprintf(fp, "    jmp eax\n");
    }
    break;

    case BREAK:
    {

        fprintf(fp, "    call pop_lbase\n");
        fprintf(fp, "    mov [pesp] , eax\n");
        fprintf(fp, "    call pop_lbase\n");
        fprintf(fp, "    mov [pebp] , eax\n");

        // fprintf(fp, "    lea eax , [%s]\n", );
        fprintf(fp, "    call pop_lcall\n"); // pop while loop address
        fprintf(fp, "    call pop_lcall\n"); // address of instruction after while loop
        fprintf(fp, "    mov [RETURN_ADDRESS] , eax\n");
        fprintf(fp, ".loop:                                 ;; function to clean up call stack \n");
        fprintf(fp, "    call pop_call\n");
        fprintf(fp, "    mov [POPED_ADDRESS] , eax\n");
        fprintf(fp, "    mov eax ,[POPED_ADDRESS]\n");
        fprintf(fp, "    cmp eax, [RETURN_ADDRESS]\n");
        fprintf(fp, "    jne .loop\n");
        fprintf(fp, "    mov eax , [RETURN_ADDRESS]\n");
        fprintf(fp, "    jmp eax\n");
    }
    break;
    case LESS_THAN:
    {
        translate(ast->left, context, fp); // parse left expression
        translate(ast->right, context, fp);
        fprintf(fp, "    pop ebx\n");
        fprintf(fp, "    pop dx \n"); // parse right expression
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop cx \n");

        fprintf(fp, "    cmp eax , ebx\n");
        fprintf(fp, "    setl al\n");
        fprintf(fp, "    movzx eax , al\n");
        fprintf(fp, "    mov bl , [INT_TYPE]\n");
        fprintf(fp, "    movzx bx , bl\n");
        fprintf(fp, "    push   bx\n");
        fprintf(fp, "    push eax\n");
    }
    break;
    case GRATER_THAN:
    {
        translate(ast->left, context, fp);  // parse left expression
        translate(ast->right, context, fp); // parse right expression
        fprintf(fp, "    pop ebx\n");
        fprintf(fp, "    pop dx \n"); // parse right expression
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop cx \n");

        fprintf(fp, "    cmp eax , ebx\n");
        fprintf(fp, "    setg al\n");
        fprintf(fp, "    movzx eax , al\n");
        fprintf(fp, "    mov bl , [INT_TYPE]\n");
        fprintf(fp, "    movzx bx , bl\n");
        fprintf(fp, "    push   bx\n");
        fprintf(fp, "    push eax\n");
    }
    break;
    case LESS_THAN_EQTO:
    {
        translate(ast->left, context, fp);  // parse left expression
        translate(ast->right, context, fp); // parse right expression
        fprintf(fp, "    pop ebx\n");
        fprintf(fp, "    pop dx \n"); // parse right expression
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop cx \n");

        fprintf(fp, "    cmp eax , ebx\n");
        fprintf(fp, "    setle al\n");
        fprintf(fp, "    movzx eax , al\n");
        fprintf(fp, "    mov bl , [INT_TYPE]\n");
        fprintf(fp, "    movzx bx , bl\n");
        fprintf(fp, "    push   bx\n");
        fprintf(fp, "    push eax\n");
    }
    break;
    case GRATER_THAN_EQTO:
    {
        translate(ast->left, context, fp);  // parse left expression
        translate(ast->right, context, fp); // parse right expression
        fprintf(fp, "    pop ebx\n");
        fprintf(fp, "    pop dx \n"); // parse right expression
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop cx \n");

        fprintf(fp, "    cmp eax , ebx\n");
        fprintf(fp, "    setge al\n");
        fprintf(fp, "    movzx eax , al\n");
        fprintf(fp, "    mov bl , [INT_TYPE]\n");
        fprintf(fp, "    movzx bx , bl\n");
        fprintf(fp, "    push   bx\n");
        fprintf(fp, "    push eax\n");
    }
    break;
    case EQUALTO:
    {
        translate(ast->left, context, fp);  // parse left expression
        translate(ast->right, context, fp); // parse right expression
        fprintf(fp, "    pop ebx\n");
        fprintf(fp, "    pop dx \n"); // parse right expression
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop cx \n");

        fprintf(fp, "    cmp eax , ebx\n");
        fprintf(fp, "    sete al\n");
        fprintf(fp, "    movzx eax , al\n");
        fprintf(fp, "    mov bl , [INT_TYPE]\n");
        fprintf(fp, "    movzx bx , bl\n");
        fprintf(fp, "    push   bx\n");
        fprintf(fp, "    push eax\n");
    }
    break;
    case NOT_EQLTO:
    {
        translate(ast->left, context, fp);  // parse left expression
        translate(ast->right, context, fp); // parse right expression
        fprintf(fp, "    pop ebx\n");
        fprintf(fp, "    pop dx \n"); // parse right expression
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop cx \n");

        fprintf(fp, "    cmp eax , ebx\n");
        fprintf(fp, "    setne al\n");
        fprintf(fp, "    movzx eax , al\n");
        fprintf(fp, "    mov bl , [INT_TYPE]\n");
        fprintf(fp, "    movzx bx , bl\n");
        fprintf(fp, "    push   bx\n");
        fprintf(fp, "    push eax\n");
        fprintf(fp, "    push eax\n");
    }
    break;
    case IDENTIFIER:
    {

        SymbolTableEntry *entry = checkSymbolEntry(context, ast->token);
        if (entry->scope == 1)
        { // this is global var
            fprintf(fp, "    mov bl , [%s]\n", ast->token.lexeme);
            fprintf(fp, "    movzx bx , bl \n");
            fprintf(fp, "    mov eax , [%s + 1]\n", ast->token.lexeme);

            // store the value of identifer to eax
        }
        else
        {
            int symbolOffset = getSymbolOffset(context, entry);
            // fprintf(fp, "    mov eax , [ebp + (%d) ]     ;; %s\n", symbolOffset, entry->token.lexeme);
            fprintf(fp, "    mov ecx , [pebp] \n");
            fprintf(fp, "    mov bl , [ ecx + (%d) ]\n", symbolOffset);
            fprintf(fp, "    movzx bx , bl \n");
            fprintf(fp, "    mov eax , [ecx + (%d) + 1 ] \n", symbolOffset);
        }
        fprintf(fp, "    push bx\n");
        fprintf(fp, "    push eax\n");
    }
    break;
    case INTEGER_CONSTANT:
        fprintf(fp, "    mov bl , [INT_TYPE]\n");
        fprintf(fp, "    movzx bx , bl\n");
        fprintf(fp, "    push bx\n");
        fprintf(fp, "    mov eax, %s\n", ast->token.lexeme);
        fprintf(fp, "    push eax\n");

        break;
    case HEX_CONSTANT:
        fprintf(fp, "    mov bl , [INT_TYPE]\n");
        fprintf(fp, "    movzx bx , bl\n");
        fprintf(fp, "    push bx\n");
        fprintf(fp, "    mov eax, %s\n", ast->token.lexeme);
        fprintf(fp, "    push eax\n");

        break;

    case FLOAT_CONSTANT:
        fprintf(fp, "    mov bl , [FLOAT_TYPE]\n");
        fprintf(fp, "    movzx bx , bl\n");
        fprintf(fp, "    push bx\n");
        fprintf(fp, "    mov eax, %s\n", ast->token.lexeme);
        fprintf(fp, "    push eax\n");

        break;

    case VALUE_AT:
        translate(ast->left, context, fp);
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop bx\n");
        fprintf(fp, "    mov eax , [eax]             ;; mov value to eax from address stored at eax\n");
        fprintf(fp, "    push bx\n");
        fprintf(fp, "    push eax\n");
        break;

    case STRING_CONSTANT:
    {
        char *litral_label = label_generate();
        // printf("\n %s",ast->token.lexeme);
        // printf("\n %s",litral_label);

        pushLitralTable(context->litralTable, litral_label, ast->token.lexeme);
        int string_length = strlen(ast->token.lexeme);
        // fprintf(fp, "    mov ecx , %d\n", string_length);
        // fprintf(fp, "    call malloc\n");

        fprintf(fp, "    mov bl , [STRING_TYPE]\n");
        fprintf(fp, "    movzx bx , bl\n");
        fprintf(fp, "    push bx\n");
        fprintf(fp, "    lea eax, [%s]              ;; Load the address of litral into eax\n", litral_label);

        fprintf(fp, "    push eax\n");
    }
    break;
    case FUNCTION:
        // FUNTION ->LEFT.TOKEN == IDENTIFIER
        FunctionTableEntry *entry = checkFuntionEntry(context, ast->left->token);
        // FUNTION ->RIGHT = ARGS + BODY

        pushASTQnodeInQueue(context->astQueue, ast, entry->token.lexeme);
        break;

    case FUNCTION_CALL:
    {

        ASTNode *args = ast->right->right;
        while (args != NULL)
        {
            translate(args, context, fp);
            args = args->next;
        }
        args = ast->right->right;
        fprintf(fp, "    mov eax , [pebp]                  ;; store value at pebp to eax\n");
        fprintf(fp, "    call push_stack                   ;; push [pebp] to stack\n");
        fprintf(fp, "    mov eax , [pesp]                  ;; store the value at pesp to eax\n");
        fprintf(fp, "    mov [pebp] , eax                ;; allocate new base pointer\n");
        int offset = 0;
        while (args != NULL)
        {
            fprintf(fp, "    sub dword [pesp] , 5                         ;; allocate space for args on stack\n");
            args = args->next;
            offset = offset + 5;
        }
        args = ast->right->right;

        while (args != NULL)
        {
            fprintf(fp, "    pop eax\n");
            fprintf(fp, "    pop bx\n");
            fprintf(fp, "    mov ecx , [pebp]                        ;; store the address value to ebx\n");
            fprintf(fp, "    mov [ecx - %d] , bl   \n", offset);
            fprintf(fp, "    mov [ecx - %d + 1] , eax\n", offset);
            // fprintf(fp, "    call print_eax\n");
            // fprintf(fp, "    mov [ebp - %d] , eax                   ;; args no:- %d\n", offset, offset / 4);
            args = args->next;

            offset = offset - 5;
        }
        fprintf(fp, "    call  %s\n", ast->left->token.lexeme);
        fprintf(fp, "    mov eax , [pebp]                   ;; store the value at pebp to eax\n");
        fprintf(fp, "    mov [pesp] , eax                ;; restore the stack pointer\n");
        fprintf(fp, "    call pop_stack                     ;; pop stack top to eax\n");
        fprintf(fp, "    mov [pebp] , eax                    ; restore the base pointer\n");
        fprintf(fp, "    mov bx , [RETURN_VALUE]\n");
        fprintf(fp, "    mov eax , [RETURN_VALUE  + 1]\n");
        fprintf(fp, "    push bx\n");
        fprintf(fp, "    push eax\n");
    }
    break;
    case ARG:
    {
        translate(ast->left, context, fp);
        return;
    }
    case PARAM_START:
    {

        SymbolTable *symboltable = getSymboTableFromQueue(context);
        pushSymbolTable(context->symbolTableStack, symboltable);
        // pushSymbolTable(context->symbolTableTempStack, symboltable);
    }

    break;

    case PARAM_END:
    {
        // SymbolTableEntry *entry = popSymbolTable(context->symbolTableStack);
        // pushSymbolTable(context->symbolTableTempStack, entry);
    }

    break;

    case ARGS_START:

        break;

    case ARGS_END:

        return;

    case LOGICAL_AND:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop cx \n");
        fprintf(fp, "    pop ebx\n");
        fprintf(fp, "    pop dx \n");
        fprintf(fp, "    test eax , eax\n"); //  Logical AND with itself to check if zero
        fprintf(fp, "    setne al\n");       //  Set cl to 1 if eax is not zero
        fprintf(fp, "    test ebx , ebx\n");
        fprintf(fp, "    setne bl\n");
        fprintf(fp, "    and al , bl\n");
        fprintf(fp, "    movzx eax , al\n");
        fprintf(fp, "    mov bl , [INT_TYPE]\n");
        fprintf(fp, "    movzx bx , bl\n");
        fprintf(fp, "    push bx\n");
        fprintf(fp, "    push eax\n");
        break;

    case LOGICAL_OR:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop cx \n");
        fprintf(fp, "    pop ebx\n");
        fprintf(fp, "    pop dx \n");
        fprintf(fp, "    test eax , eax\n"); //  Logical AND with itself to check if zero
        fprintf(fp, "    setne al\n");       //  Set al to 1 if eax is not zero
        fprintf(fp, "    test ebx , ebx\n");
        fprintf(fp, "    setne bl\n");
        fprintf(fp, "    or al , bl\n");
        fprintf(fp, "    movzx eax , al\n");
        fprintf(fp, "    mov bl , [INT_TYPE]\n");
        fprintf(fp, "    movzx bx , bl\n");
        fprintf(fp, "    push bx\n");
        fprintf(fp, "    push eax\n");
        break;
    case LOGICAL_NOT:
        translate(ast->left, context, fp);
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop bx\n");
        fprintf(fp, "    test eax , eax\n");
        fprintf(fp, "    sete al\n");
        fprintf(fp, "    movzx eax , al");
        fprintf(fp, "    mov bl , [INT_TYPE]\n");
        fprintf(fp, "    movzx bx , bl\n");
        fprintf(fp, "    push bx\n");
        fprintf(fp, "    push eax\n");
        break;

    case BIT_AND:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop cx \n");
        fprintf(fp, "    pop ebx\n");
        fprintf(fp, "    pop dx \n");
        fprintf(fp, "    and eax ,ebx\n");
        fprintf(fp, "    mov bl , [INT_TYPE]\n");
        fprintf(fp, "    movzx bx , bl\n");
        fprintf(fp, "    push bx\n");
        fprintf(fp, "    push eax\n");
        break;

    case BIT_OR:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop cx \n");
        fprintf(fp, "    pop ebx\n");
        fprintf(fp, "    pop dx \n");
        fprintf(fp, "    or eax ,ebx\n");
        fprintf(fp, "    mov bl , [INT_TYPE]\n");
        fprintf(fp, "    movzx bx , bl\n");
        fprintf(fp, "    push bx\n");
        fprintf(fp, "    push eax\n");
        break;

    case BIT_NOT:
        break;

    case BODYSTART:
    {
        fprintf(fp, "    mov eax , [pebp]                  ;; store value at pebp to eax\n");
        fprintf(fp, "    call push_stack                   ;; push [pebp] to stack\n");
        fprintf(fp, "    mov eax , [pesp]                  ;; store the value at pesp to eax\n");
        fprintf(fp, "    mov [pebp] , eax                ;; allocate new base pointer\n");
        // fprintf(fp, "    push ebp                ; Save base pointer\n");
        // fprintf(fp, "    mov ebp, esp            ; Establish new base pointer\n");
        tranlateLocalVar(ast->right, fp);
        SymbolTable *symboltable = getSymboTableFromQueue(context);
        pushSymbolTable(context->symbolTableStack, symboltable);
        // pushSymbolTable(context->symbolTableTempStack, symboltable);
        translate(ast->right, context, fp);
    }
    break;
    case BODYEND:
    {
        fprintf(fp, "    mov eax , [pebp]                   ;; store the value at pebp to eax\n");
        fprintf(fp, "    mov [pesp] , eax                ;; restore the stack pointer\n");
        fprintf(fp, "    call pop_stack                     ;; pop stack top to eax\n");
        fprintf(fp, "    mov [pebp] , eax                    ; restore the base pointer\n");
        // fprintf(fp, "    mov esp, ebp                   ;; Restore the stack pointer\n");
        // fprintf(fp, "    pop ebp                        ;; Restore the base pointer\n");
        SymbolTable *symbolTable = popSymbolTable(context->symbolTableStack);

        pushSymbolTable(context->symbolTableTempStack, symbolTable);
        break;
    }

    case ASM:
    {

        ASTNode *temp = ast->right;
        while (temp != NULL)
        {

            fprintf(fp, "    %s\n", remove_quotes(temp->token.lexeme));
            temp = temp->next;
        }
        break;
    }

    default:
        printf("\nERROR: %s token handle not implemented", ast->token.lexeme);
        exit(1);
    }
    return translate(ast->next, context, fp);
}

void generateLabels(Context *context, FILE *fp)
{
    ASTQNode *current = popFromASTQueueFront(context->astQueue);
    int num_elements = 0;
    while (current != NULL)
    {

        num_elements = context->astQueue->num_elements;
        fprintf(fp, "%s:\n", current->label);

        if (current->ast->token.value == WHILE)
        {
            fprintf(fp, "    pop eax                        ;; pop the return address to eax\n");
            fprintf(fp, "    call push_call                 ;; store the return address to ra location\n");
            fprintf(fp, "    call push_lcall\n");

            fprintf(fp, "    lea eax , [%s]\n", current->label);
            fprintf(fp, "    call push_lcall\n");
            fprintf(fp, "    mov eax , [pebp]\n");
            fprintf(fp, "    call push_lbase\n");
            fprintf(fp, "    mov eax , [pesp]\n");
            fprintf(fp, "    call push_lbase\n");
            translate(current->ast->right, context, fp);
            translate(current->ast->left, context, fp);
            fprintf(fp, "    pop eax\n");
            fprintf(fp, "    pop bx\n");
            fprintf(fp, "    mov [condition] , bl\n");
            fprintf(fp, "    mov [condition + 1] ,  eax\n");
            // fprintf(fp, "    mov ecx , 100000000\n");
            // fprintf(fp, "    call delay\n");
            // fprintf(fp, "    call delay\n");
            fprintf(fp, "    call pop_lbase\n"); // stack pointer
            fprintf(fp, "    mov [pesp] , eax\n");
            fprintf(fp, "    call pop_lbase\n"); // base pointer
            fprintf(fp, "    mov [pebp] , eax \n");
            fprintf(fp, "    call pop_lcall\n");
            fprintf(fp, "    call pop_lcall\n");
            fprintf(fp, "    call pop_call                  ;; store the return address to eax\n");
            fprintf(fp, "    push eax\n");
            fprintf(fp, "    mov eax , [condition + 1]\n");
            fprintf(fp, "    test eax , eax\n");
            fprintf(fp, "    jnz %s\n", current->label);
            fprintf(fp, "    pop eax\n");
            fprintf(fp, "    jmp eax                        ;; jmp to return address\n");
        }
        else if (current->ast->token.value == FUNCTION)
        {
            fprintf(fp, "    pop eax                        ;; pop the return address to eax\n");
            fprintf(fp, "    call push_call                 ;; store the return address to ra location\n");
            fprintf(fp, "    call push_fcall\n");
            fprintf(fp, "    mov eax , [pebp]\n");
            fprintf(fp, "    call push_base\n");   // push base pebp base pointer
            fprintf(fp, "    mov eax , [pesp]\n"); // push base pesp stack pointer
            fprintf(fp, "    call push_base\n");
            // here we have to push callers return addresF
            // and push push callers base address
            translate(current->ast->right, context, fp);
            fprintf(fp, "    call pop_base\n"); // stack pointer
            fprintf(fp, "    mov [pesp] , eax\n");
            fprintf(fp, "    call pop_base\n"); // base pointer
            fprintf(fp, "    mov [pebp] , eax \n");

            fprintf(fp, "    call pop_call                  ;; store the return address to eax\n");
            fprintf(fp, "    call pop_fcall\n");
            fprintf(fp, "    jmp eax                        ;; jmp to return address\n");
        }
        else
        {
            fprintf(fp, "    pop eax                        ;; pop the return address to eax\n");
            fprintf(fp, "    call push_call                 ;; store the return address to ra location\n");
            translate(current->ast, context, fp);
            fprintf(fp, "    call pop_call                  ;; store the return address to eax\n");
            fprintf(fp, "    jmp eax                        ;; jmp to return address\n");
        }

        // step 1 : popfromASTQueueRear  and push into astStack till num_elements == context->astQueue->num_elemnts
        // step 2 :  pop  from astStack
        // step 3: current = pop(astStack);
        // step 3: translate
        // if new elements is added pushElemts in queue repeat step 1;
        if (num_elements < context->astQueue->num_elements)
        {

            SymbolTable *st = getTopSymbolTable(context->symbolTableTempStack);
            if (st == NULL)
            {
                printf("\nERROR :symbol table temp stack is empty");
            }
            pushSymbolTable(context->symbolTableStack, st);
            while (num_elements < context->astQueue->num_elements)
            {
                // step 1 : popfromASTQueueRear  and push into astStack till num_elements == context->astQueue->num_elemnts
                current = popFromASTQueueRear(context->astQueue);
                pushASTStack(context->astStack, current);
            }
        }
        current = popASTStack(context->astStack);
        if (current != NULL)
        {
            continue;
        }

        while (getTopSymbolTable(context->symbolTableTempStack) != NULL || getTopSymbolTable(context->symbolTableStack)->next != NULL)
        {

            if (getTopSymbolTable(context->symbolTableStack)->next != NULL)
            {
                popSymbolTable(context->symbolTableStack);
            }

            popSymbolTable(context->symbolTableTempStack);
        }

        // step 2 :  pop  from astStack
        current = popFromASTQueueFront(context->astQueue);
    }
}

void print_eax(FILE *fp)
{

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