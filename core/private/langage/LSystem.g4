grammar LSystem;

fragment DIGIT09 : [0-9] ;
fragment DIGIT19 : [1-9] ;
fragment TRUE : 'true' ;
fragment FALSE : 'false' ;
fragment NEWLINE   : '\r' '\n' | '\n' | '\r';

program 		                : axiom (iterations | distance |  angle | initial_angle | alias)* transformation*;
axiom                           : 'axiom:' Identifier+ EndOfLine;
iterations                      : 'iterations:' Integer EndOfLine;
distance                        : 'distance:' Float EndOfLine;
angle                           : 'angle:' Float EndOfLine;
initial_angle                   : 'initialAngle:' Float EndOfLine;
alias                           : 'alias' Identifier '=' Identifier EndOfLine;

transformation              : Identifier TransformOperator Identifier+ EndOfLine;

Integer
    : ( '-' | '+' ) ? DIGIT09+
    ;

Float
    : ( '-' | '+' ) ? DIGIT19 DIGIT09* '.' ? DIGIT09*
    ;

Boolean
    : TRUE
    | FALSE
    ;

TransformOperator
	: '='
	;

Identifier
	: [a-zA-Z_] | '[' | ']' | '+' | '-'
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

