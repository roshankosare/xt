program             ::= { function_definition | statement }
function_definition ::= identifier "(" [ parameter_list ] ")" compound_statement
parameter_list      ::= identifier { "," identifier }
statement           ::= expression_statement | compound_statement | selection_statement | function_call | iteration_statement | jump_statement
function_call       ::= identifier "(" [ parameter_list ] ")" ";"
expression_statement ::= expression ";"
compound_statement  ::= "{" { statement } "}"
selection_statement ::= "if" "(" expression ")" compound_statement [ "else" compound_statement ]
iteration_statement ::= "while" "(" expression ")" compound_statement
jump_statement      ::= "return" [ expression ] ";"
expression          ::= assignment_expression
assignment_expression ::= logical_or_expression | identifier "=" assignment_expression
logical_or_expression ::= logical_and_expression { "||" logical_and_expression }
logical_and_expression ::= equality_expression { "&&" equality_expression }
equality_expression ::= relational_expression { ( "==" | "!=" ) relational_expression }
relational_expression ::= additive_expression { ( "<" | ">" | "<=" | ">=" ) additive_expression }
additive_expression ::= multiplicative_expression { ( "+" | "-" ) multiplicative_expression }
multiplicative_expression ::= primary_expression { ( "*" | "/" ) primary_expression }
primary_expression  ::= identifier | constant | "(" expression ")"
