grammar LCode;

program: statement* ;
statement: 'const' type IDENTIFIER ASSIGNATION expression END_OF_STATEMENT                                           #ConstantDecl
         | 'alias' function_call ASSIGNATION function_call+ END_OF_STATEMENT                                    #AliasDecl
         | 'axiom' expression END_OF_STATEMENT                                                                  #AxiomDecl
         | 'rewrite' IDENTIFIER LPARENT expression_list RPARENT ASSIGNATION function_call+ END_OF_STATEMENT     #RewriteRuleDecl
         ;

type: 'void'
    | 'integer'
    | 'float'
    | 'boolean'
    | 'string'
    ;

function_call: IDENTIFIER LPARENT expression_list RPARENT ;
expression_list: expression (',' expression)* ;

expression: SUB expression                                              #Minus
          | IDENTIFIER LPARENT expression (',' expression)* RPARENT     #Call
          | expression (MUL|DIV) expression                             #MultDiv
          | expression (ADD|SUB) expression                             #AddSub
          | LITTERAL_INTEGER                                            #Integer
          | LITTERAL_FLOAT                                              #Float
          | LITTERAL_STRING                                             #String
          | LITTERAL_BOOLEAN                                            #Boolean
          | IDENTIFIER                                                  #Identifier
          | LPARENT expression RPARENT                                  #EnclosedExpression
          ;

ADD : '+' ;
SUB : '-' ;
MUL : '*' ;
DIV : '/' ;
ASSIGNATION : '=' ;
LPARENT : '(' ;
RPARENT : ')' ;
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
WS: (' ' | '\t')+ -> skip;
