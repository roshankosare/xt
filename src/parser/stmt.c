#include "../../include/parser/stmt.h"
#include "../../include/parser/helper.h"
#include "../../include/parser/ast.h"

// STMT := { EXP_STMT | DEC_STMT | SELC_STMT | ITER_STMT   ";" }
// DEC_STMT := { "var" "IDENTFIER" ";"} | { "var" "IDENTFIER" "=" EXP_STMT }
// EXP_STMT := { "(" EXP_STMT ")" }
// EXP_STMT := { UNI_STMT | ASSIGN_STMT | FUNTION_CALL_STMT | ADDITIVE_EXP | CONDITIONAL_EXP }
// EXP_STMT := { EXP_STMT  "EXP_TOKEN" EXP_STMT }
// EXP_TOKEN := { "+" | "-" | "*" | "/" | ">" | "<" | "==" | "and" | "or" }
// UNI_STMT := {"++" | "--" "IDENTIFIER"}  | {"++" | "--" "IDENTIFIER" } | {"not" EXP_STMT }
// FUNCTION_CALL_STMT := { "IDENTIFER" "(" FUNTION_CALL _PARMA_LIST ")" }
// FUNTION_CALL _PARMA_LIST := { { "IDENTIFIER" "," FUNTION_CALL _PARMA_LIST | "IDENTIFER" ε }}
// ASSIGN_STMT := { "IDENTIFIER" "=" EXP_STMT }
// CONDITIONAL_EXP := PRIMARY_EXP "<" | ">" | "==" | "<=" | ">=" PRIMARY_EXP
// ADDITIVE_EXP := { MULTI_EXP "+" | "-" MULTI_EXP  } | {MULTI_EXP}
// MULTI_EXP := { PRIMARY_EXP "*" | "/" MULTI_EXP } | { PRIMARY_EXP }
// PRIMARY_EXP := { "IDENTIFER" | "CONSTANT" }

ASTNode *dec_stmt(Context *context);
ASTNode *parse_assign(Context *context);
ASTNode *parse_uni_stmt(Context *context);
ASTNode *parse_additive(Context *context);
ASTNode *parse_multi(Context *context);
ASTNode *parse_primary(Context *context);
ASTNode *parse_fun_call(Context *context);
ASTNode *parse_fun_call_args(Context *context);
ASTNode *parse_conditional_exp(Context *context);

// STMT := { EXP_STMT | DEC_STMT  ";" }
ASTNode *exp(Context *context)
{
    int x;
    ASTNode *node;
    /// EXP :=  "var" EXP_STMT";"
    if (match(context, VAR))
    {
        node = dec_stmt(context);
        expect(context, SEMI_COLAN);
        consume(context);
        return node;
    }

    node = exp_stmt(context);
    expect(context, SEMI_COLAN);
    consume(context);
    return node;
}

// DEC_STMT := { "var" "IDENTFIER" ";"} | { "var" "IDENTFIER" "=" EXP_STMT }
ASTNode *dec_stmt(Context *context)
{

    // DEC_STMT := { "var" "IDENTFIER" ";"}
    ASTNode *varnode = createASTNode(context->current);
    consume(context); // consume "var"
    expect(context, IDENTIFIER);
    ASTNode *idNode = createASTNode(context->current); // consume "id"
    consume(context);
    varnode->right = idNode;
    if (match(context, SEMI_COLAN))
    {
        return varnode;
    }
    // DEC_STMT := { "var" "IDENTFIER" "=" EXP_STMT }
    expect(context, ASSIGN);
    ASTNode *assignNode = createASTNode(context->current);
    consume(context); // consume "="
    assignNode->left = idNode;
    assignNode->right = exp_stmt(context);
    varnode->right = assignNode;
    return varnode;
}

ASTNode *exp_stmt(Context *context)
{
    ASTNode *node;
    // EXP_STMT := { "(" EXP_STMT ")" }
    if (match(context, OPEN_PAREN))
    {
        consume(context);
        node = exp_stmt(context);
        expect(context, CLOSE_PAREN);
        consume(context);
        // EXP_STMT := { EXP_STMT  "EXP_TOKEN" EXP_STMT }
        // EXP_TOKEN := { "+" | "-" | "*" | "/" | ">" | "<" | "==" | "and" | "or" }
        if (match(context, EXP_TOKEN))
        {
            ASTNode *op = createASTNode(context->current);
            consume(context); // consume  exp_token
            op->left = node;
            op->right = exp_stmt(context);
            node = op;
            return node;
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
            op->right = exp_stmt(context);
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
            op->right = exp_stmt(context);
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
            op->right = exp_stmt(context);
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
            op->right = exp_stmt(context);
            node = op;
            return node;
        }
        return node;
    }
}

