grammar LSystem;

program 	        : (axiom | iteration | distance | angle | alias_def | module_def)* transformation*;

axiom               : 'axiom:' module+ EndOfLine;
iteration           : 'iteration:' IntegerConstant EndOfLine;
distance            : 'distance:' FloatConstant EndOfLine;
angle               : 'angle:' FloatConstant EndOfLine;

transformation      : module TransformOperator probability? module+ EndOfLine;
probability         : '(' FloatConstant ')';
module              : StringIdentifier;

alias_def           : 'alias' StringIdentifier '=' StringIdentifier EndOfLine;
module_def          : 'module' StringIdentifier '=' StringIdentifier EndOfLine;

ModuleDefinition
    : 'module' StringIdentifier AssignOperator StringIdentifier EndOfLine
    ;

IntegerConstant
    : ( '-' | '+' ) ? DIGIT09+
    ;

FloatConstant
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
