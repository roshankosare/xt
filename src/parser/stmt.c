#include "../../include/parser/stmt.h"
#include "../../include/parser/helper.h"
#include "../../include/parser/ast.h"
#include "../../include/parser/exp.h"
#include "../../include/symboltable/symboltable.h"
#include "../../include/symboltable/functiontable.h"
// SELC_STMT :=  IF_STMT
//  IF_STMT := "if" "(" exp | CONDITIONAL_STMT ")" BLOCK_STMT | EXP

ASTNode *parseASTString(Context *context);

ASTNode *stmt(Context *context)
{
    ASTNode *node;
    if (match(context, IF))
    {
        node = selc_stmt(context);
        return node;
    }
    if (match(context, WHILE))
    {
        node = iter_stmt(context);
        return node;
    }
    if (match(context, CONTINUE))
    {
        if(context->loopStack == -1){
            printf("\nERROR: unexpected 'continue' without loop\n");
            exit(0);
        }
        node = createASTNode(context->current);
        consume(context); // consume "xontinue"
        expect(context, SEMI_COLAN);
        consume(context); // consume ";"
        return node;
    }
    if (match(context, BREAK))
    {
        if(context->loopStack == -1){
            printf("\nERROR: unexpected 'break' without loop or switch\n");
            exit(0);
        }
        node = createASTNode(context->current);
        consume(context); // consume "xontinue"
        expect(context, SEMI_COLAN);
        consume(context); // consume ";"
        return node;
    }
    if (match(context, RETURN))
    {
        node = jump_stmt(context);
        return node;
    }
    if (match(context, VAR))
    {
        node = dec_stmt(context);
        return node;
    }
    if (match(context, IDENTIFIER) || match(context, OPEN_PAREN))
    {
        node = exp_stmt(context);
        return node;
    }
    if (match(context, INC) || match(context, DEC) || match(context, VALUE_AT) || match(context, BIT_NOT))
    {
        node = exp_stmt(context);
        return node;
    }
    if (match(context, STRING_CONSTANT) || match(context, INTEGER_CONSTANT) || match(context, FLOAT_CONSTANT) || match(context, HEX_CONSTANT))
    {
        node = exp_stmt(context);
        return node;
    }

    if (match(context, ASM))
    {
        node = parseAsm(context);
        return node;
    }

    printf("\nERROR : unexpected token %s at line %d and col %d \n",
           context->current.lexeme, context->current.pos.line, context->current.pos.col);
    exit(1);

    return NULL;
}

// DEC_STMT := { "var" "IDENTFIER" ";"} | { "var" "IDENTFIER" "=" exp
ASTNode *dec_stmt(Context *context)
{
    // DEC_STMT := { "var" "IDENTFIER" ";"}
    ASTNode *varnode = createASTNode(context->current);
    consume(context); // consume "var"
    expect(context, IDENTIFIER);
    Token identifier_token = context->current;
    SymbolTableEntry *entry = insertSymbolEntry(context, identifier_token); // insert symbol in symbol table
    ASTNode *idNode = createASTNode(identifier_token);                      // consume "id"
    consume(context);
    varnode->right = idNode;
    if (match(context, SEMI_COLAN))
    {
        entry->isDefined = 1;
        consume(context); // consume ";"
        return varnode;
    }
    // DEC_STMT := { "var" "IDENTFIER" "=" exp }
    expect(context, ASSIGN);
    ASTNode *assignNode = createASTNode(context->current);
    consume(context); // consume "="
    assignNode->left = idNode;
    assignNode->right = exp(context);
    expect(context, SEMI_COLAN);
    consume(context);
    entry->isDefined = 1;
    varnode->right = assignNode;
    return varnode;
}
ASTNode *selc_stmt(Context *context)
{
    if (match(context, IF))
    {
        ASTNode *ifNode = createASTNode(context->current);
        consume(context);
        expect(context, OPEN_PAREN);
        consume(context);
        ifNode->left = exp(context);
        expect(context, CLOSE_PAREN);
        consume(context);
        if (match(context, OPEN_CURLY_PAREN))
        {
            ifNode->right = parse_block(context);
            return ifNode;
        }
        ifNode->right = stmt(context);
        return ifNode;
    }
    return NULL;
}

