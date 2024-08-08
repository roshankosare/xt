#include "../../include/parser/stmt.h"
#include "../../include/parser/helper.h"

// EXP := EXP_STMT | "var" EXP_STMT";"
// EXP_STMT := "IDENTIFIER_TOKEN" UNI_EXP | ASSIGN_EXP | FUNTION_CALL
// EXP_STMT := "(" EXP_STMT ")"
// UNI_EXP := "++" | "--"
// ASSIGN_EXP := "="  ADDITIVE_EXP
// ADDITIVE_EXP := MULTI_EXP  + | - MULTI_EXP
// ADDITIVE_EXP := "(" ADDITIVE_EXP ")" +|-|*|/ ADDITIVE_EXP
// MULTI_EXP := PRIMARY_EXP  | PRIMARY_EXP * | / MULTI_EXP
// MULTI_EXP := "(" ADDITIVE_EXP ")"
// PRIMARY_EXP := "CONSTANT" | "IDENTIFIER"
ASTNode *parse_assign(Context *context);
ASTNode *parse_additive(Context *context);
ASTNode *parse_multi(Context *context);
ASTNode *parse_primary(Context *context);

ASTNode *exp(Context *context)
{
    ASTNode *node;
    /// EXP := EXP_STMT | "var" EXP_STMT";"
    if (context->current.value == VAR)
    {
        node = dec_stmt(context);
        node->right = exp_stmt(context);
        expect(context, SEMI_COLAN);
        consume(context);
        return node;
    }
    node = exp_stmt(context);
    expect(context, SEMI_COLAN);
    consume(context);
    return node;
}

ASTNode *dec_stmt(Context *context)
{
    ASTNode *varnode = createASTNode(context->current);
    consume(context);
    return varnode;
}

ASTNode *exp_stmt(Context *context)
{
    ASTNode *node;
    if (match(context, OPEN_PAREN))
    {
        // EXP_STMT := "(" EXP_STMT ")" ";"
        // consume "(" token
        consume(context);
        node = exp_stmt(context);
        expect(context, CLOSE_PAREN);
        consume(context);
        return node;
    }

    if (match(context, IDENTIFIER))
    {
        // EXP_STMT := "IDENTIFIER_TOKEN" UNI_EXP  case
        // create left side of ASTNode as Identifier
        ASTNode *node = createASTNode(context->current);
        consume(context);

        // UNI_EXP := "++" | "--"
        // follwing lines of code will implement above rule
        if (match(context, INC) || match(context, DEC))
        {
            ASTNode *op = createASTNode(context->current);
            consume(context);
            node->right = op;
        }
        // EXP_STMT := "IDENTIFIER_TOKEN" ASSIGN_EXP case
        if (match(context, ASSIGN))
        {
            ASTNode *assign = parse_assign(context);
            assign->left = node;
            node = assign;
        }
    }
}
ASTNode *parse_assign(Context *context)
{
    ASTNode *node = createASTNode(context->current);
    consume(context);
    node->right = parse_additive(context);
    return node;
}

ASTNode *parse_additive(Context *context)
{

    // ADDITIVE_EXP := "(" ADDITIVE_EXP ")" +|-|*|/ ADDITIVE_EXP
    ASTNode *node;
    if (match(context, OPEN_PAREN))
    {
        // consume "(" token
        consume(context);
        node = parse_additive(context);
        expect(context, CLOSE_PAREN);
        consume(context);
        if (match(context, PLUS) || match(context, MINUS) || match(context, DIV) || match(context, MUL))
        {

            ASTNode *op = createASTNode(context->current);
            consume(context);
            ASTNode *right = parse_additive(context);
            op->left = node;
            op->right = right;
            node = op;
        }

        return node;
    }
    // ADDITIVE_EXP := MULTI_EXP  + | - MULTI_EXP
    node = parse_multi(context);
    while (context->current.value == PLUS || context->current.value == MINUS)
    {

        Token opToken = context->current;
        consume(context);
        ASTNode *right = parse_multi(context);
        ASTNode *opNode = createASTNode(opToken);
        opNode->left = node;
        opNode->right = right;
        node = opNode;
    }
    return node;
}
ASTNode *parse_multi(Context *context)
{
    // MULTI_EXP := "(" ADDITIVE_EXP ")"
    ASTNode *node;
    if (match(context, OPEN_PAREN))
    {
        node = parse_additive(context);
        return node;
    }

    // MULTI_EXP := PRIMARY_EXP  | PRIMARY_EXP * | / MULTI_EXP
    ASTNode *left = parse_primary(context);
    if (match(context, MUL) || match(context, DIV))
    {
        Token opToken = context->current;
        consume(context);
        ASTNode *right = parse_multi(context);
        ASTNode *opNode = createASTNode(opToken);
        opNode->left = left;
        opNode->right = right;
        left = opNode;
    }

    return left;
}
ASTNode *parse_primary(Context *context)
{

    ASTNode *node;

    // PRIMARY_EXP := "CONSTANT" | "IDENTIFIER"

    node = createASTNode(context->current);
    consume(context);
    return node;
}
ASTNode *selc_stmt(Context *context)
{
}
ASTNode *iter_stmt(Context *context)
{
}

ASTNode *jump_stmt(Context *context)
{
}