// UNI_STMT := {"++" | "--" "IDENTIFIER"}  | {"IDENTIFIER" "++" | "--"  } | {"not" EXP_STMT }
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
    // UNI_STMT := {"not" EXP_STMT }
    if (match(context, LOGICAL_NOT))
    {
        ASTNode *node = createASTNode(context->current);
        consume(context);
        node->right = exp_stmt(context);
        return node;
    }

    return NULL;
}

// FUNCTION_CALL_STMT := { "IDENTIFER" "(" FUNTION_CALL _PARMA_LIST ")" }
ASTNode *parse_fun_call(Context *context)
{
    expect(context, OPEN_PAREN);
    consume(context); // consume ""
    Token token = {.lexeme = "Args", .value = UNKNOWN};
    ASTNode *args = createASTNode(token);
    args->right = parse_fun_call_args(context);
    consume(context); // consume ")"
    return args;
}

// FUNTION_CALL _PARMA_LIST := { { "IDENTIFIER" | "CONSTANT" "," FUNTION_CALL _PARMA_LIST | "IDENTIFER"|"CONSTANT" ε }}

ASTNode *parse_fun_call_args(Context *context)
{

    if (match(context, CLOSE_PAREN))
    {
        return NULL;
    }

    if (match(context, IDENTIFIER) || match(context, CONSTANT))
    {

        ASTNode *node = createASTNode(context->current);
        consume(context);
        if (match(context, CLOSE_PAREN))
        {
            return node;
        }
        else
        {
            expect(context, COMMA);
            consume(context);
            node->next = parse_fun_call_args(context);
            return node;
        }
    }
    expect(context, IDENTIFIER);
    return NULL;
}

// ASSIGN_STMT := { "IDENTIFIER" "=" EXP_STMT }
ASTNode *parse_assign(Context *context)
{
    if (match(context, IDENTIFIER))
    {
        ASTNode *identifierNode = createASTNode(context->current);
        consume(context);
        if (match(context, ASSIGN))
        {
            ASTNode *assignNode = createASTNode(context->current);
            consume(context);
            assignNode->right = exp_stmt(context);
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
    ASTNode *node;
    ASTNode *primaryLeft = parse_multi(context);
    node = primaryLeft;
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
        ASTNode *right = parse_multi(context);
        ASTNode *opNode = createASTNode(opToken);
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
        ASTNode *node;
        node = createASTNode(context->current);
        consume(context);
        return node;
    }
    expect(context, IDENTIFIER);
    return NULL;
}

// SELC_STMT :=  IF_STMT
//  IF_STMT := "if" "(" EXP_STMT | CONDITIONAL_STMT ")" BLOCK_STMT | EXP
ASTNode *selc_stmt(Context *context)
{
    if (match(context, IF))
    {
        ASTNode *ifNode = createASTNode(context->current);
        consume(context);
        expect(context, OPEN_PAREN);
        consume(context);
        ifNode->left = exp_stmt(context);
        expect(context, CLOSE_PAREN);
        consume(context);
        if (match(context, OPEN_CURLY_PAREN))
        {
            ifNode->right = parse_block(context);
            return ifNode;
        }
        ifNode->right = exp(context);
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
        whileNode->left = exp_stmt(context);
        expect(context, CLOSE_PAREN);
        consume(context);
        if (match(context, OPEN_CURLY_PAREN))
        {
            whileNode->right = parse_block(context);
            return whileNode;
        }
        whileNode->right = exp(context);
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
        returnNode->right = exp_stmt(context);
        return returnNode;
    }
    return NULL;
}

// BLOCK := "{" EXP  "}"
ASTNode *parse_block(Context *context)
{

    expect(context, OPEN_CURLY_PAREN);
    consume(context);
    Token body_start = {.lexeme = "Body_start", .value = BODYSTART};
    ASTNode *body = createASTNode(body_start);
    ASTNode *start = body;
    while (context->current.value != CLOSE_CURLY_PAREN && context->current.value != TEOF)
    {

        if (match(context, IF))
        {
            body->next = selc_stmt(context);
            body = body->next;
        }
        if (match(context, WHILE))
        {
            body->next = iter_stmt(context);
            body = body->next;
        }
        if (match(context, RETURN))
        {
            body->next = jump_stmt(context);
            body = body->next;
        }
        if (match(context, VAR))
        {
            body->next = exp(context);
            body = body->next;
        }
        if (match(context, IDENTIFIER))
        {
            body->next = exp(context);
            body = body->next;
        }
    }
    expect(context, CLOSE_CURLY_PAREN);
    Token body_end_token = {.lexeme = "Body_end", .value = BODYEND};
    ASTNode *body_end = createASTNode(body_end_token);
    body->next = body_end;
    consume(context);
    return start;
}