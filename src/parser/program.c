#include "../../include/parser/program.h"
#include "../../include/parser/helper.h"
#include "../../include/parser/stmt.h"
#include "../../include/parser/fun.h"

// PROGRAM = IDENT_STMT | STMT
// IDENT_STMT = EXP_STMT | FUN_SMT
//
ASTNode *program(Context *context)
{

    if (context->current.value == FUNCTION)
    {
        Token token = {.lexeme = "Function", .value = UNKNOWN};
        ASTNode *expstmtNode = createASTNode(token);
        expstmtNode->right = fun(context);
        return expstmtNode;
    }

    Token token = {.lexeme = "Statement", .value = UNKNOWN};
    ASTNode *stmtNode = createASTNode(token);
    stmtNode->right = stmt(context);
    return stmtNode;
}

// STMT = SELC_STMT | JUMP_STMT | ITER_STMT | DEC_STMT
ASTNode *stmt(Context *context)
{
    ASTNode *node;
    if (context->current.value == IF)
    {
        node = selc_stmt(context);
        return node;
    }
    if (context->current.value == WHILE)
    {
        node = iter_stmt(context);
        return node;
    }
    if (context->current.value == RETURN)
    {
        node = jump_stmt(context);
        return node;
    }
    if (context->current.value == VAR)
    {
        node = exp(context);
        return node;
    }
    if (context->current.value == IDENTIFIER)
    {
        node = exp(context);
        return node;
    }
}

ASTNode *fun(Context *context)
{
    ASTNode *node;
    node = parse_fun(context);
    return node;
}