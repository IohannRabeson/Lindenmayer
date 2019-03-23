grammar LCode;

program: statement+
       | WHITESPACES
       ;

statement: 'number' IDENTIFIER ASSIGNATION expression END_OF_STATEMENT            #ConstantDecl
         | 'alias' function_call ASSIGNATION function_call+ END_OF_STATEMENT      #AliasDecl
         | 'axiom' function_call+ END_OF_STATEMENT                                #AxiomDecl
         | 'rewrite' function_call ASSIGNATION function_call+ END_OF_STATEMENT    #RewriteRuleDecl
         ;

function_call: IDENTIFIER LEFT_PARENTHESIS expression_list RIGHT_PARENTHESIS ;
expression_list: expression (',' expression)* ;

expression: SUBSTRACTION expression                        #NegativeExpression
          | function_call                                  #FunctionCall
          | expression MULTIPLICATION expression           #Multiplication
          | expression DIVISION expression                 #Division
          | expression ADDITION expression                 #Addition
          | expression SUBSTRACTION expression             #Substraction
          | LITTERAL_FLOAT                                 #Float
          | IDENTIFIER                                     #Identifier
          | LEFT_PARENTHESIS expression RIGHT_PARENTHESIS  #EnclosedExpression
          ;

ADDITION : '+' ;
SUBSTRACTION : '-' ;
MULTIPLICATION : '*' ;
DIVISION : '/' ;
ASSIGNATION : '=' ;
LEFT_PARENTHESIS : '(' ;
RIGHT_PARENTHESIS : ')' ;
END_OF_STATEMENT : ';' ;

IDENTIFIER  : [a-zA-Z_]+([a-zA-Z_]|DIGIT09)*;

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
