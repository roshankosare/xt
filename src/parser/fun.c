#include "../../include/parser/fun.h"
#include "../../include/parser/helper.h"
#include "../../include/parser/stmt.h"

ASTNode *parse_fun_def_param(Context *context, Token funtion_name)
{
    if (match(context, CLOSE_PAREN))
    {
        return NULL;
    }

    if (match(context, IDENTIFIER))
    {

        Token identifier_token = context->current;
        insertParamSymbol(context, funtion_name, identifier_token);
        ASTNode *node = createASTNode(identifier_token);
        consume(context);
        if (match(context, CLOSE_PAREN))
        {
            return node;
        }
        else
        {
            expect(context, COMMA);
            consume(context);
            expect(context, IDENTIFIER);
            node->next = parse_fun_def_param(context,funtion_name);
            return node;
        }
    }
    expect(context, IDENTIFIER);
    return NULL;
}

// FUN_DEF := "function" "IDENTIFIER" "(" FUN_DEF_PARAM ")" "{" BLOCK_STMT "}"
//  FUN_DEF_PARAM :=  "IDENTIFIER" "," | FUN_DEF_PARAM
//  FUN_DEF_PARAM  := ε

ASTNode *parse_fun(Context *context)
{
    expect(context, FUNCTION);
    consume(context);
    expect(context, IDENTIFIER);

    Token funtion_token = context->current;
    ASTNode *funtion_node = createASTNode(funtion_token);
    insertFuntionEntry(context, funtion_token);
    consume(context);
    expect(context, OPEN_PAREN);
    consume(context);
    Token args_token = {.lexeme = "Args", .value = UNKNOWN};
    ASTNode *args = createASTNode(args_token);
    args->right = parse_fun_def_param(context, funtion_token);
    funtion_node->left = args;
    expect(context, CLOSE_PAREN);
    consume(context);
    ASTNode *body;
    body = parse_block(context);
    funtion_node->right = body;

    return funtion_node;
}