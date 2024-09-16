#include "../../include/asm/asmstmt.h"
#include "../../include/parser/helper.h"

void print_if(ASTNode *ast, Context *context, FILE *fp)
{
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
    return;
}
void print_while(ASTNode *ast, Context *context, FILE *fp)
{
    char *label_true = label_generate(); // generate label for  this if block
    char *label_false = label_generate();

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
}

void print_var(ASTNode *ast, Context *context, FILE *fp)
{
    char *label = malloc(100 * sizeof(char)); // Allocate memory for 20 characters (adjust size as needed)

    if (label == NULL)
    {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    if (ast->right->token.value == ASSIGN) // decleration with assignment
    {
        SymbolTableEntry *entry = checkSymbolEntry(context, ast->right->left->token);
        if (entry->scope == 1)
        {
            snprintf(label, 100, "    %s dd 5 dup(0)\n", ast->right->left->token.lexeme);
            print_data_section(fp, label);
        }
        else
        {
            // fprintf(fp, "    sub dword [pesp] , 5\n");
        }
        translate(ast->right, context, fp);
        return;
    }

    SymbolTableEntry *entry = checkSymbolEntry(context, ast->right->token);
    if (entry->scope == 1)
    {
        snprintf(label, 100, "    %s dd 5 dup(0)\n", ast->right->token.lexeme);
        print_data_section(fp, label);
    }
    else
    {
        // fprintf(fp, "    sub dword [pesp] , 5\n");
    }
    return;
}
void print_funtion(ASTNode *ast, Context *context, FILE *fp)
{
    // FUNTION ->LEFT.TOKEN == IDENTIFIER
    FunctionTableEntry *entry = checkFuntionEntry(context, ast->left->token);
    // FUNTION ->RIGHT = ARGS + BODY

    pushASTQnodeInQueue(context->astQueue, ast, entry->token.lexeme);
    return;
}
void print_assign(ASTNode *ast, Context *context, FILE *fp)
{
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
        return;
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

void print_return(ASTNode *ast, Context *context, FILE *fp)
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
}

void print_continue(ASTNode *ast, Context *context, FILE *fp)
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
void print_break(ASTNode *ast, Context *context, FILE *fp)
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
    return;
}

void print_body_start(ASTNode *ast, Context *context, FILE *fp)
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
void print_body_end(ASTNode *ast, Context *context, FILE *fp)
{
    fprintf(fp, "    mov eax , [pebp]                   ;; store the value at pebp to eax\n");
    fprintf(fp, "    mov [pesp] , eax                ;; restore the stack pointer\n");
    fprintf(fp, "    call pop_stack                     ;; pop stack top to eax\n");
    fprintf(fp, "    mov [pebp] , eax                    ; restore the base pointer\n");
    // fprintf(fp, "    mov esp, ebp                   ;; Restore the stack pointer\n");
    // fprintf(fp, "    pop ebp                        ;; Restore the base pointer\n");
    SymbolTable *symbolTable = popSymbolTable(context->symbolTableStack);

    pushSymbolTable(context->symbolTableTempStack, symbolTable);
    return;
}

void print_asm(ASTNode *ast, Context *context, FILE *fp)
{
    ASTNode *temp = ast->right;
    while (temp != NULL)
    {
        fprintf(fp, "    %s\n", remove_quotes(temp->token.lexeme));
        temp = temp->next;
    }
    return;
}
