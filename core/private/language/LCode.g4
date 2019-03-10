grammar LCode;

program: statement* ;
statement: 'number' IDENTIFIER ASSIGNATION const_expression END_OF_STATEMENT                        #ConstantDecl
         | 'alias' function_call ASSIGNATION function_call+ END_OF_STATEMENT                        #AliasDecl
         | 'axiom' function_call+ END_OF_STATEMENT                                                  #AxiomDecl
         | 'rewrite' function_call ASSIGNATION function_call+ END_OF_STATEMENT    #RewriteRuleDecl
         ;

function_call: IDENTIFIER LEFT_PARENTHESIS expression_list RIGHT_PARENTHESIS ;
expression_list: expression (',' expression)* ;

expression: SUBSTRACTION expression                                                 #NegativeExpression
          | function_call                                                           #FunctionCall
          | expression MULTIPLICATION expression                                    #Multiplication
          | expression DIVISION expression                                          #Division
          | expression ADDITION expression                                          #Addition
          | expression SUBSTRACTION expression                                      #Substraction
          | LITTERAL_FLOAT                                                          #Float
          | IDENTIFIER                                                              #Identifier
          | LEFT_PARENTHESIS expression RIGHT_PARENTHESIS                           #EnclosedExpression
          ;

// Const expression can't contains function calls because
// function are non const.
const_expression: SUBSTRACTION const_expression                         #ConstNegativeExpression
                | const_expression MULTIPLICATION const_expression      #ConstMultiplication
                | const_expression DIVISION const_expression            #ConstDivision
                | const_expression ADDITION const_expression            #ConstAddition
                | const_expression SUBSTRACTION const_expression        #ConstSubstraction
                | LITTERAL_FLOAT                                        #ConstFloat
                | IDENTIFIER                                            #ConstIdentifier
                | LEFT_PARENTHESIS const_expression RIGHT_PARENTHESIS   #ConstEnclosedExpression
                ;

ADDITION : '+' ;
SUBSTRACTION : '-' ;
MULTIPLICATION : '*' ;
DIVISION : '/' ;
ASSIGNATION : '=' ;
LEFT_PARENTHESIS : '(' ;
RIGHT_PARENTHESIS : ')' ;
END_OF_STATEMENT : ';' ;

IDENTIFIER  : [a-zA-Z_]+DIGIT09*;

LITTERAL_FLOAT
    : '-'? DIGIT09+ '.' DIGIT09*
    | '-'? DIGIT09* '.' DIGIT09+
    | '-'? DIGIT09+
    ;

fragment DIGIT09 : [0-9] ;
fragment DIGIT19 : [1-9] ;

LINE_COMMENT: '//' .*? '\r'? '\n' -> skip;
COMMENT: '/*' .*? '*/' -> skip;
NEWLINE: ('\r'? '\n') -> skip;
WHITESPACES: (' ' | '\t')+ -> skip;
