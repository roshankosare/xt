
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../../includes/lexer/lexer.h"

Token *tokenize(FILE *fp, int *tokenCount)
{
    Token *tokens = (Token *)malloc(200 * sizeof(Token)); // Initial assumption of 100 tokens

    // TODO :: dynamically allow to creates tokens at run time based number of tokens in program
    if (!tokens)
    {
        printf("Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    char buffer[100]; // Assuming maximum token length
    int count = 0;
    int line = 1;
    int col = 1;
    char ch;
    int i = 0;
    int start = 1;
    int isspace_token = 1;
    while ((ch = fgetc(fp)) != EOF || (i != 0 || start == 1))
    {
        start = 0;

        if (ch == '\n')
        {
            line++;
            col = 0;
            continue;
        }
        if (isspace(ch) && isspace_token)
        {
            col++;
            continue;
        }
        col++;

        buffer[i++] = ch;
        isspace_token = 0;
        if (((ispunct(ch) || isspace(ch) || ch == EOF) && i > 1) && ch != '\"' && ch != '\'')
        {
            /* This block of code will Run if Token is IDENTIFIER | KEYWORD | INTIGER CONSTANT
               This Code will Execute if character is space or pucntuation that means end of token*/
            buffer[--i] = '\0';

            strcpy(tokens[count].lexeme, buffer);
            tokens[count].pos.col = col - i;
            tokens[count].pos.line = line;
            i = 0;

            // The Following block will run if token is  keyword
            if (isKeywordToken(tokens[count].lexeme))
            {
                fillKeywordTokenValue(&tokens[count]);
                if (ch != EOF)
                {
                    fseek(fp, -1, SEEK_CUR);
                    col--;
                }

                isspace_token = 1;
                count++;
                continue;
            }
            // The following block will run if token is Identifier
            if (isIdentifierToken(tokens[count].lexeme))
            {
                tokens[count].value = IDENTIFIER;
                if (ch != EOF)
                {
                    fseek(fp, -1, SEEK_CUR);
                    col--;
                }
                isspace_token = 1;
                count++;
                continue;
            }
            // The following block will run if token is Integer Constant
            if (isConstantToken(tokens[count].lexeme))
            {
                tokens[count].value = CONSTANT;
                if (ch != EOF)
                {
                    fseek(fp, -1, SEEK_CUR);
                    col--;
                }
                isspace_token = 1;
                count++;
                continue;
            }

            // If none of the above block runs that means token is unknown
            tokens[count].value = UNKNOWN;
            fseek(fp, -1, SEEK_CUR);

            isspace_token = 1;
            count++;
            continue;
        }

        /* The Following block of code will check if Token is Punctuation token
           It checks if character is not  " or ' as beacause they are separate for string constant
           it checks for i == 1  value because it will help to determine there is only one
           character in buffer and that should be pucntuation character */

        if (ispunct(ch) && i == 1 && ch != '\"' && ch != '\'' && !isOperator(ch))
        {
            // this block will run if token is punctuation

            buffer[i] = '\0';

            strcpy(tokens[count].lexeme, buffer);
            tokens[count].pos.col = col - i;
            tokens[count].pos.line = line;
            i = 0;
            if (isPunctuationToken(tokens[count].lexeme))
            {

                fillPunctuationTokenValue(&tokens[count]);
                isspace_token = 1;
                count++;
                continue;
            }
            tokens[count].value = UNKNOWN;

            // printf("\nPucntuation: %s", buffer);
            isspace_token = 1;
            count++;
            continue;
        }

        /* This block will run if Token is operator By checking if character is punctuation character
           and its operator token also*/
        if (ispunct(ch) && i == 1 && ch != '\"' && ch != '\'' && isOperator(ch))
        {

            // this block will run if token is operetor
            // loop to save operator token as operator tokens can have grater value than one
            while ((ch = fgetc(fp)) != EOF && !(isalnum(ch) || isspace(ch)) && isOperator(ch))
            {
                buffer[i++] = ch;
            }
            buffer[i] = '\0';
            i = 0;
            strcpy(tokens[count].lexeme, buffer);
            tokens[count].pos.col = col - i;
            tokens[count].pos.line = line;

            fseek(fp, -1, SEEK_CUR);
            col--;
            i = 0;
            if (isOperatorToken(tokens[count].lexeme))
            {
                fillOperatorTokenValue(&tokens[count]);
                isspace_token = 1;
                count++;
                continue;
            }
            tokens[count].value = UNKNOWN;

            // printf("\nOperator: %s", buffer);
            isspace_token = 1;
            count++;
            continue;
        }

        /*This block of code will run to identify the string constants in program like "hello world" | 'hello world*/

        if ((ch == '\"' || ch == '\'') && i == 1)
        {
            while ((ch = fgetc(fp)) != EOF && (ch != '\'' && ch != '\"'))
            {
                buffer[i++] = ch;
                col++;
            }

            // this if block will run if token is string constant
            buffer[i++] = ch;
            buffer[i] = '\0';

            strcpy(tokens[count].lexeme, buffer);
            tokens[count].pos.col = col - i;
            tokens[count].pos.line = line;
            i = 0;

            if (isConstantToken(tokens[count].lexeme))
            {
                tokens[count].value = CONSTANT;
                isspace_token = 1;
                count++;
                continue;
            }
            tokens[count].value = UNKNOWN;
            isspace_token = 1;
            count++;
            continue;
        }
    }
    strcpy(tokens[count].lexeme, "EOF");
    tokens[count].value = TEOF;

    *tokenCount = count;
    int program_ok = 1;
    for (int j = 0; j < count; j++)
    {
        if (tokens[j].value == UNKNOWN)
        {
            printf("\nError: unknown token at line: %d col: %d  value: '%s' \n", tokens[j].pos.line, tokens[j].pos.col, tokens[j].lexeme);
            program_ok = 0;
        }
    }

    if (!program_ok)
        exit(1);

    return tokens;
}

void printTokens(Token *tokens, int tokenCount)
{

    for (int i = 0; i < tokenCount; i++)
    {
        printf("\nLexme: %s ", tokens[i].lexeme);
        printf("\nValue: %s", getTokenStringValue(tokens[i].value));
        printf("\n----------------------------------------------------------");
    }
}