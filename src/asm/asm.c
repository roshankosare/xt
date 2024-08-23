#include "../../include/asm/asm.h"
#include "../../include/parser/helper.h"
#include "../../include/symboltable/functiontable.h"
#include "../../include/symboltable/symboltable.h"
#include <assert.h>

void translateGlobalVar(ASTNode *ast, FILE *fp);
void generateLabels(Context *context, FILE *fp);

void createASMFile(ASTNode *ast, Context *context, FILE *fp)
{
    fprintf(fp, "section .data          ; Section for initialized data\n");
    translateGlobalVar(ast, fp);
    fprintf(fp,"\n");
   
    fprintf(fp, "section .text          ; Section for code\n");
    fprintf(fp, "    global _start      ; Make the _start symbol available to the linker\n");
    fprintf(fp, "\n");
    fprintf(fp, "_start: \n");
    translate(ast, context, fp); // translate code;
    fprintf(fp, "; Exit the program\n");
    fprintf(fp, "    mov eax, 1         ; syscall number for sys_exit\n");
    fprintf(fp, "    xor ebx, ebx       ; exit code 0\n");
    fprintf(fp, "    int 0x80           ; make syscall\n");
    generateLabels(context, fp);
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
            fprintf(fp, "    mov [ebp + %d - %d  ] , eax   ;;%s\n", offset, entry->symbolOffset, entry->token.lexeme);
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
                fprintf(fp, "     inc dword [%s]\n", ast->token.lexeme);
                translate(ast->left, context, fp);
                // store the value of identifer to eax
            }
            else
            {
                int offset = getSymbolOffset(context, entry);
                fprintf(fp, "    inc dword [ebp + %d - %d  ]     ;; %s\n", offset, entry->symbolOffset, entry->token.lexeme);
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
                fprintf(fp, "     inc dword [%s]\n", ast->token.lexeme);

                // store the value of identifer to eax
            }
            else
            {
                int offset = getSymbolOffset(context, entry);
                translate(ast->right, context, fp);
                fprintf(fp, "    inc dword [ebp + %d - %d  ]     ;; %s\n", offset, entry->symbolOffset, entry->token.lexeme);
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
                fprintf(fp, "     dec dword [%s]\n", ast->token.lexeme);
                translate(ast->left, context, fp);
                // store the value of identifer to eax
            }
            else
            {
                int offset = getSymbolOffset(context, entry);
                fprintf(fp, "    dec dword [ebp + %d - %d  ]     ;; %s\n", offset, entry->symbolOffset, entry->token.lexeme);
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
                fprintf(fp, "     inc dword [%s]\n", ast->token.lexeme);

                // store the value of identifer to eax
            }
            else
            {
                int offset = getSymbolOffset(context, entry);
                translate(ast->right, context, fp);
                fprintf(fp, "    inc dword [ebp + %d - %d  ]     ;; %s\n", offset, entry->symbolOffset, entry->token.lexeme);
            }
            break;
        }
    }
    case VAR:
    {
        if (ast->right->token.value == ASSIGN) // decleration with assignment
        {

            SymbolTableEntry *entry = checkSymbolEntry(context, ast->right->left->token);
            if (entry->scope != 1)
            { // means variable is local we have to allocate it meemory in stack first then assign value
              // TODO allocate different size of memeory based on type of symbol; for int and float = 4byte char = 2byte
                fprintf(fp, "    sub esp , 4\n");
                // for integers and float// for integer and float
            }
            translate(ast->right, context, fp);
            break;
        }
        // decleration only
        SymbolTableEntry *entry = checkSymbolEntry(context, ast->right->token);
        if (entry->scope == 1)
        {
            // global variable
            break;
        }
        // allocate  memory in stack for local variables
        // TODO allocate different size of memeory based on type of symbol; for int and float = 4byte char = 2byte
        fprintf(fp, "    sub esp, 4\n"); // for integers// for integer and float
    }

    break;
    case IF:
        translate(ast->left, context, fp);
        // prase  condition first
        char *label = label_generate(); // generate label for  this if block

        fprintf(fp, "    pop eax\n");        // get value of expression from stack
        fprintf(fp, "    test eax , eax\n"); // check if value is non zero
        fprintf(fp, "    jnz %s      ;; jump if expression  is zero\n", label);
        pushASTQnodeInQueue(context->astQueue, ast->right, label);

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
        char *label = label_generate();
        translate(ast->left, context, fp);
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    test eax , eax\n");
        fprintf(fp, "   jnz %s\n", label);
        pushASTQnodeInQueue(context->astQueue, ast, label);
        // assert(0 && "TODO: WHILE is not implemented");
    }
    break;
    case LESS_THAN:
    {
        translate(ast->left, context, fp);  // parse left expression
        translate(ast->right, context, fp); // parse right expression
        fprintf(fp, "    pop eax\n");       // right oprand
        fprintf(fp, "    pop ebx\n");       // left oprand
        fprintf(fp, "    cmp ebx , eax\n");
        fprintf(fp, "    setl al\n");
        fprintf(fp, "    movzx eax , al\n");
        fprintf(fp, "    push eax\n");
    }
    break;
    case GRATER_THAN:
    {
        translate(ast->left, context, fp);  // parse left expression
        translate(ast->right, context, fp); // parse right expression
        fprintf(fp, "    pop eax\n");       // right oprand
        fprintf(fp, "    pop ebx\n");       // left oprand
        fprintf(fp, "    cmp ebx , eax\n");
        fprintf(fp, "    setg al\n");
        fprintf(fp, "    movzx eax , al\n");
        fprintf(fp, "    push eax\n");
    }
    break;
    case LESS_THAN_EQTO:
    {
        translate(ast->left, context, fp);  // parse left expression
        translate(ast->right, context, fp); // parse right expression
        fprintf(fp, "    pop eax\n");       // right oprand
        fprintf(fp, "    pop ebx\n");       // left oprand
        fprintf(fp, "    cmp ebx , eax\n");
        fprintf(fp, "    setle al\n");
        fprintf(fp, "    movzx eax , al\n");
        fprintf(fp, "    push eax\n");
    }
    break;
    case GRATER_THAN_EQTO:
    {
        translate(ast->left, context, fp);  // parse left expression
        translate(ast->right, context, fp); // parse right expression
        fprintf(fp, "    pop eax\n");       // right oprand
        fprintf(fp, "    pop ebx\n");       // left oprand
        fprintf(fp, "    cmp ebx , eax\n");
        fprintf(fp, "    setge al\n");
        fprintf(fp, "    movzx eax , al\n");
        fprintf(fp, "    push eax\n");
    }
    break;
    case EQUALTO:
    {
        translate(ast->left, context, fp);  // parse left expression
        translate(ast->right, context, fp); // parse right expression
        fprintf(fp, "    pop eax\n");       // right oprand
        fprintf(fp, "    pop ebx\n");       // left oprand
        fprintf(fp, "    cmp ebx , eax\n");
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
        fprintf(fp, "    cmp ebx , eax\n");
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
            int offset = getSymbolOffset(context, entry);
            fprintf(fp, "    mov eax , [ebp + %d - %d  ]     ;; %s\n", offset, entry->symbolOffset, entry->token.lexeme);
        }
        fprintf(fp, "    push eax\n");
    }
    break;
    case CONSTANT:
        fprintf(fp, "    mov eax, %s\n", ast->token.lexeme);
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
        fprintf(fp, "    push rbp                ; Save base pointer\n");
        fprintf(fp, "    mov rbp, rsp            ; Establish new base pointer\n");
        ASTNode *args = ast->right->right;

        int offset = 4;
        while (args != NULL)
        {
            translate(args, context, fp);
            fprintf(fp, "    pop eax\n");
            fprintf(fp, "    mov [ebp - %d] , eax       ;; args no:- %d\n", offset, offset / 4);
            args = args->next;
            offset = offset + 4;
        }
        fprintf(fp, "    call  %s\n", ast->left->token.lexeme);
    }
    break;
    case PARAM_START:
    {
        SymbolTableEntry *entry = getSymboTableFromQueue(context);
        pushSymbolTable(context->symbolTableStack, entry);
        pushSymbolTable(context->symbolTableTempStack, entry);
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

        fprintf(fp, "    push rbp                ; Save base pointer\n");
        fprintf(fp, "    mov rbp, rsp            ; Establish new base pointer\n");
        SymbolTable *symboltable = getSymboTableFromQueue(context);
        pushSymbolTable(context->symbolTableStack, symboltable);
    }
    break;
    case BODYEND:
    {
        fprintf(fp, "    mov rsp, rbp            ; Restore the stack pointer\n");
        fprintf(fp, "    pop rbp                 ; Restore the base pointer\n");
        SymbolTable *symbolTable = popSymbolTable(context->symbolTableStack);
        pushSymbolTable(context->symbolTableTempStack, symbolTable);
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
        fprintf(fp, "%s\n", current->label);
        if (current->ast->token.value == WHILE)
        {

            translate(current->ast->right, context, fp);
            translate(current->ast->left, context, fp);
            fprintf(fp, "    pop eax\n");
            fprintf(fp, "    test eax , eax\n");
            fprintf(fp, "    jzn %s\n", current->label);
        }
        else
        {
            translate(current->ast, context, fp);
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