#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"

#define MAX_LINE_LENGTH 1024
void usage(char *programName)
{
    printf("Usage: %s [options]\n", programName);
    printf("Options:\n");
    printf("  -f <inputfile.cl>  provide file to compiler\n");
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
    int tokenCount;

    // Open file for reading
    fp = fopen(inputfile, "r");
    Token *tokens = tokenize(fp, &tokenCount);

    printf("\n token: %d", tokenCount);
    int index = 0;
    printTokens(tokens, tokenCount);
    ASTNode *start = parseProgram(tokens, &index, tokenCount);

    for (int i = 0; i < tokenCount; i++)
    {
        if (tokens[i].value == UNKNOWN)
        {
            printf("\nERROR: at line %d and col %d [unknown token]-> '%s'\n", tokens[i].pos.line, tokens[i].pos.col, tokens[i].lexeme);
            exit(1);
        }
    }

    printf("\nLexical anaysis completed without any error\n");
    printAST(start, 0);

    // Read and print file contents line by line

    return 0;
}
