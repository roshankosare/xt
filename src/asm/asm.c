#include "../../include/asm/asm.h"
#include "../../include/parser/helper.h"
#include "../../include/symboltable/functiontable.h"
#include "../../include/symboltable/symboltable.h"
#include "../../include/litrals/litrals.h"
#include <assert.h>

void translateGlobalVar(ASTNode *ast, FILE *fp);
void generateLabels(Context *context, FILE *fp);
void tranlateLocalVar(ASTNode *ast, FILE *fp);
void createLitralConstansts(Context *Context, FILE *fp);
void call_stack_operations(FILE *fp);
void param_stack_operations(FILE *fp);
void print_eax(FILE *fp);
void printDelay(FILE *fp);

void createASMFile(ASTNode *ast, Context *context, FILE *fp)
{
    fprintf(fp, "section .data                        ;; Section for initialized data\n");
    translateGlobalVar(ast, fp);
    fprintf(fp, "    call_stack_top dd 0\n");
    fprintf(fp, "    pesp dd 0\n");
    fprintf(fp, "    pebp dd 0\n");
    fprintf(fp, "    buffer db '0000000000', 0       ; Buffer to hold the converted number (10 digits max)\n");
    fprintf(fp, "    len equ 10                      ; Length of the buffer\n");
    fprintf(fp, "    space db  \" \"                       ; Space character to print\n");
    fprintf(fp, "    condition dd 0\n");

    fprintf(fp, "section .bss\n");
    fprintf(fp, "    call_stack resb 4096               ;; Reserve 4096 bytes (4 KB) for the call stack\n");
    fprintf(fp, "    param_stack resb 4096              ;; Reserve 4096 bytes (4 KB) for the param stack\n");
    fprintf(fp, "\n");

    fprintf(fp, "section .text                        ;; Section for code\n");
    fprintf(fp, "    global _start                    ;; Make the _start symbol available to the linker\n");
    fprintf(fp, "\n");
    fprintf(fp, "_start: \n");
    fprintf(fp, "    mov eax , call_stack + 4096\n");
    fprintf(fp, "    mov [call_stack_top] , eax                 ;; init call stack \n");
    fprintf(fp, "    mov eax , param_stack + 4096\n");
    fprintf(fp, "    mov [pesp] , eax\n");
    fprintf(fp, "    mov [pebp] , eax\n");
    translate(ast, context, fp); // translate code;
    fprintf(fp, "; Exit the program\n");
    fprintf(fp, "    mov eax, 1                       ;; syscall number for sys_exit\n");
    fprintf(fp, "    xor ebx, ebx                     ;; exit code 0\n");
    fprintf(fp, "    int 0x80                         ;; make syscall\n");

    call_stack_operations(fp);
    param_stack_operations(fp);
    print_eax(fp);
    printDelay(fp);

    generateLabels(context, fp);

    fprintf(fp, "section .rodata\n");
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
            fprintf(fp, "    %s dd 0\n", ast->right->left->token.lexeme);
        }
        else
        {
            fprintf(fp, "    %s dd 0\n", ast->right->token.lexeme);
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
            fprintf(fp, "    sub dword [pesp] , 4\n");
            // fprintf(fp, "    sub esp , 4\n");
        }
        else
        {
            fprintf(fp, "    sub dword [pesp] , 4\n");
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
        fprintf(fp, "    %s: dd %s, 0\n", entry->label, entry->value);
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
        fprintf(fp, "    pop ebx\n");
        fprintf(fp, "    add eax , ebx\n");
        fprintf(fp, "    push eax\n");
        break;
    case MUL:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        fprintf(fp, "    ;; mul\n");
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop ebx\n");
        fprintf(fp, "    imul eax , ebx\n");
        fprintf(fp, "    push eax\n");
        break;
    case DIV:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        fprintf(fp, "    ;; mul\n");
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop ebx\n");
        fprintf(fp, "    idiv eax , ebx\n");
        fprintf(fp, "    push eax\n");
        break;
    case MINUS:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        fprintf(fp, "    ;; plus\n");
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop ebx\n");
        fprintf(fp, "    sub eax , ebx\n");
        fprintf(fp, "    push eax\n");
        break;
    case ASSIGN:
    {
        translate(ast->right, context, fp); // convert expression to asm which will store value of evaluated expresion to stack
        fprintf(fp, "    pop eax\n");

        SymbolTableEntry *entry = checkSymbolEntry(context, ast->left->token);
        if (entry->scope == 1)
        {
            // this is global var
            fprintf(fp, "    mov [%s] , eax\n", ast->left->token.lexeme);
            fprintf(fp, "    push eax\n"); // store the to address of identifier
        }
        else
        {
            int offset = getSymbolOffset(context, entry);
            fprintf(fp, "    mov ebx , [pebp]                    ;; store the address to ebx\n");
            fprintf(fp, "    mov [ebx + (%d)] , eax              ;; store the value at location ebx\n", offset, entry->token.lexeme);
            // fprintf(fp, "    mov [ebp + (%d) ] , eax   ;;%s\n", offset, entry->token.lexeme);
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
                fprintf(fp, "     inc dword [%s]\n", ast->left->token.lexeme);
                translate(ast->left, context, fp);
                // store the value of identifer to eax
            }
            else
            {
                int offset = getSymbolOffset(context, entry);
                fprintf(fp, "    mov eax , [pebp]           ;; move address stored in pebp to eax\n");
                fprintf(fp, "    inc dword [eax + (%d)]     ;; %s\n", offset, entry->token.lexeme);
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
                fprintf(fp, "     inc dword [%s]\n", ast->right->token.lexeme);

                // store the value of identifer to eax
            }
            else
            {
                int offset = getSymbolOffset(context, entry);
                translate(ast->right, context, fp);
                fprintf(fp, "    mov eax , [pebp]           ;; move address stored in pebp to eax\n");
                fprintf(fp, "    inc dword [eax + (%d)]     ;; %s\n", offset, entry->token.lexeme);
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
                fprintf(fp, "     dec dword [%s]\n", ast->left->token.lexeme);
                translate(ast->left, context, fp);
                // store the value of identifer to eax
            }
            else
            {
                int offset = getSymbolOffset(context, entry);
                fprintf(fp, "    mov eax , [pebp] \n");
                fprintf(fp, "    dec dword [eax + (%d)  ]     ;; %s\n", offset, entry->token.lexeme);
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
                fprintf(fp, "     inc dword [%s]\n", ast->right->token.lexeme);

                // store the value of identifer to eax
            }
            else
            {
                int offset = getSymbolOffset(context, entry);
                translate(ast->right, context, fp);
                fprintf(fp, "    mov eax , [pebp] \n");
                fprintf(fp, "    dec dword [eax + (%d)  ]     ;; %s\n", offset, entry->token.lexeme);
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

    break;
    case IF:
        translate(ast->left, context, fp);
        // prase  condition first
        char *label_true = label_generate(); // generate label for  this if block
        char *label_false = label_generate();
        fprintf(fp, "    pop eax\n");        // get value of expression from stack
        fprintf(fp, "    test eax , eax\n"); // check if value is non zero
        fprintf(fp, "    lea eax , [%s]\n", label_false);
        fprintf(fp, "    push eax\n");
        fprintf(fp, "    jnz %s                 ;; jump if expression  is not  zero\n", label_true);
        pushASTQnodeInQueue(context->astQueue, ast->right, label_true);
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    lea eax , [%s]         ;; save the false label to eax\n", label_false);
        fprintf(fp, "    jmp eax\n");
        fprintf(fp, "%s:                        ;; defination of false label \n", label_false);
        break;
    case ELSE:
        assert(0 && "TODO: ELSE is not implemented");
    case FOR:
        assert(0 && "TODO: FOR is not implemented");
    case RETURN:
    {
        if (ast->left != NULL)
        {
            translate(ast->left, context, fp);
        }
        fprintf(fp, "    ret\n");
        break;
    }

    case WHILE:
    {
        char *label_true = label_generate(); // generate label for  this if block
        char *label_false = label_generate();
        translate(ast->left, context, fp);

        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    mov [condition] , eax\n");

        fprintf(fp, "    lea eax , [%s]\n", label_false);
        fprintf(fp, "    push eax\n");
        fprintf(fp, "    mov eax , [condition]\n");
        fprintf(fp, "    test eax , eax\n");
        fprintf(fp, "    jnz %s\n", label_true);
        pushASTQnodeInQueue(context->astQueue, ast, label_true);
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    lea eax , [%s]         ;; save the false label to eax\n", label_false);
        fprintf(fp, "    jmp eax\n");
        fprintf(fp, "%s:                        ;; defination of false label \n", label_false);
        // assert(0 && "TODO: WHILE is not implemented");
    }
    break;
    case LESS_THAN:
    {
        translate(ast->left, context, fp);  // parse left expression
        translate(ast->right, context, fp); // parse right expression
        fprintf(fp, "    pop ebx\n");       // right oprand
        fprintf(fp, "    pop eax\n");       // left oprand
        fprintf(fp, "    cmp eax , ebx\n");
        fprintf(fp, "    setl al\n");
        fprintf(fp, "    movzx eax , al\n");
        fprintf(fp, "    push eax\n");
    }
    break;
    case GRATER_THAN:
    {
        translate(ast->left, context, fp);  // parse left expression
        translate(ast->right, context, fp); // parse right expression
        fprintf(fp, "    pop ebx\n");       // right oprand
        fprintf(fp, "    pop eax\n");       // left oprand
        fprintf(fp, "    cmp eax , ebx\n");
        fprintf(fp, "    setg al\n");
        fprintf(fp, "    movzx eax , al\n");
        fprintf(fp, "    push eax\n");
    }
    break;
    case LESS_THAN_EQTO:
    {
        translate(ast->left, context, fp);  // parse left expression
        translate(ast->right, context, fp); // parse right expression
        fprintf(fp, "    pop ebx\n");       // right oprand
        fprintf(fp, "    pop eax\n");       // left oprand
        fprintf(fp, "    cmp eax , ebx\n");
        fprintf(fp, "    setle al\n");
        fprintf(fp, "    movzx eax , al\n");
        fprintf(fp, "    push eax\n");
    }
    break;
    case GRATER_THAN_EQTO:
    {
        translate(ast->left, context, fp);  // parse left expression
        translate(ast->right, context, fp); // parse right expression
        fprintf(fp, "    pop ebx\n");       // right oprand
        fprintf(fp, "    pop eax\n");       // left oprand
        fprintf(fp, "    cmp eax , ebx\n");
        fprintf(fp, "    setge al\n");
        fprintf(fp, "    movzx eax , al\n");
        fprintf(fp, "    push eax\n");
    }
    break;
    case EQUALTO:
    {
        translate(ast->left, context, fp);  // parse left expression
        translate(ast->right, context, fp); // parse right expression
        fprintf(fp, "    pop ebx\n");       // right oprand
        fprintf(fp, "    pop eax\n");       // left oprand
        fprintf(fp, "    cmp eax , ebx\n");
        fprintf(fp, "    sete al\n");
        fprintf(fp, "    movzx eax , al\n");
        fprintf(fp, "    push eax\n");
    }
    break;
    case NOT_EQLTO:
    {
        translate(ast->left, context, fp);  // parse left expression
        translate(ast->right, context, fp); // parse right expression
        fprintf(fp, "    pop eax\n");       // right oprand
        fprintf(fp, "    pop ebx\n");       // left oprand
        fprintf(fp, "    cmp eax , ebx\n");
        fprintf(fp, "    setne al\n");
        fprintf(fp, "    movzx eax , al\n");
        fprintf(fp, "    push eax\n");
    }
    break;
    case IDENTIFIER:
    {

        SymbolTableEntry *entry = checkSymbolEntry(context, ast->token);
        if (entry->scope == 1)
        { // this is global var
            fprintf(fp, "    mov eax , [%s]\n", ast->token.lexeme);
            // store the value of identifer to eax
        }
        else
        {
            int symbolOffset = getSymbolOffset(context, entry);
            // fprintf(fp, "    mov eax , [ebp + (%d) ]     ;; %s\n", symbolOffset, entry->token.lexeme);
            fprintf(fp, "    mov eax , [pebp]              ;; load the address stored in pebp to eax\n");
            fprintf(fp, "    mov eax , [eax + (%d) ]     ;; %s\n", symbolOffset, entry->token.lexeme);
        }
        fprintf(fp, "    push eax\n");
    }
    break;
    case INTEGER_CONSTANT:
        fprintf(fp, "    mov eax, %s\n", ast->token.lexeme);
        fprintf(fp, "    push eax\n");
        break;

    case FLOAT_CONSTANT:
        assert(0 && "TODO:- handle FLOAT_CONSTANT not implemented");
        break;

    case VALUE_AT:
        translate(ast->left, context, fp);
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    mov eax , [eax]             ;; mov value to eax from address stored at eax\n");
        fprintf(fp, "    push eax\n");
        break;

    case STRING_CONSTANT:
        char *litral_label = label_generate();
        // printf("\n %s",ast->token.lexeme);
        // printf("\n %s",litral_label);

        pushLitralTable(context->litralTable, litral_label, ast->token.lexeme);
        fprintf(fp, "    lea eax, [%s]              ;; Load the address of litral into eax\n", litral_label);
        fprintf(fp, "    push eax\n");
        break;
    case FUNCTION:
        // FUNTION ->LEFT.TOKEN == IDENTIFIER
        FunctionTableEntry *entry = checkFuntionEntry(context, ast->left->token);
        // FUNTION ->RIGHT = ARGS + BODY

        pushASTQnodeInQueue(context->astQueue, ast->right, entry->token.lexeme);
        break;

    case FUNCTION_CALL:
    {
        fprintf(fp, "    mov eax , [pebp]                  ;; store value at pebp to eax\n");
        fprintf(fp, "    call push_stack                   ;; push [pebp] to stack\n");
        fprintf(fp, "    mov eax , [pesp]                  ;; store the value at pesp to eax\n");
        fprintf(fp, "    mov [pebp] , eax                ;; allocate new base pointer\n");
        ASTNode *args = ast->right->right;

        int offset = 4;
        while (args != NULL)
        {
            fprintf(fp, "    sub dword [pesp] , 4                         ;; allocate space for args on stack\n");
            args = args->next;
        }
        args = ast->right->right;
        while (args != NULL)
        {
            translate(args, context, fp);
            fprintf(fp, "    pop eax\n");
            fprintf(fp, "    mov ebx , [pebp]                        ;; store the address value to ebx\n");
            fprintf(fp, "    mov [ebx - %d] , eax                   ;; args no:- %d\n", offset, offset / 4);
            // fprintf(fp, "    mov [ebp - %d] , eax                   ;; args no:- %d\n", offset, offset / 4);
            args = args->next;
            offset = offset + 4;
        }
        fprintf(fp, "    call  %s\n", ast->left->token.lexeme);
        fprintf(fp, "    mov eax , [pebp]                   ;; store the value at pebp to eax\n");
        fprintf(fp, "    mov [pesp] , eax                ;; restore the stack pointer\n");
        fprintf(fp, "    call pop_stack                     ;; pop stack top to eax\n");
        fprintf(fp, "    mov [pebp] , eax                    ; restore the base pointer\n");
    }
    break;
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
        fprintf(fp, "    pop ebx\n");
        fprintf(fp, "    test eax , eax\n"); //  Logical AND with itself to check if zero
        fprintf(fp, "    setne al\n");       //  Set cl to 1 if eax is not zero
        fprintf(fp, "    test ebx , ebx\n");
        fprintf(fp, "    setne bl\n");
        fprintf(fp, "    and al , bl\n");
        fprintf(fp, "    movzx eax , al\n");
        fprintf(fp, "    push eax\n");
        break;

    case LOGICAL_OR:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop ebx\n");
        fprintf(fp, "    test eax , eax\n"); //  Logical AND with itself to check if zero
        fprintf(fp, "    setne al\n");       //  Set al to 1 if eax is not zero
        fprintf(fp, "    test ebx , ebx\n");
        fprintf(fp, "    setne bl\n");
        fprintf(fp, "    or al , bl\n");
        fprintf(fp, "    movzx eax , al\n");
        fprintf(fp, "    push eax\n");
        break;
    case LOGICAL_NOT:
        translate(ast->left, context, fp);
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    test eax , eax\n");
        fprintf(fp, "    sete al\n");
        fprintf(fp, "    movzx eax , al");
        fprintf(fp, "    push eax\n");
        break;

    case BODYSTART:
    {
        fprintf(fp, "    mov eax , [pebp]                  ;; store value at pebp to eax\n");
        fprintf(fp, "    call push_stack                   ;; push [pebp] to stack\n");
        fprintf(fp, "    mov eax , [pesp]                  ;; store the value at pesp to eax\n");
        fprintf(fp, "    mov [pebp] , eax                ;; allocate new base pointer\n");
        // fprintf(fp, "    push ebp                ; Save base pointer\n");
        // fprintf(fp, "    mov ebp, esp            ; Establish new base pointer\n");
        tranlateLocalVar(ast, fp);
        SymbolTable *symboltable = getSymboTableFromQueue(context);
        pushSymbolTable(context->symbolTableStack, symboltable);
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
            // fprintf(fp, "    call print_eax\n");
            translate(current->ast->right, context, fp);
            translate(current->ast->left, context, fp);
            fprintf(fp, "    pop eax\n");

            fprintf(fp, "    mov [condition] ,  eax\n");
            // fprintf(fp, "    call print_eax\n");

            // fprintf(fp, "    mov ecx , 100000000\n");
            // fprintf(fp, "    call delay\n");
            // fprintf(fp, "    call delay\n");

            fprintf(fp, "    call pop_call                  ;; store the return address to eax\n");
            fprintf(fp, "    push eax\n");
            // fprintf(fp, "    call print_eax\n");
            fprintf(fp, "    mov eax , [condition]\n");
            fprintf(fp, "    test eax , eax\n");
            fprintf(fp, "    jnz %s\n", current->label);
            fprintf(fp, "    pop eax\n");
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

        while (getTopSymbolTable(context->symbolTableTempStack) != NULL)
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

void call_stack_operations(FILE *fp)
{

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

void param_stack_operations(FILE *fp)
{

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