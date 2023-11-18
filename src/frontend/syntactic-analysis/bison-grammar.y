%{

#include "bison-actions.h"

%}

// Tipos de dato utilizados en las variables semánticas ($$, $1, $2, etc.).
%union {
	// No-terminales (backend).
	Program * program;
	int constant;
	char * component_name;
	char * node_name;
	Declaration * declaration;
	Concat * concat;
	void * end_of_line;
	DeclareType * declare_type;
	ComponentDefRec * coma_text;
	// SemiColonText semi_colon_text;
	DeclareNode * coma_node;
	ComponentType * component_type;
	Boolean * boolean;
	DeclareNode * declare_node;

	ComponentDefRec * component_def;
	ComponentDefRec * component_def_rec;
	
	Params * params;
	ComaParameter * coma_parameter;
	Parameter * parameter;

	// No-terminales (frontend).
	/*
	int program;
	int constant;
	int component_name;
	int node_name;
	int declaration;
	int concat;
	int end_of_line;
	int declare_type;
	int coma_text;
	int coma_node;
	int component_type;
	int boolean;
	int declare_node;

	int component_def;
	int component_def_rec;
	
	int params;
	int coma_parameter;
	int parameter;
	*/
	
	// Terminales.
	token token;
	int integer;
	float real;
	char * text;
}

// Un token que jamás debe ser usado en la gramática.
%token <token> ERROR

// IDs y tipos de los tokens terminales generados desde Flex.
%token <component_type> RESISTOR
%token <component_type> BATTERY
%token <component_type> INDUCTOR
%token <component_type> CAPACITOR
%token <component_type> VOLTMETER
%token <component_type> OHMMETER
%token <component_type> AMMETER
%token <component_type> SINGLEPHASEVOL

%token <coma_text> COMA
%token <token> EQUAL  // eeee ??

%token <char> SEMICOLON //e?

%token <declare_node> NODE

%token <concat_type> ADD
%token <concat_type> GREATER_THAN

%token <parameter_type> SHOW_NAME

%token <boolean> TRUE
%token <boolean> FALSE

%token <text> OPEN_PARENTHESIS
%token <text> CLOSE_PARENTHESIS

%token <text> OPEN_BRACKET
%token <text> CLOSE_BRACKET

%token <integer> INTEGER
%token <real> REAL
%token <text> TEXT


// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program
%type <declaration> declaration
%type <declare_type> declare_type
%type <coma_text> coma_text
%type <coma_node> coma_node
%type <component_def> component_def
%type <component_def_rec> component_def_rec
%type <component_type> component_type
%type <params> params
%type <coma_parameter> coma_parameter
%type <parameter> parameter
%type <boolean> boolean
%type <declare_node> declare_node
%type <constant> constant
%type <component_name> component_name
%type <node_name> node_name

%type <concat> concat
%type <end_of_line> end_of_line

// Reglas de asociatividad y precedencia (de menor a mayor).
// %left ADD SUB
// %left MUL DIV

// El símbolo inicial de la gramatica.
%start program

%%

program: declaration												{ $$ = ProgramGrammarAction($1); }
	;

declaration: declare_type end_of_line declaration					{ $$ = DeclarationTypeGrammarAction($1, $3);	}
	| declare_node end_of_line declaration							{ $$ = DeclarationNodeGrammarAction($1, $3);	}
	| concat end_of_line declaration								{ $$ = DeclarationConcatGrammarAction($1, $3);	}
	|																{ $$ = NULL; }
	;

declare_type: component_type coma_text									{ $$ = DeclareTypeGrammarAction($1, $2, NULL); 	}
	| component_type component_def params								{ $$ = DeclareTypeGrammarAction($1, $2, $3); 	}
	;

coma_text: component_name COMA coma_text								{ $$ = ComaTextGrammarAction($1, $3);	 }
	| component_name													{ $$ = ComaTextGrammarAction($1, NULL);	 }
	;

component_def: component_name component_def_rec constant CLOSE_BRACKET		{ $$= ComponentDefGrammarAction($1, $3, $2); }
	;

component_def_rec: COMA component_name component_def_rec constant COMA		{ $$= ComponentDefGrammarAction($2, $4, $3); }
	|  EQUAL OPEN_BRACKET													{ $$= NULL; }
	;

component_type: RESISTOR											{ $$ = ComponentTypeResistorGrammarAction();	}
	| BATTERY														{ $$ = ComponentTypeBatteryGrammarAction();		}
	| INDUCTOR														{ $$ = ComponentTypeInductorGrammarAction(); 	}
	| CAPACITOR														{ $$ = ComponentTypeCapacitorGrammarAction(); 	}
	| AMMETER														{ $$ = ComponentTypeAmmeterGrammarAction(); 	}
	| VOLTMETER														{ $$ = ComponentTypeVoltmeterGrammarAction(); 	}
	| OHMMETER														{ $$ = ComponentTypeOhmMeterGrammarAction(); 	}
	| SINGLEPHASEVOL												{ $$ = ComponentTypeSinglePhaseVolGrammarAction(); }
	;

params: OPEN_PARENTHESIS coma_parameter CLOSE_PARENTHESIS			{ $$ = ParamsGrammarAction($2); }
	|																{ $$ = NULL; }
	;

coma_parameter: parameter COMA coma_parameter						{ $$ = ComaParameterGrammarAction($1, $3); 	}
	| parameter														{ $$ = ComaParameterGrammarAction($1, NULL);}
	;

parameter: SHOW_NAME EQUAL boolean									{ $$ = ParameterShowNameGrammarAction($3); }
	;

boolean: TRUE														{ $$ = BooleanTrueGrammarAction();	 }
	| FALSE															{ $$ = BooleanFalseGrammarAction();	 }
	;

coma_node: COMA node_name coma_node									{ $$ = DeclareNodeGrammarAction($2, $3); }
	|																{ $$ = NULL; }
	;

declare_node: NODE node_name coma_node								{ $$ = DeclareNodeGrammarAction($2, $3); }
	;

concat: component_name GREATER_THAN concat							{ $$ = ConcatToGrammarAction($1, $3); }
	| node_name ADD concat											{ $$ = ConcatPlusGrammarAction($1, $3); }
	| component_name 												{ $$ = ConcatToGrammarAction($1, NULL); }
	;

end_of_line: SEMICOLON												{ $$ = NULL; }
	;

constant: INTEGER													{ $$ = ConstantGrammarAction($1); }
	| REAL															{ $$ = ConstantGrammarAction($1); }
	;

component_name: TEXT												{ $$ = ComponentNameGrammarAction($1); }
	;

node_name: TEXT														{ $$ = ComponentNameGrammarAction($1); }
	;
%%
