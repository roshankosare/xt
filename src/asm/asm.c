#include "../../include/asm/asm.h"
#include "../../include/parser/helper.h"
#include "../../include/symboltable/functiontable.h"
#include "../../include/symboltable/symboltable.h"
#include "../../include/asm/asmcontext.h"
#include "../../include/asm/stacks.h"
#include "../../include/asm/system.h"
#include "../../include/asm/helper.h"
#include "../../include/asm/asmexp.h"
#include "../../include/asm/asmstmt.h"
#include <assert.h>
#include <string.h>

void translate(ASTNode *ast, Context *context, FILE *fp)
{
    if (ast == NULL)
    {
        return;
    }
    switch (ast->token.value)
    {
    case UNKNOWN:
        translate(ast->right, context, fp);
        break;
    case PLUS:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        print_add(fp);
        break;

    case MUL:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        print_mul(fp);
        break;

    case DIV:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        print_div(fp);
        break;

    case MINUS:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        print_sub(fp);
        break;

    case MODULAS:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        print_modulas(fp);
        break;

    case ASSIGN:
        translate(ast->right, context, fp); // convert expression to asm which will store value of evaluated expresion to stack
        print_assign(ast, context, fp);
        break;

    case INC:
        print_inc(ast, context, fp);
        break;

    case DEC:
        print_dec(ast, context, fp);
        break;

    case VAR:
        print_var(ast, context, fp);
        break;

    case TYPEOF:
        translate(ast->left, context, fp);
        print_typeof(fp);
        break;

    case IF:
        // translate(ast->left, context, fp);
        print_if(ast, context, fp);
        // prase  condition first
        break;

    case ELSE:
        assert(0 && "TODO: ELSE is not implemented");
    case FOR:
        assert(0 && "TODO: FOR is not implemented");

    case RETURN:
        print_return(ast, context, fp);
        break;

    case WHILE:
        print_while(ast, context, fp);
        // assert(0 && "TODO: WHILE is not implemented");
        break;

    case CONTINUE:
        print_continue(ast, context, fp);
        break;

    case BREAK:
        print_break(ast, context, fp);
        break;

    case LESS_THAN:
        translate(ast->left, context, fp); // parse left expression
        translate(ast->right, context, fp);
        print_less_than(fp);
        break;

    case GRATER_THAN:
        translate(ast->left, context, fp);  // parse left expression
        translate(ast->right, context, fp); // parse right expression
        print_grater_than(fp);
        break;

    case LESS_THAN_EQTO:
        translate(ast->left, context, fp);  // parse left expression
        translate(ast->right, context, fp); // parse right expression
        print_lessthan_eqto(fp);
        break;

    case GRATER_THAN_EQTO:
        translate(ast->left, context, fp);  // parse left expression
        translate(ast->right, context, fp); // parse right expression
        print_graterthan_eqto(fp);
        break;

    case EQUALTO:
        translate(ast->left, context, fp);  // parse left expression
        translate(ast->right, context, fp); // parse right expression
        print_eqto(fp);
        break;

    case NOT_EQLTO:
        translate(ast->left, context, fp);  // parse left expression
        translate(ast->right, context, fp); // parse right expression
        print_noteqto(fp);
        break;

    case IDENTIFIER:
        print_identifier(ast, context, fp);
        break;

    case INTEGER_CONSTANT:
        print_integer_constant(ast, context, fp);
        break;

    case HEX_CONSTANT:
        print_hex_constant(ast, context, fp);
        break;

    case FLOAT_CONSTANT:
        print_float_constant(ast, context, fp);
        break;

    case VALUE_AT:
        translate(ast->left, context, fp);
        print_valueat(fp);
        break;

    case STRING_CONSTANT:
        print_string_constant(ast, context, fp);
        break;

    case FUNCTION:
        print_function(ast, context, fp);
        break;

    case FUNCTION_CALL:
        print_funtion_call(ast, context, fp);
        break;

    case ARG:
        translate(ast->left, context, fp);
        return;

    case PARAM_START:

        break;

    case PARAM_END:
        // SymbolTableEntry *entry = popSymbolTable(context->symbolTableStack);
        // pushSymbolTable(context->symbolTableTempStack, entry);
        break;

    case ARGS_START:
        break;

    case ARGS_END:
        return;

    case LOGICAL_AND:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        print_logical_and(fp);
        break;

    case LOGICAL_OR:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        print_logigcal_or(fp);
        break;
    case LOGICAL_NOT:
        translate(ast->left, context, fp);
        print_logical_not(fp);
        break;

    case BIT_AND:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        print_bitwise_and(fp);
        break;

    case BIT_OR:
        translate(ast->left, context, fp);
        translate(ast->right, context, fp);
        print_bitwise_or(fp);
        break;

    case BIT_NOT:
        print_bitwise_not(fp);
        break;

    case BODYSTART:
        print_body_start(ast, context, fp);
        break;

    case BODYEND:
        print_body_end(ast, context, fp);
        break;

    case ASM:
        print_asm(ast, context, fp);
        break;

    default:
        printf("\nERROR: %s token handle not implemented", ast->token.lexeme);
        exit(1);
    }
    return translate(ast->next, context, fp);
}
