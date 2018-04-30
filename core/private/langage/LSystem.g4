grammar LSystem;

fragment DIGIT09 : [0-9] ;
fragment DIGIT19 : [1-9] ;
fragment TRUE : 'true' ;
fragment FALSE : 'false' ;
fragment NEWLINE   : '\r' '\n' | '\n' | '\r';

program 	        : (axiom | iteration | distance |  angle | initial_angle | alias | module_def)* transformation*;

axiom               : 'axiom:' module+ EndOfLine;
iteration           : 'iteration:' Integer EndOfLine;
distance            : 'distance:' Float EndOfLine;
angle               : 'angle:' Float EndOfLine;
initial_angle       : 'initial_angle:' Float EndOfLine;

alias               : 'alias' ModuleIdentifier '=' ModuleIdentifier EndOfLine;
module_def          : 'module' ModuleIdentifier '=' ModuleIdentifier+ EndOfLine;
transformation      : module TransformOperator probability? module+ EndOfLine;
probability         : '(' Float ')';
module              : ModuleIdentifier;
expression          : Float | Integer | Boolean;

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

ModuleIdentifier
	: [a-zA-Z_] | '[' | ']' | '+' | '-'
	;

StringIdentifier
    : [a-zA-Z_]
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

