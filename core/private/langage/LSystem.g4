grammar LSystem;

fragment DIGIT09 : [0-9] ;
fragment DIGIT19 : [1-9] ;
fragment TRUE : 'true' ;
fragment FALSE : 'false' ;
fragment NEWLINE   : '\r' '\n' | '\n' | '\r';

program 		  : axiom transformation*;
axiom   		  : Identifier+ ';';
transformation    : Identifier Assign Identifier+ EndOfLine;
constant_def      : 'const' Identifier '=' expression EndOfLine;
expression        : Integer | Float | Boolean;

Integer
    : [-|+] ? DIGIT09+
    ;

Float
    : [-|+]? DIGIT19 DIGIT09* '.' ? DIGIT09*
    ;

Boolean
    : TRUE
    | FALSE
    ;

Assign
	: '='
	;

Identifier
	: [a-zA-Z_]+ | '[' | ']'
	;

EndOfLine
    : ';'
    ;

Newline
    : ( '\r' '\n'?
      | '\n'
      )
      -> skip
    ;

Whitespace
    : [ \t]+
      -> skip
    ;

