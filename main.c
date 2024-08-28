#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/tokens/tokens.h"
#include "include/lexer/lexer.h"
#include "include/parser/ast.h"
#include "include/parser/parser.h"
#include "include/asm/asm.h"
#include "include/context/context.h"

#define FLAG 0

#define MAX_LINE_LENGTH 1024
void usage(char *programName)
{
    printf("Usage: %s [options]\n", programName);
    printf("Options:\n");
    printf("  -f <inputfile.xt>  provide file to compiler\n");
    printf("  -o <outputfile.o>  provide file to compiler\n");

    // Add more options as needed

    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{

    /* if args not provide show usage menu*/
    if (argc < 2)
    {
        usage(argv[0]);
    }

    char *inputfile;
    char *outputfile;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc)
        {
            inputfile = argv[i + 1];
            i++;
            // Skip the next argument
        }
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc)
        {
            outputfile = argv[i + 1];
            i++;
        }
        else
        {
            printf("Unknown option: %s\n", argv[i]);
            usage(argv[0]);
        }
    }

    FILE *fp;
    int tokenCount = 0;

    // Open file for reading
    fp = fopen(inputfile, "r");
    if (fp == NULL)
    {
        printf("\nERROR: %s file not found\n", inputfile);
        exit(1);
    }
    Token *tokens = lexer(fp, &tokenCount);

    // Token *tokens = tokenize(fp, &tokenCount);
    // for (int i = 0; i < tokenCount; i++)
    // {
    //     if (tokens[i].value == UNKNOWN)
    //     {
    //         printf("\nERROR: at line %d and col %d [unknown token]-> '%s'\n", tokens[i].pos.line, tokens[i].pos.col, tokens[i].lexeme);
    //         exit(1);
    //     }
    // }

    int index = 0;
    printTokens(tokens, tokenCount);

    Context *context = initContext(tokens);
    SymbolTable *symbolTable = initSymbolTable();
    pushSymbolTable(context->symbolTableStack, symbolTable);
    ASTNode *start = parseProgram(context, tokens, &index, tokenCount);
    fclose(fp);

    printf("\nLexical anaysis completed without any error\n");
    printAST(start, 0);

#ifdef FLAG
    FILE *op;

    char *asmFile = malloc(50 * sizeof(char));
    snprintf(asmFile, 50, "%s.asm", outputfile);

    op = fopen(asmFile, "w+");
    if (fp == NULL)
    {
        printf("\nERROR: %s file cannot be creted\n", inputfile);
        exit(1);
    }
    printf("\n Parsing is completed successfully..\n");
    createASMFile(start, context, op);
    fclose(op);

    char *asmCommand = malloc(50 * sizeof(char));
    char *linkCommand = malloc(50 * sizeof(char));
    // Execute the command using system()

    snprintf(asmCommand, 50, "nasm -f elf32 %s.asm  -o %s.o", outputfile, outputfile);
    snprintf(linkCommand, 50, "ld -m elf_i386  -o %s %s.o", outputfile, outputfile);

    int asmResult = system(asmCommand);
    int linkResult = system(linkCommand);

    if (asmResult != 0)
    {
        fprintf(stderr, "Assembly failed with error code %d\n", asmResult);
        return 1;
    }

    if (linkResult != 0)
    {
        fprintf(stderr, "Linking failed with error code %d\n", linkResult);
        return 1;
    }

#endif

    return 0;
}
