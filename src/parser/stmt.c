#include "../../include/parser/stmt.h"
#include "../../include/parser/helper.h"
#include "../../include/parser/ast.h"
#include "../../include/parser/exp.h"
#include "../../include/symboltable/symboltable.h"
#include "../../include/symboltable/functiontable.h"
#include "../../include/lexer/lexer.h"
#include "../../include/asm/asm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// SELC_STMT :=  IF_STMT
//  IF_STMT := "if" "(" exp | CONDITIONAL_STMT ")" BLOCK_STMT | EXP

ASTNode *parseASTString(Context *context, FILE *fp);

void stmt(Context *context, FILE *fp)
{
    ASTNode *node;
    if (match(context, IF))
    {
        selc_stmt(context, fp);
        return;
    }
    if (match(context, WHILE))
    {
        iter_stmt(context, fp);
        return;
    }
    if (match(context, CONTINUE))
    {
        if (context->loopStack == -1)
        {
            printf("\nERROR: unexpected 'continue' without loop\n");
            exit(0);
        }
        node = createASTNode(getCurrentToken(context));
        consume(context); // consume "xontinue"
        expect(context, SEMI_COLAN);
        consume(context); // consume ";"
        translate(node, context, fp);
        freeASTNode(node);
        return;
    }
    if (match(context, BREAK))
    {
        if (context->loopStack == -1)
        {
            printf("\nERROR: unexpected 'break' without loop or switch\n");
            exit(0);
        }
        node = createASTNode(getCurrentToken(context));
        consume(context); // consume "xontinue"
        expect(context, SEMI_COLAN);
        consume(context); // consume ";"
        translate(node, context, fp);
        freeASTNode(node);
        return;
    }
    if (match(context, RETURN))
    {
        jump_stmt(context, fp);
        return;
    }
    if (match(context, VAR))
    {
        dec_stmt(context, fp);
        return;
    }
    if (match(context, IDENTIFIER) || match(context, OPEN_PAREN))
    {
        node = exp_stmt(context);
        translate(node, context, fp);
        freeASTNode(node);
        return;
    }
    if (match(context, INC) || match(context, DEC) || match(context, VALUE_AT) || match(context, BIT_NOT) || match(context, TYPEOF))
    {
        node = exp_stmt(context);
        translate(node, context, fp);
        freeASTNode(node);
        return;
    }
    if (match(context, STRING_CONSTANT) || match(context, INTEGER_CONSTANT) || match(context, FLOAT_CONSTANT) || match(context, HEX_CONSTANT))
    {
        node = exp_stmt(context);
        translate(node, context, fp);
        freeASTNode(node);
        return;
    }

    if (match(context, ASM))
    {
        parseAsm(context, fp);
        return;
    }

    if (match(context, TEOF))
    {
        return NULL;
    }

    printf("\nERROR : unexpected token %s at line %d and col %d \n",
           context->current.lexeme, context->current.pos.line, context->current.pos.col);
    exit(1);

    return NULL;
}