ASTNode *iter_stmt(Context *context)
{
    if (match(context, WHILE))
    {
        context->loopStack += 1;
        ASTNode *whileNode = createASTNode(context->current);
        consume(context);
        expect(context, OPEN_PAREN);
        consume(context);
        whileNode->left = exp(context);
        expect(context, CLOSE_PAREN);
        consume(context);
        if (match(context, OPEN_CURLY_PAREN))
        {
            whileNode->right = parse_block(context);
            context->loopStack -= 1;
            return whileNode;
        }
        context->loopStack -= 1;
        whileNode->right = stmt(context);
        return whileNode;
    }
    return NULL;
}

ASTNode *jump_stmt(Context *context)
{
    if (match(context, RETURN))
    {
        ASTNode *returnNode = createASTNode(context->current);
        consume(context);
        if (match(context, SEMI_COLAN))
        {
            consume(context); // consume ";"
            return returnNode;
        }
        returnNode->left = exp(context);
        expect(context, SEMI_COLAN);
        consume(context);
        return returnNode;
    }
    return NULL;
}

// BLOCK := "{" EXP  "}"
ASTNode *parse_block(Context *context)
{

    expect(context, OPEN_CURLY_PAREN);
    consume(context);
    SymbolTable *symbolTable = initSymbolTable();
    pushSymbolTable(context->symbolTableStack, symbolTable);
    insertSymbolTableToQueue(context, symbolTable);
    Token body_start = {.lexeme = "Body_start", .value = BODYSTART};
    ASTNode *bodyStartNode = createASTNode(body_start);
    ASTNode *start = bodyStartNode;
    ASTNode *body;
    int first = 1;
    while (context->current.value != CLOSE_CURLY_PAREN && context->current.value != TEOF)
    {

        if (first)
        {
            body = stmt(context);
            bodyStartNode->right = body;
            first = 0;
            continue;
        }
        body->next = stmt(context);
        body = body->next;
    }
    expect(context, CLOSE_CURLY_PAREN);
    Token body_end_token = {.lexeme = "Body_end", .value = BODYEND};
    ASTNode *body_end = createASTNode(body_end_token);
    bodyStartNode->next = body_end;
    consume(context);
    popSymbolTable(context->symbolTableStack);

    return start;
}

// ASM := "asm" "(" "[" STRING_ASM "]" ")" ";"
// this function will push assembly instuction directly in asm file to make sys calls
ASTNode *parseAsm(Context *context)
{
    if (match(context, ASM))
    {
        ASTNode *asmNode = createASTNode(context->current);
        consume(context); // consume asm token

        expect(context, OPEN_PAREN);
        consume(context); // consume "("
        expect(context, OPEN_SQ_PARAN);
        consume(context); // consume "["
        asmNode->right = parseASTString(context);
        expect(context, CLOSE_SQ_PARAN);
        consume(context); // consume "]"
        expect(context, CLOSE_PAREN);
        consume(context); // consume ")"
        expect(context, SEMI_COLAN);
        consume(context); // consume ";"
        return asmNode;
    }
    return NULL;
}
ASTNode *parseASTString(Context *context)
{
    if (match(context, CLOSE_SQ_PARAN))
    {
        return NULL;
    }
    expect(context, STRING_CONSTANT);
    ASTNode *node = createASTNode(context->current);
    consume(context); // consume "string"
    if (match(context, CLOSE_SQ_PARAN))
    {
        return node;
    }
    expect(context, COMMA);
    consume(context); // consume ","
    node->next = parseASTString(context);
    return node;
}
