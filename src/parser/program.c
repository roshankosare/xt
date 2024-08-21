#include "../../include/parser/program.h"
#include "../../include/parser/helper.h"
#include "../../include/parser/stmt.h"
#include "../../include/parser/fun.h"

// PROGRAM = IDENT_STMT | STMT
// IDENT_STMT = exp | FUN_SMT
//
ASTNode *program(Context *context)
{

    if (context->current.value == FUNCTION)
    {
        
        ASTNode *funtionNode;
        funtionNode = fun(context);
        return funtionNode;
    }

    Token token = {.lexeme = "Statement", .value = UNKNOWN};
    ASTNode *stmtNode = createASTNode(token);
    stmtNode->right = stmt(context);
    return stmtNode;
}

// STMT = SELC_STMT | JUMP_STMT | ITER_STMT | DEC_STMT


ASTNode *fun(Context *context)
{
    ASTNode *node;
    node = parse_fun(context);
    return node;
}