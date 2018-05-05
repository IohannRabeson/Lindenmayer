grammar LSystem;

program 	        : (axiom | iteration | distance | angle | initial_angle | alias_def | module_def)* transformation*;

axiom               : 'axiom:' module+ EndOfLine;
iteration           : 'iteration:' Integer EndOfLine;
distance            : 'distance:' Float EndOfLine;
angle               : 'angle:' Float EndOfLine;
initial_angle       : 'initial_angle:' Float EndOfLine;

transformation      : module TransformOperator probability? module+ EndOfLine;
probability         : '(' Float ')';
module              : StringIdentifier;
expression          : Float | Integer | Boolean;

alias_def           : 'alias' StringIdentifier '=' StringIdentifier EndOfLine;
module_def          : 'module' StringIdentifier '=' StringIdentifier EndOfLine;

ModuleDefinition
    : 'module' StringIdentifier AssignOperator StringIdentifier EndOfLine
    ;

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

StringIdentifier
    :  (('a'..'z' | 'A'..'Z' | '_') ('a'..'z' | 'A'..'Z' | '_' | '0'..'9')*)
    |  '[' | ']' | '+' | '-'
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

fragment DIGIT09 : [0-9] ;
fragment DIGIT19 : [1-9] ;
fragment TRUE : 'true' ;
fragment FALSE : 'false' ;
fragment NEWLINE   : '\r' '\n' | '\n' | '\r';
