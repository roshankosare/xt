#include "../../include/asm/asm.h"
#include "../../include/parser/helper.h"
#include "../../include/symboltable/functiontable.h"
#include "../../include/symboltable/symboltable.h"
#include <assert.h>

void translateGlobalVar(ASTNode *ast, FILE *fp);

void createASMFile(ASTNode *ast, Context *context, FILE *fp)
{
    fprintf(fp, "section .data          ; Section for initialized data\n");
    translateGlobalVar(ast, fp);
    fprintf(fp, "section .bss           ; Section for uninitialized data\n");
    fprintf(fp, "section .text          ; Section for code\n");
    fprintf(fp, "    global _start      ; Make the _start symbol available to the linker");
    fprintf(fp, "\n");
    fprintf(fp, " _start: \n");
    translate(ast, context, fp); // translate code;
    fprintf(fp, "; Exit the program\n");
    fprintf(fp, "    mov eax, 1         ; syscall number for sys_exit\n");
    fprintf(fp, "    xor ebx, ebx       ; exit code 0\n");
    fprintf(fp, "    int 0x80           ; make syscall\n");
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
        fprintf(fp, "    add eax ,ebx\n");
        fprintf(fp, "    push eax\n");
        break;
    case MUL:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        fprintf(fp, "    ;; mul\n");
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop ebx\n");
        fprintf(fp, "    imul eax ,ebx\n");
        fprintf(fp, "    push eax\n");
        break;
    case DIV:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        fprintf(fp, "    ;; mul\n");
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop ebx\n");
        fprintf(fp, "    idiv eax ,ebx\n");
        fprintf(fp, "    push eax\n");
        break;
    case MINUS:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        fprintf(fp, "    ;; plus\n");
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop ebx\n");
        fprintf(fp, "    sub eax ,ebx\n");
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
            fprintf(fp, "    mov [%s], eax\n", ast->left->token.lexeme);
            fprintf(fp, "    push eax\n"); // store the to address of identifier
        }
        else
        {
            int offset = getSymbolOffset(context, entry);
            fprintf(fp, "    mov [ebp + %d - %d  ], eax\n", offset, entry->symbolOffset);
            fprintf(fp, "    push eax\n");
        }
    }
    break;

    case INC:
        assert(0 && "TODO: INC is not implemented");
    case DEC:
        assert(0 && "TODO: DEC is not implemented");
    case VAR:
    {
        if (ast->right->token.value == ASSIGN) // decleration with assignment
        {
            SymbolTableEntry *entry = checkSymbolEntry(context, ast->right->left->token);
            if (entry->scope != 1)
            { // means variable is local we have to allocate it meemory in stack first then assign value
              // TODO allocate different size of memeory based on type of symbol; for int and float = 4byte char = 2byte
                fprintf(fp, "    sub esp, 4\n");
                break;
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
        fprintf(fp, "    sub esp, 4\n");       // for integers// for integer and float
    }

    break;
    case BLOCK_VAR:
        assert(0 && "TODO: BLOCK_VAR is not implemented");
    case IF:
        translate(ast->left, context, fp); // prase condition first
        translate(ast->right, context, fp);
        break;
    case ELSE:
        assert(0 && "TODO: ELSE is not implemented");
    case FOR:
        assert(0 && "TODO: FOR is not implemented");
    case RETURN:
        assert(0 && "TODO: RETURN is not implemented");
    case WHILE:
        assert(0 && "TODO: WHILE is not implemented");
    case LESS_THAN:
        assert(0 && "TODO: LESS_THAN is not implemented");
    case GRATER_THAN:
        assert(0 && "TODO: GRATER_THAN is not implemented");
    case LESS_THAN_EQTO:
        assert(0 && "TODO: LESS_THAN_EQTO is not implemented");
    case GRATER_THAN_EQTO:
        assert(0 && "TODO: GRATER_THAN_EQTO is not implemented");
    case EQUALTO:
        assert(0 && "TODO: EQUALTO is not implemented");
    case IDENTIFIER:
    {
        SymbolTableEntry *entry = checkSymbolEntry(context, ast->token);
        if (entry->scope == 1)
        { // this is global var
            fprintf(fp, "    mov eax, [%s]", ast->left->token.lexeme);
            break; // store the value of identifer to eax
        }
        else
        {
            int offset = getSymbolOffset(context, entry);
            fprintf(fp, "    mov eax, [ebp + %d - %d  ]", offset, entry->symbolOffset);
        }
        fprintf(fp, "    push eax\n");
    }
        return;
    case CONSTANT:
        fprintf(fp, "    mov eax, %s\n", ast->token.lexeme);
        fprintf(fp, "    push eax\n");
        return;
    case FUNCTION:
        assert(0 && "TODO: FUNCTION is not implemented");
    case CONDITIONAL_TOKEN:
        assert(0 && "TODO: CONDITIONAL_TOKEN is not implemented");
    case LOGICAL_AND:
        assert(0 && "TODO: LOGICAL_AND is not implemented");
    case LOGICAL_OR:
        assert(0 && "TODO: LOGICAL_OR is not implemented");
    case LOGICAL_NOT:
        assert(0 && "TODO: LOGICAL_NOT is not implemented");

    case BODYSTART:
    {

        fprintf(fp, "    rbp                     ; Save base pointer\n");
        fprintf(fp, "    mov rbp, rsp            ; Establish new base pointer\n");
        SymbolTable *symboltable = getSymboTableFromQueue(context);
        pushSymbolTable(context->symbolTableStack, symboltable);
        translate(ast->right, context, fp);
        break;
    }
    case BODYEND:
    {
        fprintf(fp, "    mov rsp, rbp            ; Restore the stack pointer\n");
        fprintf(fp, "    pop rbp                 ; Restore the base pointer\n");
        SymbolTable *symbolTable = popSymbolTable(context->symbolTableStack);
        free(symbolTable);
        break;
    }

    default:
        printf("\nERROR: %s token handle not implemented", ast->token.lexeme);
        exit(1);
    }

    translate(ast->next, context, fp);
}