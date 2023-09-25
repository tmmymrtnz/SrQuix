%{

#include "bison-actions.h"

%}

// Tipos de dato utilizados en las variables semánticas ($$, $1, $2, etc.).
%union {
	// No-terminales (backend).
	/*
	Program program;
	Expression expression;
	Factor factor;
	Constant constant;
	...
	*/

	// No-terminales (frontend).
	int program;
	int expression;
	int factor;
	int constant;

	int declarations;
	int concat;
	int end_of_line;
	int declare_type;
	int coma_text;
	int type;
	int boolean;
	int declare_nodes;

	int matching_params;
	int matching_params_rec;
	
	int params;
	int coma_parameter;
	int parameter;

	// Terminales.
	token token;
	int integer;
	char * text;
}

// Un token que jamás debe ser usado en la gramática.
%token <token> ERROR

// IDs y tipos de los tokens terminales generados desde Flex.
%token <token> RESISTANCE
%token <token> SOURCE
%token <token> INDUCTANCE
%token <token> CAPACITANCE

%token <token> COMA
%token <token> EQUAL

%token <token> SEMICOLON

%token <token> NODE

%token <token> ADD
%token <token> GREATER_THAN

%token <token> SHOW_NAME

%token <token> TRUE
%token <token> FALSE

%token <token> SUB
%token <token> MUL
%token <token> DIV

%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS

%token <token> OPEN_BRACKET
%token <token> CLOSE_BRACKET

%token <integer> INTEGER
%token <integer> TEXT

// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program
%type <expression> expression
%type <factor> factor
%type <constant> constant

%type <concat> concat
%type <end_of_line> end_of_line

// Reglas de asociatividad y precedencia (de menor a mayor).
%left ADD SUB
%left MUL DIV

// El símbolo inicial de la gramatica.
%start program

%%

program: declarations												{ 0; }
	| expression													{ $$ = ProgramGrammarAction($1); }
	;

declarations: declare_type end_of_line declarations					{ 0; }
	| declare_nodes end_of_line declarations						{ 0; }
	| concat end_of_line declarations								{ 0; }
	|																{ 0; }
	;

declare_type: type TEXT coma_text									{ 0; }
	| type matching_params											{ 0; }

coma_text: COMA TEXT coma_text										{ 0; }
	| 																{ 0; }
	;

matching_params: TEXT matching_params_rec INTEGER CLOSE_BRACKET params		{ 0; }

matching_params_rec: COMA TEXT matching_params_rec INTEGER COMA		{ 0; }
	|  EQUAL OPEN_BRACKET											{ 0; }
	;

type: RESISTANCE													{ 0; }
	| SOURCE														{ 0; }
	| INDUCTANCE													{ 0; }
	| CAPACITANCE													{ 0; }
	;

params: OPEN_PARENTHESIS coma_parameter CLOSE_PARENTHESIS			{ 0; }
	|																{ 0; }
	;

coma_parameter: parameter COMA coma_parameter						{ 0; }
	| parameter														{ 0; }
	;

parameter: SHOW_NAME EQUAL boolean									{ 0; }
	;

boolean: TRUE														{ 0; }
	| FALSE															{ 0; }
	;

declare_nodes: NODE TEXT coma_text									{ 0; }

concat: TEXT GREATER_THAN concat									{ 0; }
	| TEXT ADD concat												{ 0; }
	| TEXT 															{ 0; }
	;

end_of_line: SEMICOLON												{ 0; }
	;






expression: expression[left] ADD expression[right]					{ $$ = AdditionExpressionGrammarAction($left, $right); }
	| expression[left] SUB expression[right]						{ $$ = SubtractionExpressionGrammarAction($left, $right); }
	| expression[left] MUL expression[right]						{ $$ = MultiplicationExpressionGrammarAction($left, $right); }
	| expression[left] DIV expression[right]						{ $$ = DivisionExpressionGrammarAction($left, $right); }
	| factor														{ $$ = FactorExpressionGrammarAction($1); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS				{ $$ = ExpressionFactorGrammarAction($2); }
	| constant														{ $$ = ConstantFactorGrammarAction($1); }
	;

constant: INTEGER													{ $$ = IntegerConstantGrammarAction($1); }
	;

%%
