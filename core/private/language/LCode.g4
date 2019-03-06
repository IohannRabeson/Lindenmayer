grammar LCode;

program: statement* ;
statement: 'const' type IDENTIFIER ASSIGNATION const_expression END_OF_STATEMENT                #ConstantDecl
         | 'alias' function_call ASSIGNATION function_call+ END_OF_STATEMENT                    #AliasDecl
         | 'axiom' expression END_OF_STATEMENT                                                  #AxiomDecl
         | 'rewrite' function_call RIGHT_PARENTHSIS ASSIGNATION function_call+ END_OF_STATEMENT #RewriteRuleDecl
         ;

type: 'void'
    | 'integer'
    | 'float'
    | 'boolean'
    | 'string'
    ;

function_call: IDENTIFIER LEFT_PARENTHSIS expression_list RIGHT_PARENTHSIS ;
expression_list: expression (',' expression)* ;

expression: SUBSTRACTION expression                                                 #NegativeExpression
          | function_call                                                           #FunctionCall
          | expression MULTIPLICATION expression                                    #Multiplication
          | expression DIVISION expression                                          #Division
          | expression ADDITION expression                                          #Addition
          | expression SUBSTRACTION expression                                      #Substraction
          | LITTERAL_INTEGER                                                        #Integer
          | LITTERAL_FLOAT                                                          #Float
          | LITTERAL_STRING                                                         #String
          | LITTERAL_BOOLEAN                                                        #Boolean
          | IDENTIFIER                                                              #Identifier
          | LEFT_PARENTHSIS expression RIGHT_PARENTHSIS                             #EnclosedExpression
          ;

const_expression: SUBSTRACTION const_expression                     #ConstNegativeExpression
                | const_expression MULTIPLICATION const_expression  #ConstMultiplication
                | const_expression DIVISION const_expression        #ConstDivision
                | const_expression ADDITION const_expression        #ConstAddition
                | const_expression SUBSTRACTION const_expression    #ConstSubstraction
                | LITTERAL_INTEGER                                  #ConstInteger
                | LITTERAL_FLOAT                                    #ConstFloat
                | LITTERAL_STRING                                   #ConstString
                | LITTERAL_BOOLEAN                                  #ConstBoolean
                | IDENTIFIER                                        #ConstIdentifier
                | LEFT_PARENTHSIS const_expression RIGHT_PARENTHSIS #ConstEnclosedExpression
                ;

ADDITION : '+' ;
SUBSTRACTION : '-' ;
MULTIPLICATION : '*' ;
DIVISION : '/' ;
ASSIGNATION : '=' ;
LEFT_PARENTHSIS : '(' ;
RIGHT_PARENTHSIS : ')' ;
END_OF_STATEMENT : ';' ;

IDENTIFIER  : [a-zA-Z_]+ ;

LITTERAL_INTEGER
    : '-'? DIGIT09+
    ;

LITTERAL_FLOAT
    : '-'? DIGIT09+ '.' DIGIT09*
    | '-'? DIGIT09* '.' DIGIT09+
    ;

LITTERAL_STRING
    : ('"'[ a-zA-Z0-9]*?'"')
    ;

LITTERAL_BOOLEAN
    : 'true'
    | 'false'
    ;

fragment DIGIT09 : [0-9] ;
fragment DIGIT19 : [1-9] ;

LINE_COMMENT: '//' .*? '\r'? '\n' -> skip;
COMMENT: '/*' .*? '*/' -> skip;
NEWLINE: ('\r'? '\n') -> skip;
WHITESPACES: (' ' | '\t')+ -> skip;
