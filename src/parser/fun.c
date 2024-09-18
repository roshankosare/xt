#include "../../include/parser/fun.h"
#include "../../include/parser/helper.h"
#include "../../include/parser/stmt.h"
#include "../../include/asm/asm.h"

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
            node->next = parse_fun_def_param(context, funtion_name);
            return node;
        }
    }
    expect(context, IDENTIFIER);
    return NULL;
}

// FUN_DEF := "function" "IDENTIFIER" "(" FUN_DEF_PARAM ")" "{" BLOCK_STMT "}"
//  FUN_DEF_PARAM :=  "IDENTIFIER" "," | FUN_DEF_PARAM
//  FUN_DEF_PARAM  := ε

ASTNode *parse_fun(Context *context, FILE *fp)
{
    expect(context, FUNCTION);
    Token functionKeywordToken = {.lexeme = "Function", .value = FUNCTION};
    ASTNode *functionKeywordNode = createASTNode(functionKeywordToken);
    consume(context);
    expect(context, IDENTIFIER);
    Token functionNameToken = context->current;
    consume(context);
    insertFuntionEntry(context, functionNameToken);
    FunctionTableEntry *funtionEntry = checkFuntionEntry(context, functionNameToken);
    ASTNode *functionNamenode = createASTNode(functionNameToken);

    functionKeywordNode->left = functionNamenode;

    expect(context, OPEN_PAREN);
    consume(context);
    Token args_start_token = {.lexeme = "Param_start", .value = PARAM_START};
    ASTNode *args_start = createASTNode(args_start_token);
    pushSymbolTable(context->symbolTableStack, funtionEntry->parameterTable);
    args_start->right = parse_fun_def_param(context, functionNameToken);

    Token args_end_token = {.lexeme = "Param_end", .value = PARAM_END};
    ASTNode *args_end = createASTNode(args_end_token);
    args_start->next = args_end;

    functionKeywordNode->right = args_start;
    expect(context, CLOSE_PAREN);
    consume(context);

    translate(functionKeywordNode, context, fp);

    popSymbolTable(context->symbolTableStack);
    return functionKeywordNode;
}