// DEC_STMT := { "var" "IDENTFIER" ";"} | { "var" "IDENTFIER" "=" exp
void dec_stmt(Context *context, FILE *fp)
{
    // DEC_STMT := { "var" "IDENTFIER" ";"}
    ASTNode *varnode = createASTNode(getCurrentToken(context));
    consume(context); // consume "var"
    expect(context, IDENTIFIER);
    Token identifier_token = getCurrentToken(context);
    SymbolTableEntry *entry = insertSymbolEntry(context, identifier_token); // insert symbol in symbol table
    ASTNode *idNode = createASTNode(identifier_token);                      // consume "id"
    consume(context);
    varnode->right = idNode;
    if (match(context, SEMI_COLAN))
    {
        entry->isDefined = 1;
        consume(context); // consume ";"
        translate(varnode, context, fp);
        freeASTNode(varnode);
        return;
    }
    // DEC_STMT := { "var" "IDENTFIER" "=" exp }
    expect(context, ASSIGN);
    ASTNode *assignNode = createASTNode(getCurrentToken(context));
    consume(context); // consume "="
    assignNode->left = idNode;
    assignNode->right = exp(context);
    expect(context, SEMI_COLAN);
    consume(context);
    entry->isDefined = 1;
    varnode->right = assignNode;

    translate(varnode, context, fp);
    freeASTNode(varnode);
    return;
}
void selc_stmt(Context *context, FILE *fp)
{
    if (match(context, ELSE))
    {
        printf("\nERROR : unexpected `else` without `if`\n at line %d and col %d",
               context->current.pos.line, context->current.pos.col);
        exit(1);
    }
    if (match(context, IF))
    {
        ASTNode *ifNode = createASTNode(getCurrentToken(context));
        char *labelElse = label_generate();
        char *labelFalse = label_generate();
        Token ifConToken = {.lexeme = "", .value = YES};
        strcpy(ifConToken.lexeme, labelElse);
        ifNode->left = createASTNode(ifConToken);

        consume(context);

        expect(context, OPEN_PAREN);
        consume(context);
        ASTNode *exp_node = exp(context);
        translate(exp_node, context, fp);

        freeASTNode(exp_node);

        expect(context, CLOSE_PAREN);
        consume(context);
        translate(ifNode, context, fp);
        if (match(context, OPEN_CURLY_PAREN))
        {
            parse_block(context, fp);
        }
        else
        {
            stmt(context, fp);
        }

        ifConToken.value = NO;
        strcpy(ifConToken.lexeme, labelFalse);
        ifNode->left = createASTNode(ifConToken);
        translate(ifNode, context, fp);

        freeASTNode(ifNode);

        if (match(context, ELSE))
        {
            Token elseConToken = {.lexeme = "", .value = YES};
            strcpy(elseConToken.lexeme, labelElse);
            ASTNode *elseConNode = createASTNode(elseConToken);
            ASTNode *elseNode = createASTNode(getCurrentToken(context));
            elseNode->left = elseConNode;
            translate(elseNode, context, fp);
            consume(context);
            if (match(context, OPEN_CURLY_PAREN))
            {
                parse_block(context, fp);
            }
            else
            {
                stmt(context, fp);
            }

            elseConToken.value = NO;
            strcpy(elseConToken.lexeme, labelFalse);
            elseNode->left = NULL;
            freeASTNode(elseConNode);
            elseConNode = createASTNode(elseConToken);

            elseNode->left = elseConNode;

            translate(elseNode, context, fp);
            return;
        }

        Token elseNodeToken = {.lexeme = "else", .value = ELSE};
        Token elseConToken = {.lexeme = "", .value = YES};
        strcpy(elseConToken.lexeme, labelElse);
        ASTNode *elseConNode = createASTNode(elseConToken);
        ASTNode *elseNode = createASTNode(elseNodeToken);
        elseNode->left = elseConNode;
        translate(elseNode, context, fp);

        elseConToken.value = NO;
        strcpy(elseConToken.lexeme, labelFalse);
        elseNode->left = NULL;
        freeASTNode(elseConNode);
        elseConNode = createASTNode(elseConToken);

        elseNode->left = elseConNode;

        translate(elseNode, context, fp);
        return;
    }

    return;
}

