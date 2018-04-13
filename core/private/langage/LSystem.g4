grammar LSystem;

fragment DIGIT09 : [0-9] ;
fragment DIGIT19 : [1-9] ;
fragment TRUE : 'true' ;
fragment FALSE : 'false' ;
fragment NEWLINE   : '\r' '\n' | '\n' | '\r';

program 		                : axiom EndOfLine (iterations ';')? (distance ';')? (angle ';')? transformation*;
//global_variable_declarations    : axiom iterations? distance? angle? initialAngle?;
axiom                           : 'axiom:' Identifier+;
iterations                      : 'iterations:' UnsignedInteger;
distance                        : 'distance:' UnsignedFloat;
angle                           : 'angle:' Float;
initial_angle                   : 'initialAngle:' Float;

transformation              : Identifier TransformOperator Identifier+ EndOfLine;

UnsignedInteger
    : DIGIT19 DIGIT09*
    ;

Integer
    : [-|+]? DIGIT09+
    ;

UnsignedFloat
    : DIGIT19 DIGIT09* '.' ? DIGIT09*
    ;

Float
    : ('-' | '+')? UnsignedFloat
    ;

Boolean
    : TRUE
    | FALSE
    ;

TransformOperator
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

