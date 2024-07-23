#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

static int isDelimiter(char ch)
{
    return isspace(ch);
}

int main() {


   
    printf("%d",isDelimiter('='));

    return 0;
}
