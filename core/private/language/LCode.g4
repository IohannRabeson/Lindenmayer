grammar LCode;

program: statement+ ;
statement: expression
         | 'const' IDENTIFIER ASSIGNATION expression ';'
         | 'alias' expression ASSIGNATION expression ';'
         | 'axiom' expression ';'
         | IDENTIFIER '(' expression (',' expression)* ')' REWRITE (IDENTIFIER '(' expression (',' expression)* ')')+ ';'
         | ';'
         | ENDOFLINE
         ;

expression: SUB expression
          | IDENTIFIER '(' expression (',' expression)* ')'
          | expression (MUL|DIV) expression
          | expression (ADD|SUB) expression
          | LITTERAL_INTEGER
          | LITTERAL_FLOAT
          | LITTERAL_STRING
          | LITTERAL_BOOLEAN
          | IDENTIFIER
          | '(' expression ')'
          ;

REWRITE : '->' ;
ADD : '+' ;
SUB : '-' ;
MUL : '*' ;
DIV : '/' ;
ASSIGNATION : '=' ;

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

ENDOFLINE
    : ';'
    ;

fragment DIGIT09 : [0-9] ;
fragment DIGIT19 : [1-9] ;

LINE_COMMENT: '//' .*? '\r'? '\n' -> skip;
COMMENT: '/*' .*? '*/' -> skip;
NEWLINE: ('\r'? '\n') -> skip;
WS: [' ' \t]+ -> skip;
