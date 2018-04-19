grammar LSystem;

fragment DIGIT09 : [0-9] ;
fragment DIGIT19 : [1-9] ;
fragment TRUE : 'true' ;
fragment FALSE : 'false' ;
fragment NEWLINE   : '\r' '\n' | '\n' | '\r';

program 	   : axiom (iterations | distance |  angle | initial_angle | alias)* transformation*;
axiom          : 'axiom:' module+ EndOfLine;
iterations     : 'iterations:' Integer EndOfLine;
distance       : 'distance:' Float EndOfLine;
angle          : 'angle:' Float EndOfLine;
initial_angle  : 'initialAngle:' Float EndOfLine;
alias          : 'alias' Identifier '=' Identifier EndOfLine;
transformation : module TransformOperator probability? module+ EndOfLine;
probability    : '(' Float ')';
module         : Identifier parameter_pack?;
parameter_pack : ('(' parameter ( ',' parameter )* ')') | '(' ')';
parameter      : expression;
expression     : Float | Integer | Boolean;

Integer
    : ( '-' | '+' ) ? DIGIT09+
    ;

Float
    : ( '-' | '+' ) ? DIGIT09+ '.' DIGIT09*
    ;

Boolean
    : TRUE
    | FALSE
    ;

AssignOperator
    : '='
    ;

TransformOperator
	: '->'
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

