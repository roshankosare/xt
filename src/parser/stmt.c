#include "../../include/parser/stmt.h"
#include "../../include/parser/helper.h"
#include "../../include/parser/ast.h"

// EXP := EXP_STMT | "var" EXP_STMT";"
// EXP_STMT := "IDENTIFIER" UNI_EXP | ASSIGN_EXP | FUNTION_CALL |
// EXP_STMT := "(" EXP_STMT ")"
// UNI_EXP := "++" | "--"
// ASSIGN_EXP := "="  ADDITIVE_EXP | CONDITIONAL_EXP
// ADDITIVE_EXP := MULTI_EXP  + | - MULTI_EXP
// ADDITIVE_EXP := "(" ADDITIVE_EXP ")" +|-|*|/ ADDITIVE_EXP
// MULTI_EXP := PRIMARY_EXP  | PRIMARY_EXP * | / MULTI_EXP
// MULTI_EXP := "(" ADDITIVE_EXP ")"
// PRIMARY_EXP := "CONSTANT" | "IDENTIFIER"
// FUNTION_CALL :=  "(" PARAM_LIST ")"
ASTNode *parse_assign(Context *context);
ASTNode *parse_additive(Context *context);
ASTNode *parse_multi(Context *context);
ASTNode *parse_primary(Context *context);
ASTNode *parse_fun_call(Context *context);
ASTNode *parse_fun_call_args(Context *context);
ASTNode *parse_conditional_exp(Context *context);

ASTNode *exp(Context *context)
{
    ASTNode *node;

    /// EXP := EXP_STMT | "var" EXP_STMT";"

    node = exp_stmt(context);
    expect(context, SEMI_COLAN);
    consume(context);
    return node;
}

ASTNode *dec_stmt(Context *context)
{
    ASTNode *varnode = createASTNode(context->current);
    consume(context);
    varnode->right = exp_stmt(context);

    return varnode;
}

ASTNode *exp_stmt(Context *context)
{
    ASTNode *node;
    if (match(context, VAR))
    {
        node = dec_stmt(context);
        return node;
    }

    if (match(context, IDENTIFIER))
    {
        // EXP_STMT := "IDENTIFIER_TOKEN" UNI_EXP  case
        // create left side of ASTNode as Identifier
        ASTNode *node = createASTNode(context->current);
        consume(context);

        // UNI_EXP := "++" | "--"
        // following lines of code will implement above rule
        if (match(context, INC) || match(context, DEC))
        {
            ASTNode *op = createASTNode(context->current);
            consume(context);
            node->right = op;
            return node;
        }
        // EXP_STMT := "IDENTIFIER_TOKEN" ASSIGN_EXP case
        if (match(context, ASSIGN))
        {
            ASTNode *assign = parse_assign(context);
            assign->left = node;
            node = assign;
            return node;
        }
        if (match(context, OPEN_PAREN))
        {
            node->right = parse_fun_call(context);
            return node;
        }
    }
    consume(context);
    return NULL;
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

    // ASSIGN_EXP := "="  ADDITIVE_EXP | CONDITIONAL_EXP
    if (isConditionalOperator(context->lookahed.value))
    {
        node = parse_conditional_exp(context);
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

    if (match(context, IDENTIFIER) || match(context, CONSTANT))
    {
        ASTNode *node;

        // PRIMARY_EXP := "CONSTANT" | "IDENTIFIER"

        node = createASTNode(context->current);
        consume(context);
        return node;
    }
    expect(context, IDENTIFIER);
    return NULL;
}

// FUNTION_CALL :=  "(" PARAM_LIST ")"
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

// PARAM_LIST :=  "IDENTIFIER"|"CONSTANT" ","| ε  PARAMLIST
// PARAM_LIST := ε

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

// CONDITIONAL_STMT := "(" CONDITIONAL_EXP ")" "and" | "or" | "not" CONDITIONAL_EXP
// CONDITIONAL_STMT :=  PRIMARY_EXP  | PPRIMARY_EXP OPERATOR_TOKEN "IDEN" PRIMARY_EXP
ASTNode *parse_conditional_exp(Context *context)
{
    ASTNode *node;
    // CONDITIONAL_STMT := "(" CONDITIONAL_EXP ")"
    if (match(context, OPEN_PAREN))
    {
        // consume "("
        consume(context);
        node = parse_conditional_exp(context);
        expect(context, CLOSE_PAREN);
        consume(context);
        if (match(context, LOGICAL_AND) || match(context, LOGICAL_OR) || match(context, LOGICAL_NOT))
        {
            ASTNode *right = createASTNode(context->current);
            consume(context);
            right->left = node;
            right->right = parse_conditional_exp(context);
            return right;
        }
        return node;
    }
    ASTNode *first = parse_primary(context);
    expect(context, CONDITIONAL_TOKEN);
    ASTNode *op = createASTNode(context->current);
    consume(context);
    ASTNode *second = parse_primary(context);
    if (match(context, LOGICAL_AND) || match(context, LOGICAL_OR) || match(context, LOGICAL_NOT))
    {
        ASTNode *right = createASTNode(context->current);
        consume(context);
        right->left = op;
        right->right = parse_conditional_exp(context);
        return right;
    }
    node = op;
    return op;
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
        ifNode->left = parse_additive(context);
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
        whileNode->left = parse_additive(context);
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
        returnNode->right = exp(context);
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