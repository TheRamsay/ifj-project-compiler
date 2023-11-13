grammar ifj;

prog:
	'func' FUNC_ID '(' func_params ')' return_def '{' statement_list '}' prog
	| statement prog
	|;

return_def: '->' TYPE |;
func_params: PARAM_OUT_ID PARAM_IN_ID ':' TYPE func_params_n |;
func_params_n:
	',' PARAM_OUT_ID PARAM_IN_ID ':' TYPE func_params_n
	|;

statement_list: statement statement_list |;

statement:
	'if (' expr ') {' statement_list '} else {' statement_list '}'
	| 'while (' expr ') {' statement_list '}'
	| 'break'
	| 'continue'
	| 'return' optional_expr
	| FUNC_ID '(' call_params ')'
	| VAR_DEFINITION IDENTIFIER var_definition_value
	| expr;

var_definition_value: '=' expr |;

optional_expr: expr |;

expr: term | FUNC_ID '(' call_params ')';

call_params: call_param_kw term call_param_n |;
call_param_n: call_param_kw term call_param_n |;
call_param_kw: PARAM_OUT_ID ':' |;

term: IDENTIFIER | LITERAL;

LITERAL: STRING_LITERAL | INTEGER_LITERAL | 'nil';

VAR_DEFINITION: VAR_DEFINITION_KW | VAR_DEFINITION_KW ':' TYPE;
VAR_DEFINITION_KW: 'var' | 'let';

TYPE: 'Int' | 'Double' | 'String';

FUNC_ID: IDENTIFIER;
PARAM_OUT_ID: IDENTIFIER;
PARAM_IN_ID: IDENTIFIER;
IDENTIFIER: [a-z][a-z0-9]*;
INTEGER_LITERAL: [0-9]+;
STRING_LITERAL: '"' ~[<>]+ '"';