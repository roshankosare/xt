#include "../../include/parser/exp.h"
#include "../../include/parser/helper.h"
#include "../../include/parser/ast.h"

// EXP_STMT := { exp ";" }
// EXP := { "(" exp ")" }
// EXP := { UNI_STMT | ASSIGN_STMT | FUNTION_CALL_STMT | ADDITIVE_EXP | CONDITIONAL_EXP }
// EXP := { EXP "EXP_TOKEN" EXP }
// EXP_TOKEN := { "+" | "-" | "*" | "/" | ">" | "<" | "==" | "and" | "or" }
// UNI_STMT := {"++" | "--" "IDENTIFIER"}  | {"++" | "--" "IDENTIFIER" } | {"not" exp }
// FUNCTION_CALL_STMT := { "IDENTIFER" "(" FUNTION_CALL _PARMA_LIST ")" }
// FUNTION_CALL _PARMA_LIST := { { EXP "," FUNTION_CALL _PARMA_LIST | EXP ε }}
// ASSIGN_STMT := { "IDENTIFIER" "=" exp }
// CONDITIONAL_EXP := PRIMARY_EXP "<" | ">" | "==" | "<=" | ">=" PRIMARY_EXP
// ADDITIVE_EXP := { MULTI_EXP "+" | "-" MULTI_EXP  } | {MULTI_EXP}
// MULTI_EXP := { PRIMARY_EXP "*" | "/" MULTI_EXP } | { PRIMARY_EXP }
// PRIMARY_EXP := { "IDENTIFER" | "CONSTANT" }

ASTNode *parse_assign(Context *context);
ASTNode *parse_uni_stmt(Context *context);
ASTNode *parse_additive(Context *context);
ASTNode *parse_multi(Context *context);
ASTNode *parse_primary(Context *context);
ASTNode *parse_fun_call(Context *context);
ASTNode *parse_fun_call_args(Context *context);
ASTNode *parse_conditional_exp(Context *context);

// STMT := { exp | DEC_STMT  ";" }
ASTNode *exp_stmt(Context *context)
{

    ASTNode *node;
    node = exp(context);
    expect(context, SEMI_COLAN);
    consume(context);
    return node;
}

ASTNode *exp(Context *context)
{
    ASTNode *node;
    // exp := { "(" exp ")" }
    if (match(context, OPEN_PAREN))
    {
        consume(context);
        node = exp(context);
        expect(context, CLOSE_PAREN);
        consume(context);
        // exp := { exp  "EXP_TOKEN" exp }
        // EXP_TOKEN := { "+" | "-" | "*" | "/" | ">" | "<" | "==" | "and" | "or" }
        if (match(context, EXP_TOKEN))
        {
            ASTNode *op = createASTNode(context->current);
            consume(context); // consume  exp_token
            op->left = node;
            op->right = exp(context);
            node = op;
        }
        return node;
    }

    node = parse_assign(context);
    if (node != NULL)
    {
        if (match(context, EXP_TOKEN))
        {
            ASTNode *op = createASTNode(context->current);
            consume(context); // consume  exp_token
            op->left = node;
            op->right = exp(context);
            node = op;
            return node;
        }
        return node;
    }

    node = parse_fun_call(context);
    if (node != NULL)
    {
        if (match(context, EXP_TOKEN))
        {
            ASTNode *op = createASTNode(context->current);
            consume(context); // consume  exp_token
            op->left = node;
            op->right = exp(context);
            node = op;
            return node;
        }
        return node;
    }

    node = parse_uni_stmt(context);
    if (node != NULL)
    {
        if (match(context, EXP_TOKEN))
        {
            ASTNode *op = createASTNode(context->current);
            consume(context); // consume  exp_token
            op->left = node;
            op->right = exp(context);
            node = op;
            return node;
        }
        return node;
    }

    node = parse_conditional_exp(context);
    if (node != NULL)
    {
        if (match(context, EXP_TOKEN))
        {
            ASTNode *op = createASTNode(context->current);
            consume(context); // consume  exp_token
            op->left = node;
            op->right = exp(context);
            node = op;
            return node;
        }
        return node;
    }

    node = parse_additive(context);
    if (node != NULL)
    {
        if (match(context, EXP_TOKEN))
        {
            ASTNode *op = createASTNode(context->current);
            consume(context); // consume  exp_token
            op->left = node;
            op->right = exp(context);
            node = op;
            return node;
        }
        return node;
    }
    return NULL;
}

