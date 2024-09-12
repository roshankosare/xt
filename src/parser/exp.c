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
ASTNode *parse_valueAt(Context *context);
ASTNode *parse_typeof(Context *context);
// ASTNode *parse_conditional_exp(Context *context);

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
        // EXP_TOKEN := { "+" | "-" | "*" | "/" | ">" | "<" | "==" | "and" | "or" | "!=" }
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

    // node = parse_uni_stmt(context);
    // if (node != NULL)
    // {
    //     if (match(context, EXP_TOKEN))
    //     {
    //         ASTNode *op = createASTNode(context->current);
    //         consume(context); // consume  exp_token
    //         op->left = node;
    //         op->right = exp(context);
    //         node = op;
    //         return node;
    //     }
    //     return node;
    // }

    // node = parse_conditional_exp(context);
    // if (node != NULL)
    // {
    //     if (match(context, EXP_TOKEN))
    //     {
    //         ASTNode *op = createASTNode(context->current);
    //         consume(context); // consume  exp_token
    //         op->left = node;
    //         op->right = exp(context);
    //         node = op;
    //         return node;
    //     }
    //     return node;
    // }

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
        printf("\nthis runs");

        ASTNode *op = createASTNode(context->current);
        consume(context); // consume op
        expect(context, IDENTIFIER);
        // TODO:= here identifer should not funtion name
        // checkSymbolEntry(context, context->current);

        op->left = createASTNode(context->current);
        consume(context);
        return op;
    }
    // UNI_STMT :=  {"IDENTIFIER" "++" | "--"  }
    if (match(context, IDENTIFIER))
    {
        Token identifier = context->current;
        ASTNode *node = createASTNode(context->current);
        consume(context);
        if (match(context, INC) || match(context, DEC))
        {
            checkSymbolEntry(context, identifier);
            ASTNode *op = createASTNode(context->current);

            consume(context);
            op->right = node;
            return op;
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
        node->left = exp(context);
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
            printf("\nthis runs");
            Token funtion_call_token = {.lexeme = "Funtion_call", .value = FUNCTION_CALL};
            ASTNode *funtion_call_node = createASTNode(funtion_call_token);
            checkFuntionEntry(context, funtion_name);
            funtion_call_node->left = funId;
            consume(context); // consume "("
            Token args_start_token = {.lexeme = "Args_start", .value = ARGS_START};
            ASTNode *args_start = createASTNode(args_start_token);
            funtion_call_node->right = args_start;
            args_start->right = parse_fun_call_args(context);

            Token args_end_token = {.lexeme = "Args_end", .value = ARGS_END};
            ASTNode *args_end = createASTNode(args_end_token);
            expect(context, CLOSE_PAREN);
            consume(context); // consume ")"
            args_start->next = args_end;

            return funtion_call_node;
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
    Token arg = {.lexeme = "ARG", .value = ARG};
    ASTNode *node = createASTNode(arg);
    node->left = exp(context);
    if (match(context, COMMA))
    {

        consume(context);

        node->next = createASTNode(arg);
        node->next->left = exp(context);
        node->next->left->next = parse_fun_call_args(context);

        return node;
    }
    expect(context, CLOSE_PAREN);
    return node;
}

// ASSIGN_STMT := { "IDENTIFIER" "=" exp }
ASTNode *parse_assign(Context *context)
{
    if (match(context, IDENTIFIER) || match(context, VALUE_AT))
    {
        ASTNode *identifierNode;
        if (match(context, VALUE_AT))
        {
            identifierNode = parse_valueAt(context);
            if (!match(context, ASSIGN))
            {
                unconsume(context);
                unconsume(context);
                return NULL;
            }
        }
        else
        {
            identifierNode = createASTNode(context->current);
            consume(context);
            if (!match(context, ASSIGN))
            {
                unconsume(context);
                return NULL;
            }

            checkSymbolEntry(context, identifierNode->token);
            // check for symbol entry
        }

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
// ASTNode *parse_conditional_exp(Context *context)
// {
//     ASTNode *primaryLeft = parse_additive(context);

//     if (match(context, CONDITIONAL_TOKEN))
//     {

//         ASTNode *op = createASTNode(context->current);
//         consume(context);
//         op->left = primaryLeft;
//         op->right = parse_primary(context);
//         return op;
//     }
//     unconsume(context);
//     free(primaryLeft);
//     return NULL;
// }

// ADDITIVE_EXP := { MULTI_EXP "+" | "-" MULTI_EXP  } | {MULTI_EXP}

ASTNode *parse_additive(Context *context)
{

    ASTNode *primaryLeft = parse_multi(context);
    ASTNode *node = primaryLeft;
    if (match(context, INTEGER_CONSTANT) || match(context, FLOAT_CONSTANT))
    {

        if (context->current.lexeme[0] != '+' && context->current.lexeme[0] != '-')
        {
            expect(context, EXP_TOKEN);
        }

        Token plusToken = {
            .lexeme = "+", .value = PLUS, .pos = {.col = context->tokens->pos.col - 1, .line = context->tokens->pos.line}};

        ASTNode *plus = createASTNode(plusToken);
        ASTNode *oprand = parse_primary(context);
        plus->left = primaryLeft;
        plus->right = oprand;
        node = plus;
    }
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

// PRIMARY_EXP := { "IDENTIFER" | "CONSTANT" |INC_EXP | DEC_EXP| FUN_CALL |  }
ASTNode *parse_primary(Context *context)
{
    switch (context->current.value)
    {
    case INC:
        /* code */
        return parse_uni_stmt(context);

    case DEC:
        return parse_uni_stmt(context);
    
    case TYPEOF:
        return parse_typeof(context);

    case INTEGER_CONSTANT:
        break;
    case STRING_CONSTANT:
        break;
    case FLOAT_CONSTANT:
        break;
    case HEX_CONSTANT:
        break;
    case VALUE_AT:
    {
        ASTNode *valueAt = parse_valueAt(context);
        if (valueAt)
        {
            return valueAt;
        }
    }

    case IDENTIFIER:
        ASTNode *funCall = parse_fun_call(context);
        if (funCall)
        {
            return funCall;
        }
        ASTNode *uniExp = parse_uni_stmt(context);
        if (uniExp)
        {
            return uniExp;
        }

        Token symbol = context->current;
        if (match(context, IDENTIFIER))
        {
            checkSymbolEntry(context, symbol);
        }
        break;

    default:
        expect(context, IDENTIFIER);
        break;
    }

    ASTNode *node;
    node = createASTNode(context->current);
    consume(context);
    return node;
}

ASTNode *parse_valueAt(Context *context)
{
    if (match(context, VALUE_AT))
    {
        ASTNode *valueAtNode = createASTNode(context->current);
        consume(context); // consume "@"
        expect(context, IDENTIFIER);
        Token symbol = context->current;

        checkSymbolEntry(context, symbol);
        valueAtNode->left = createASTNode(context->current);
        consume(context); // consume IDENTIFIER
        return valueAtNode;
    }
    return NULL;
}

// TYPEOF := "typeof" "(" "iden"
ASTNode *parse_typeof(Context *context)
{

    if (match(context, TYPEOF))
    {

        ASTNode *node = createASTNode(context->current);
        consume(context); // consume typeof
        expect(context, OPEN_PAREN);
        consume(context);
        node->left = exp(context);
        expect(context, CLOSE_PAREN);
        consume(context);
        return node;
    }
    return NULL;
}
