#include "../../include/asm/asmexp.h"
#include "../../include/parser/helper.h"
#include "../../include/asm/helper.h"

void print_identifier(ASTNode *ast, Context *context, FILE *fp)
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
    return;
}
void print_string_constant(ASTNode *ast, Context *context, FILE *fp)
{
    char *litral_label = label_generate();
    char *label = malloc(100 * sizeof(char));
    snprintf(label, 100, "    %s: db %s, 0\n", litral_label,ast->token.lexeme);
    print_rodata_section(fp, label);
    int string_length = strlen(ast->token.lexeme);
   
    fprintf(fp, "    mov bl , [STRING_TYPE]\n");
    fprintf(fp, "    movzx bx , bl\n");
    fprintf(fp, "    push bx\n");
    fprintf(fp, "    lea eax, [%s]              ;; Load the address of litral into eax\n", litral_label);

    fprintf(fp, "    push eax\n");
    return;
}
void print_integer_constant(ASTNode *ast, Context *context, FILE *fp)
{
    fprintf(fp, "    mov bl , [INT_TYPE]\n");
    fprintf(fp, "    movzx bx , bl\n");
    fprintf(fp, "    push bx\n");
    fprintf(fp, "    mov eax, %s\n", ast->token.lexeme);
    fprintf(fp, "    push eax\n");
    return;
}
void print_float_constant(ASTNode *ast, Context *context, FILE *fp)
{

    fprintf(fp, "    mov bl , [FLOAT_TYPE]\n");
    fprintf(fp, "    movzx bx , bl\n");
    fprintf(fp, "    push bx\n");
    fprintf(fp, "    mov eax, %s\n", ast->token.lexeme);
    fprintf(fp, "    push eax\n");
    return;
}
void print_hex_constant(ASTNode *ast, Context *context, FILE *fp)
{
    fprintf(fp, "    mov bl , [INT_TYPE]\n");
    fprintf(fp, "    movzx bx , bl\n");
    fprintf(fp, "    push bx\n");
    fprintf(fp, "    mov eax, %s\n", ast->token.lexeme);
    fprintf(fp, "    push eax\n");
    return;
}

void print_add(FILE *fp)
{
    fprintf(fp, "    ;; plus\n");
    fprintf(fp, "    pop eax\n");
    fprintf(fp, "    pop cx \n");
    fprintf(fp, "    pop ebx\n");
    fprintf(fp, "    pop dx \n");
    fprintf(fp, "    add eax , ebx\n");
    fprintf(fp, "    push cx\n");
    fprintf(fp, "    push eax\n");
    return;
}
void print_sub(FILE *fp)
{
    fprintf(fp, "    ;; MINUS\n");
    fprintf(fp, "    pop eax\n");
    fprintf(fp, "    pop cx \n");
    fprintf(fp, "    pop ebx\n");
    fprintf(fp, "    pop dx \n");
    fprintf(fp, "    sub eax , ebx\n");
    fprintf(fp, "    push cx\n");
    fprintf(fp, "    push eax\n");
    return;
}
void print_mul(FILE *fp)
{
    fprintf(fp, "    ;; MUL\n");
    fprintf(fp, "    pop eax\n");
    fprintf(fp, "    pop cx \n");
    fprintf(fp, "    pop ebx\n");
    fprintf(fp, "    pop dx \n");
    fprintf(fp, "    imul eax , ebx\n");
    fprintf(fp, "    push cx\n");
    fprintf(fp, "    push eax\n");
    return;
}
void print_div(FILE *fp)
{
    fprintf(fp, "    ;; DIV\n");
    fprintf(fp, "    pop ebx\n");
    fprintf(fp, "    pop cx \n");
    fprintf(fp, "    pop eax\n");
    fprintf(fp, "    pop cx \n");
    fprintf(fp, "    cdq\n");
    fprintf(fp, "    idiv ebx\n");
    fprintf(fp, "    push cx\n");
    fprintf(fp, "    push eax\n");
    return;
}

void print_modulas(FILE *fp)
{

    fprintf(fp, "    pop ebx\n");
    fprintf(fp, "    pop cx \n");
    fprintf(fp, "    pop eax\n");
    fprintf(fp, "    pop cx \n");
    // fprintf(fp, "    cdq\n");
    fprintf(fp, "    xor edx ,edx\n");
    fprintf(fp, "    div ebx\n");
    fprintf(fp, "    push cx\n");
    fprintf(fp, "    push edx\n");
    return;
}
void print_inc(ASTNode *ast, Context *context, FILE *fp)
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
        return;
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
        return;
    }
}
void print_dec(ASTNode *ast, Context *context, FILE *fp)
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
        return;
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
        return;
    }
}
void print_valueat(FILE *fp)
{
    fprintf(fp, "    pop eax\n");
    fprintf(fp, "    pop bx\n");
    fprintf(fp, "    mov eax , [eax]             ;; mov value to eax from address stored at eax\n");
    fprintf(fp, "    push bx\n");
    fprintf(fp, "    push eax\n");
    return;
}

void print_typeof(FILE *fp)
{
    fprintf(fp, "    pop eax\n");
    fprintf(fp, "    pop bx\n");
    fprintf(fp, "    movzx eax , bx \n");
    fprintf(fp, "    mov bx , [INT_TYPE]\n");
    fprintf(fp, "    push bx\n");
    fprintf(fp, "    push eax\n");
    return;
}

void print_less_than(FILE *fp)
{
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
    return;
}
void print_grater_than(FILE *fp)
{
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
    return;
}
void print_lessthan_eqto(FILE *fp)
{
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
    return;
}
void print_graterthan_eqto(FILE *fp)
{
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
    return;
}
void print_eqto(FILE *fp)
{
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
    return;
}
void print_noteqto(FILE *fp)
{
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
    return;
}

void print_logical_and(FILE *fp)
{
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
    return;
}
void print_logigcal_or(FILE *fp)
{
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
    return;
}
void print_logical_not(FILE *fp)
{
    fprintf(fp, "    pop eax\n");
    fprintf(fp, "    pop bx\n");
    fprintf(fp, "    test eax , eax\n");
    fprintf(fp, "    sete al\n");
    fprintf(fp, "    movzx eax , al");
    fprintf(fp, "    mov bl , [INT_TYPE]\n");
    fprintf(fp, "    movzx bx , bl\n");
    fprintf(fp, "    push bx\n");
    fprintf(fp, "    push eax\n");
    return;
}

void print_bitwise_and(FILE *fp)
{
    fprintf(fp, "    pop eax\n");
    fprintf(fp, "    pop cx \n");
    fprintf(fp, "    pop ebx\n");
    fprintf(fp, "    pop dx \n");
    fprintf(fp, "    and eax ,ebx\n");
    fprintf(fp, "    mov bl , [INT_TYPE]\n");
    fprintf(fp, "    movzx bx , bl\n");
    fprintf(fp, "    push bx\n");
    fprintf(fp, "    push eax\n");
    return;
}
void print_bitwise_or(FILE *fp)
{
    fprintf(fp, "    pop eax\n");
    fprintf(fp, "    pop cx \n");
    fprintf(fp, "    pop ebx\n");
    fprintf(fp, "    pop dx \n");
    fprintf(fp, "    or eax ,ebx\n");
    fprintf(fp, "    mov bl , [INT_TYPE]\n");
    fprintf(fp, "    movzx bx , bl\n");
    fprintf(fp, "    push bx\n");
    fprintf(fp, "    push eax\n");
    return;
}
void print_bitwise_not(FILE *fp)
{
    return;
}

void print_funtion_call(ASTNode *ast, Context *context, FILE *fp)
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
    return;
}