// UNI_STMT := {"++" | "--" "IDENTIFIER"}  | {"IDENTIFIER" "++" | "--"  } | {"not" exp }
ASTNode *parse_uni_stmt(Context *context)
{

    // UNI_STMT := {"++" | "--" "IDENTIFIER"}
    if (match(context, INC) || match(context, DEC))
    {
        ASTNode *op = createASTNode(context->current);
        consume(context); // consume op
        expect(context, IDENTIFIER);
        // TODO:= here identifer should not funtion name
        op->right = createASTNode(context->current);
        return op;
    }
    int x;
    // UNI_STMT :=  {"IDENTIFIER" "++" | "--"  }
    if (match(context, IDENTIFIER))
    {
        ASTNode *node = createASTNode(context->current);
        consume(context);
        if (match(context, INC) || match(context, DEC))
        {
            node->right = createASTNode(context->current);
            return node;
        }
        free(node);
        unconsume(context);
        return NULL;
    }
    // UNI_STMT := {"not" exp }
    if (match(context, LOGICAL_NOT))
    {
        ASTNode *node = createASTNode(context->current);
        consume(context);
        node->right = exp(context);
        return node;
    }

    return NULL;
}

// FUNCTION_CALL_STMT := { "IDENTIFER" "(" FUNTION_CALL _PARMA_LIST ")" }
ASTNode *parse_fun_call(Context *context)
{

    if (match(context, IDENTIFIER))
    {
        Token funtion_name = context->current;
        ASTNode *funId = createASTNode(funtion_name);
        consume(context);
        if (match(context, OPEN_PAREN))
        {
            checkFuntionEntry(context,funtion_name);
            consume(context); // consume "("
            Token token = {.lexeme = "Args", .value = UNKNOWN};
            ASTNode *args = createASTNode(token);
            funId->left = args;
            args->right = parse_fun_call_args(context);
            expect(context, CLOSE_PAREN);
            consume(context); // consume ")"
            return funId;
        }
        free(funId);
        unconsume(context);
    }

    return NULL;
}

// FUNTION_CALL _PARMA_LIST := { { EXP "," FUNTION_CALL _PARMA_LIST | EXP ε }}

ASTNode *parse_fun_call_args(Context *context)
{
    if (match(context, CLOSE_PAREN))
    {
        return NULL;
    }
    ASTNode *node = exp(context);
    if (match(context, COMMA))
    {
        consume(context);
        node->next = parse_fun_call_args(context);
        return node;
    }
    return node;
}

// ASSIGN_STMT := { "IDENTIFIER" "=" exp }
ASTNode *parse_assign(Context *context)
{
    if (match(context, IDENTIFIER))
    {
        ASTNode *identifierNode = createASTNode(context->current);
        checkSymbolEntry(context,identifierNode->token); // check for symbol entry
        consume(context);
        if (match(context, ASSIGN))
        {
            ASTNode *assignNode = createASTNode(context->current);
            consume(context);
            assignNode->right = exp(context);
            assignNode->left = identifierNode;
            return assignNode;
        }
        unconsume(context);
        free(identifierNode);
        return NULL;
    }
    return NULL;
}

// CONDITIONAL_EXP := PRIMARY_EXP "<" | ">" | "==" | "<=" | ">=" PRIMARY_EXP
ASTNode *parse_conditional_exp(Context *context)
{
    ASTNode *primaryLeft = parse_primary(context);
    if (match(context, CONDITIONAL_TOKEN))
    {
        ASTNode *op = createASTNode(context->current);
        consume(context);
        op->left = primaryLeft;
        op->right = parse_primary(context);
        return op;
    }
    unconsume(context);
    free(primaryLeft);
    return NULL;
}

// ADDITIVE_EXP := { MULTI_EXP "+" | "-" MULTI_EXP  } | {MULTI_EXP}

ASTNode *parse_additive(Context *context)
{

    ASTNode *primaryLeft = parse_multi(context);
    ASTNode *node = primaryLeft;
    while (match(context, PLUS) || match(context, MINUS))
    {
        ASTNode *op = createASTNode(context->current);
        consume(context);
        op->left = primaryLeft;
        op->right = parse_multi(context);
        primaryLeft = op;
        node = op;
    }

    return node;
}

// MULTI_EXP := { PRIMARY_EXP "*" | "/" MULTI_EXP } | { PRIMARY_EXP }

ASTNode *parse_multi(Context *context)
{
    ASTNode *primaryLeft = parse_primary(context);
    ASTNode *node = primaryLeft;
    while (match(context, MUL) || match(context, DIV))
    {
        Token opToken = context->current;
        consume(context);
        ASTNode *opNode = createASTNode(opToken);
        ASTNode *right = parse_multi(context);
        opNode->left = primaryLeft;
        opNode->right = right;
        primaryLeft = opNode;
        node = opNode;
    }
    return node;
}

// PRIMARY_EXP := { "IDENTIFER" | "CONSTANT" }
ASTNode *parse_primary(Context *context)
{
    if (match(context, IDENTIFIER) || match(context, CONSTANT))
    {
        Token symbol = context->current;
        if (match(context, IDENTIFIER))
        {
            checkSymbolEntry(context, symbol);
        }
        ASTNode *node;
        node = createASTNode(context->current);
        consume(context);
        return node;
    }
    expect(context, IDENTIFIER);
    return NULL;
}
