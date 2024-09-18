#include "../../include/parser/program.h"
#include "../../include/parser/helper.h"
#include "../../include/parser/stmt.h"
#include "../../include/parser/fun.h"
#include "../../include/asm/asm.h"

// PROGRAM = IDENT_STMT | STMT
// IDENT_STMT = exp | FUN_SMT
//
 void program(Context *context,FILE *fp)
{

    if (context->current.value == FUNCTION)
    {
        
        ASTNode *funtionNode;
        funtionNode = fun(context,fp);
        return funtionNode;
    }

    Token token = {.lexeme = "Statement", .value = UNKNOWN};
    ASTNode *stmtNode = createASTNode(token);
    stmtNode->right = stmt(context,fp);
    return stmtNode;
}

// STMT = SELC_STMT | JUMP_STMT | ITER_STMT | DEC_STMT


ASTNode *fun(Context *context,FILE *fp)
{
    ASTNode *node;
    node = parse_fun(context , fp);
    return node;
}