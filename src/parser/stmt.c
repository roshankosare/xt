#include "../../include/parser/stmt.h"
#include "../../include/parser/helper.h"
#include "../../include/parser/ast.h"
#include "../../include/parser/exp.h"
// SELC_STMT :=  IF_STMT
//  IF_STMT := "if" "(" exp | CONDITIONAL_STMT ")" BLOCK_STMT | EXP

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
    if (match(context, IDENTIFIER) || match(context, OPEN_PAREN) || match(context, CONSTANT))
    {
        node = exp_stmt(context);
        return node;
    }

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
            return whileNode;
        }
        whileNode->right = stmt(context);
        return whileNode;
    }
    return NULL;
}

ASTNode *jump_stmt(Context *context)
{
    if (expect(context, RETURN))
    {
        ASTNode *returnNode = createASTNode(context->current);
        consume(context);
        returnNode->right = exp(context);
        return returnNode;
    }
    return NULL;
}

// BLOCK := "{" EXP  "}"
ASTNode *parse_block(Context *context)
{

    expect(context, OPEN_CURLY_PAREN);
    consume(context);
    pushSymbolTable(context->symbolTableStack);
    Token body_start = {.lexeme = "Body_start", .value = BODYSTART};
    ASTNode *body = createASTNode(body_start);
    ASTNode *start = body;
    while (context->current.value != CLOSE_CURLY_PAREN && context->current.value != TEOF)
    {
        body->next = stmt(context);
        body = body->next;
    }
    expect(context, CLOSE_CURLY_PAREN);
    Token body_end_token = {.lexeme = "Body_end", .value = BODYEND};
    ASTNode *body_end = createASTNode(body_end_token);
    body->next = body_end;
    consume(context);
    popSymbolTable(context->symbolTableStack);
    return start;
}