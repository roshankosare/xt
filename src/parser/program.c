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
        node = exp(context);
        return node;
    }
    if (match(context, IDENTIFIER) || match(context, OPEN_PAREN))
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