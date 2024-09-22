#include "../../include/asm/asmstmt.h"
#include "../../include/parser/helper.h"
#include "../../include/parser/stmt.h"
#include "../../include/asm/asm.h"
#include "../../include/tokens/tokenizer.h"
#include <stdio.h>
#include <stdlib.h>

void print_if(ASTNode *ast, Context *context, FILE *fp)
{
    if (ast->left->token.value == YES)
    {
        fprintf(fp, "    pop eax\n");        // get value of expression from stack
        fprintf(fp, "    test eax , eax\n"); // check if value is non zero
        fprintf(fp, "    jz .%s                 ;; jump if expression  is not  zero\n", ast->left->token.lexeme);
        return;
    }
    if (ast->left->token.value == NO)
    {
        fprintf(fp, "     jmp .%s                        ;; defination of false label \n", ast->left->token.lexeme);
    }
}

void print_else(ASTNode *ast, Context *context, FILE *fp)
{
    if (ast->left->token.value == YES)
    {
        fprintf(fp, ".%s:\n", ast->left->token.lexeme);
    }

    if (ast->left->token.value == NO)
    {
        fprintf(fp, ".%s:\n", ast->left->token.lexeme);
    }
}

void print_while(ASTNode *ast, Context *context, FILE *fp)
{
    if (ast->left->token.value == YES)
    {
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop bx\n");
        fprintf(fp, "    mov [condition] , bx\n");
        fprintf(fp, "    mov [condition + 1] , eax\n");

        fprintf(fp, "    lea eax , [.%s]\n", ast->left->next->token.lexeme);
        fprintf(fp, "    push eax\n");
        // fprintf(fp, "    call print_eax\n");
        fprintf(fp, "    mov eax , [condition + 1]\n");
        fprintf(fp, "    test eax , eax\n");
        fprintf(fp, "    jz .%s\n", ast->left->next->token.lexeme);
        fprintf(fp, ".%s:\n", ast->left->token.lexeme);
        fprintf(fp, "    pop eax                        ;; pop the return address to eax\n");
        fprintf(fp, "    call push_call                 ;; store the return address to ra location\n");
        fprintf(fp, "    call push_lcall\n");

        fprintf(fp, "    lea eax , [.%s]\n", ast->left->token.lexeme);
        fprintf(fp, "    call push_lcall\n");
        fprintf(fp, "    mov eax , [pebp]\n");
        fprintf(fp, "    call push_lbase\n");
        fprintf(fp, "    mov eax , [pesp]\n");
        fprintf(fp, "    call push_lbase\n");
        return;
    }

    if (ast->left->token.value == NO)
    {
        fprintf(fp, "    pop eax\n");
        fprintf(fp, "    pop bx\n");
        fprintf(fp, "    mov [condition] , bx\n");
        fprintf(fp, "    mov [condition + 1] , eax\n");
        // fprintf(fp, "    call print_eax\n");
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
        fprintf(fp, "    jnz .%s\n", ast->left->next->token.lexeme);
        fprintf(fp, ".%s:                        ;; defination of false label \n", ast->left->token.lexeme);
    }

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
            fprintf(fp, "    sub dword [pesp] , 5\n");
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
        fprintf(fp, "    sub dword [pesp] , 5\n");
    }
    return;
}
void print_function(ASTNode *ast, Context *context, FILE *fp)
{
    char *buffer = NULL;
    long file_size;
    int insert_pos = -1;
    FILE *temp_fp = NULL;
    long original_pos = ftell(fp); // Save the original file pointer position

    // Get file size
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    rewind(fp);

    // Allocate buffer to read file contents
    buffer = malloc(file_size + 1);
    if (buffer == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Read the file into the buffer
    fread(buffer, file_size, 1, fp);
    buffer[file_size] = '\0';

    // Find the 'global _start' in the file
    char *global_start_ptr = strstr(buffer, "global _start");
    if (global_start_ptr)
    {
        // Find the location after 'global _start'
        insert_pos = global_start_ptr - buffer + strlen("global _start\n");
    }

    if (insert_pos != -1)
    {
        // Create a temporary file
        temp_fp = tmpfile();
        if (temp_fp == NULL)
        {
            fprintf(stderr, "Could not create temporary file\n");
            free(buffer);
            return;
        }

        // Write content before the insert position to the temp file
        fwrite(buffer, 1, insert_pos, temp_fp);

        fprintf(temp_fp, "\n%s:\n", ast->left->token.lexeme);
        fprintf(temp_fp, "    pop eax                        ;; pop the return address to eax\n");
        fprintf(temp_fp, "    call push_call                 ;; store the return address to ra location\n");
        fprintf(temp_fp, "    call push_fcall\n");
        fprintf(temp_fp, "    mov eax , [pebp]\n");
        fprintf(temp_fp, "    call push_base\n");   // push base pebp base pointer
        fprintf(temp_fp, "    mov eax , [pesp]\n"); // push base pesp stack pointer
        fprintf(temp_fp, "    call push_base\n");
        parse_block(context, temp_fp);

        fprintf(temp_fp, "    call pop_base\n"); // stack pointer
        fprintf(temp_fp, "    mov [pesp] , eax\n");
        fprintf(temp_fp, "    call pop_base\n"); // base pointer
        fprintf(temp_fp, "    mov [pebp] , eax \n");

        fprintf(temp_fp, "    call pop_call                  ;; store the return address to eax\n");
        fprintf(temp_fp, "    call pop_fcall\n");
        fprintf(temp_fp, "    jmp eax                        ;; jmp to return address\n");

        // Write the remaining original content
        fwrite(buffer + insert_pos, 1, file_size - insert_pos, temp_fp);

        // Rewind the temporary file to the beginning
        rewind(temp_fp);

        // Reopen the original file in write mode to overwrite its content
        freopen(NULL, "w+", fp);

        // Copy the temporary file content back to the original file
        char temp_buffer[1024];
        size_t n;
        while ((n = fread(temp_buffer, 1, sizeof(temp_buffer), temp_fp)) > 0)
        {
            fwrite(temp_buffer, 1, n, fp);
        }

        // Close the temporary file
        fclose(temp_fp);
    }
    else
    {
        fprintf(stderr, "Error: 'global _start' not found\n");
    }

    // Clean up
    free(buffer);

    // Restore the original file pointer position
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
    char *loopLabel = label_generate();
    fprintf(fp, "    call pop_lbase\n");
    fprintf(fp, "    mov [pesp] , eax\n");
    fprintf(fp, "    call pop_lbase\n");
    fprintf(fp, "    mov [pebp] , eax\n");

    // fprintf(fp, "    lea eax , [%s]\n", );
    fprintf(fp, "    call pop_lcall\n"); // pop while loop address
    fprintf(fp, "    push eax\n");
    fprintf(fp, "    call pop_lcall\n"); // address of instruction after while loop
    fprintf(fp, "    mov [RETURN_ADDRESS] , eax\n");
    fprintf(fp, ".%s:                                 ;; function to clean up call stack \n", loopLabel);
    fprintf(fp, "    call pop_call\n");
    fprintf(fp, "    mov [POPED_ADDRESS] , eax\n");
    fprintf(fp, "    mov eax ,[POPED_ADDRESS]\n");
    fprintf(fp, "    cmp eax, [RETURN_ADDRESS]\n");
    fprintf(fp, "    jne .%s\n", loopLabel);
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
    return;
}
void print_body_end(ASTNode *ast, Context *context, FILE *fp)
{
    fprintf(fp, "    mov eax , [pebp]                   ;; store the value at pebp to eax\n");
    fprintf(fp, "    mov [pesp] , eax                ;; restore the stack pointer\n");
    fprintf(fp, "    call pop_stack                     ;; pop stack top to eax\n");
    fprintf(fp, "    mov [pebp] , eax                    ; restore the base pointer\n");
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
