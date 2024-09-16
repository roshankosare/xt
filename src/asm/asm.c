#include "../../include/asm/asm.h"
#include "../../include/parser/helper.h"
#include "../../include/symboltable/functiontable.h"
#include "../../include/symboltable/symboltable.h"
#include "../../include/litrals/litrals.h"
#include "../../include/asm/asmcontext.h"
#include "../../include/asm/stacks.h"
#include "../../include/asm/system.h"
#include "../../include/asm/helper.h"
#include "../../include/asm/asmexp.h"
#include "../../include/asm/asmstmt.h"
#include <assert.h>
#include <string.h>

void generateLabels(Context *context, FILE *fp);
void tranlateLocalVar(ASTNode *ast, FILE *fp);
void createLitralConstansts(Context *Context, FILE *fp);

void createASMFile(ASTNode *ast, Context *context, FILE *fp)
{
    fprintf(fp, "section .data                        ;; Section for initialized data\n");
    fprintf(fp, "    condition dd 5 dup(0)\n");
    fprintf(fp, "    RETURN_VALUE dd 5 dup(0)\n");
    fprintf(fp, "    RETURN_ADDRESS dd 0\n");
    fprintf(fp, "    POPED_ADDRESS dd 0\n");
    // fprintf(fp, "    NUM_REF_ALLO dd 0\n");
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
    print_system_functions(fp);

    generateLabels(context, fp);

    fprintf(fp, "section .rodata\n");
    fprintf(fp, "    INT_TYPE: db 1\n");
    fprintf(fp, "    FLOAT_TYPE: db 2\n");
    fprintf(fp, "    STRING_TYPE: db 3\n");
    fprintf(fp, "    REF_TYPE: db 4\n");
    createLitralConstansts(context, fp);
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
        print_add(fp);
        break;

    case MUL:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        print_mul(fp);
        break;

    case DIV:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        print_div(fp);
        break;

    case MINUS:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        print_sub(fp);
        break;

    case MODULAS:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        print_modulas(fp);
        break;

    case ASSIGN:
        translate(ast->right, context, fp); // convert expression to asm which will store value of evaluated expresion to stack
        print_assign(ast, context, fp);
        break;

    case INC:
        print_inc(ast, context, fp);
        break;

    case DEC:
        print_dec(ast, context, fp);
        break;

    case VAR:
        print_var(ast, context, fp);
        break;

    case TYPEOF:
        translate(ast->left, context, fp);
        print_typeof(fp);
        break;

    case IF:
        translate(ast->left, context, fp);
        print_if(ast, context, fp);
        // prase  condition first
        break;

    case ELSE:
        assert(0 && "TODO: ELSE is not implemented");
    case FOR:
        assert(0 && "TODO: FOR is not implemented");

    case RETURN:
        print_return(ast, context, fp);
        break;

    case WHILE:
        translate(ast->left, context, fp);
        print_while(ast, context, fp);
        // assert(0 && "TODO: WHILE is not implemented");
        break;

    case CONTINUE:
        print_continue(ast, context, fp);
        break;

    case BREAK:
        print_break(ast, context, fp);
        break;

    case LESS_THAN:
        translate(ast->left, context, fp); // parse left expression
        translate(ast->right, context, fp);
        print_less_than(fp);
        break;

    case GRATER_THAN:
        translate(ast->left, context, fp);  // parse left expression
        translate(ast->right, context, fp); // parse right expression
        print_grater_than(fp);
        break;

    case LESS_THAN_EQTO:
        translate(ast->left, context, fp);  // parse left expression
        translate(ast->right, context, fp); // parse right expression
        print_lessthan_eqto(fp);
        break;

    case GRATER_THAN_EQTO:
        translate(ast->left, context, fp);  // parse left expression
        translate(ast->right, context, fp); // parse right expression
        print_graterthan_eqto(fp);
        break;

    case EQUALTO:
        translate(ast->left, context, fp);  // parse left expression
        translate(ast->right, context, fp); // parse right expression
        print_eqto(fp);
        break;

    case NOT_EQLTO:
        translate(ast->left, context, fp);  // parse left expression
        translate(ast->right, context, fp); // parse right expression
        print_noteqto(fp);
        break;

    case IDENTIFIER:
        print_identifier(ast, context, fp);
        break;

    case INTEGER_CONSTANT:
        print_integer_constant(ast, context, fp);
        break;

    case HEX_CONSTANT:
        print_hex_constant(ast, context, fp);
        break;

    case FLOAT_CONSTANT:
        print_float_constant(ast, context, fp);
        break;

    case VALUE_AT:
        translate(ast->left, context, fp);
        print_valueat(fp);
        break;

    case STRING_CONSTANT:
        print_string_constant(ast, context, fp);
        break;

    case FUNCTION:
        print_funtion(ast, context, fp);
        break;

    case FUNCTION_CALL:
        print_funtion_call(ast, context, fp);
        break;

    case ARG:
        translate(ast->left, context, fp);
        return;

    case PARAM_START:
    {

        SymbolTable *symboltable = getSymboTableFromQueue(context);
        pushSymbolTable(context->symbolTableStack, symboltable);
        // pushSymbolTable(context->symbolTableTempStack, symboltable);
    }

    break;

    case PARAM_END:
        // SymbolTableEntry *entry = popSymbolTable(context->symbolTableStack);
        // pushSymbolTable(context->symbolTableTempStack, entry);
        break;

    case ARGS_START:
        break;

    case ARGS_END:
        return;

    case LOGICAL_AND:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        print_logical_and(fp);
        break;

    case LOGICAL_OR:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        print_logigcal_or(fp);
        break;
    case LOGICAL_NOT:
        translate(ast->left, context, fp);
        print_logical_not(fp);
        break;

    case BIT_AND:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        print_bitwise_and(fp);
        break;

    case BIT_OR:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        print_bitwise_or(fp);
        break;

    case BIT_NOT:
        print_bitwise_not(fp);
        break;

    case BODYSTART:
        print_body_start(ast, context, fp);
        break;

    case BODYEND:
        print_body_end(ast, context, fp);
        break;

    case ASM:
        print_asm(ast, context, fp);
        break;

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