void iter_stmt(Context *context, FILE *fp)
{
    if (match(context, WHILE))
    {
        context->loopStack += 1;
        ASTNode *whileNode = createASTNode(getCurrentToken(context));
        char *labelTrue = label_generate();
        char *labelFalse = label_generate();

        Token labelTrueToken = {.value = YES};
        strcpy(labelTrueToken.lexeme, labelTrue);
        ASTNode *labelTrueNode = createASTNode(labelTrueToken);

        Token labelFalseToken = {.value = NO};
        strcpy(labelFalseToken.lexeme, labelFalse);
        ASTNode *labelFalseNode = createASTNode(labelFalseToken);

        consume(context);
        expect(context, OPEN_PAREN);
        consume(context);
        ASTNode *exp_condition = exp(context);
        expect(context, CLOSE_PAREN);
        consume(context);
        translate(exp_condition, context, fp);

        labelTrueNode->next = labelFalseNode;
        whileNode->left = labelTrueNode;
        translate(whileNode, context, fp);
        if (match(context, OPEN_CURLY_PAREN))
        {
            parse_block(context, fp);
        }
        else
        {
            stmt(context, fp);
        }
        translate(exp_condition, context, fp);

        labelTrueNode->next = NULL;
        labelFalseNode->next = labelTrueNode;
        whileNode->left = labelFalseNode;
        translate(whileNode, context, fp);
        context->loopStack -= 1;

        labelFalseNode->next = NULL;
        whileNode->left = NULL;
        freeASTNode(whileNode);
        freeASTNode(exp_condition);
        freeASTNode(labelFalseNode);
        freeASTNode(labelTrueNode);
        return;
    }
    return;
}

void jump_stmt(Context *context, FILE *fp)
{
    if (match(context, RETURN))
    {
        ASTNode *returnNode = createASTNode(getCurrentToken(context));
        consume(context);
        if (match(context, SEMI_COLAN))
        {
            consume(context); // consume ";"
            translate(returnNode, context, fp);
            freeASTNode(returnNode);
            return;
        }
        returnNode->left = exp(context);
        expect(context, SEMI_COLAN);
        consume(context);
        translate(returnNode, context, fp);
        freeASTNode(returnNode);
        return;
    }
    return;
}

// BLOCK := "{" EXP  "}"
void parse_block(Context *context, FILE *fp)
{

    expect(context, OPEN_CURLY_PAREN);
    consume(context);
    SymbolTable *symbolTable = initSymbolTable();
    pushSymbolTable(context->symbolTableStack, symbolTable);
    Token body_start = {.lexeme = "Body_start", .value = BODYSTART};
    ASTNode *bodyStartNode = createASTNode(body_start);

    // translate body start   for maneging base pointer and stack pointer
    translate(bodyStartNode, context, fp);

    freeASTNode(bodyStartNode);

    while (context->current.value != CLOSE_CURLY_PAREN && context->current.value != TEOF)
    {
        stmt(context, fp);
    }
    expect(context, CLOSE_CURLY_PAREN);
    consume(context);
    Token body_end_token = {.lexeme = "Body_end", .value = BODYEND};
    ASTNode *body_end = createASTNode(body_end_token);

    // translate body end   for maneging base pointer and stack pointer
    translate(body_end, context, fp);
    freeASTNode(body_end);

    popSymbolTable(context->symbolTableStack);

    return;
}

// ASM := "asm" "(" "[" STRING_ASM "]" ")" ";"
// this function will push assembly instuction directly in asm file to make sys calls
void parseAsm(Context *context, FILE *fp)
{
    if (match(context, ASM))
    {
        ASTNode *asmNode = createASTNode(getCurrentToken(context));
        consume(context); // consume asm token

        expect(context, OPEN_PAREN);
        consume(context); // consume "("
        expect(context, OPEN_SQ_PARAN);
        consume(context); // consume "["
        asmNode->right = parseASTString(context, fp);
        expect(context, CLOSE_SQ_PARAN);
        consume(context); // consume "]"
        expect(context, CLOSE_PAREN);
        consume(context); // consume ")"
        expect(context, SEMI_COLAN);
        consume(context); // consume ";"
        translate(asmNode, context, fp);
        freeASTNode(asmNode);
        return;
    }
    return;
}
ASTNode *parseASTString(Context *context, FILE *fp)
{
    if (match(context, CLOSE_SQ_PARAN))
    {
        return NULL;
    }
    expect(context, STRING_CONSTANT);
    ASTNode *node = createASTNode(getCurrentToken(context));
    consume(context); // consume "string"
    if (match(context, CLOSE_SQ_PARAN))
    {
        return node;
    }
    expect(context, COMMA);
    consume(context); // consume ","
    node->next = parseASTString(context, fp);
    return node;
}
