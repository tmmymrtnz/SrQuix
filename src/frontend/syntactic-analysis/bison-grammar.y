%{

#include "bison-actions.h"

%}

// Tipos de dato utilizados en las variables semánticas ($$, $1, $2, etc.).
%union {
	// No-terminales (backend).
	Program * program;
	float constant;
	char * component_name;
	char * node_name;
	Declaration * declaration;
	Concat * concat;
	DeclareType * declare_type;
	ComponentDefRec * component_def_onlyname;
	// SemiColonText semi_colon_text;
	DeclareNode * node_def_rec;
	ComponentType * component_type;
	ParameterType * parameter_type;
	Boolean * boolean;
	DeclareNode * declare_node;

	ComponentDefRec * component_def;
	ComponentDefRec * constant_def_rec;
	
	Params * params;
	ComaParameter * parameter_def_rec;
	Parameter * parameter;

	// No-terminales (frontend).
	/*
	int program;
	int constant;
	int component_name;
	int node_name;
	int declaration;
	int concat;
	int declare_type;
	int component_def_onlyname;
	int node_def_rec;
	int component_type;
	int boolean;
	int declare_node;

	int component_def;
	int component_def_rec;
	
	int params;
	int parameter_def_rec;
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
%token <component_type> AMMETER
%token <component_type> LAMP
%token <component_type> GENERIC_COMP

%token <component_def_onlyname> COMA
%token <token> EQUAL

%token <char> SEMICOLON

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
%type <component_def_onlyname> component_def_onlyname
%type <node_def_rec> node_def_rec
%type <component_def> component_def
%type <constant_def_rec> constant_def_rec
%type <component_type> component_type
%type <params> params
%type <parameter_def_rec> parameter_def_rec
%type <parameter> parameter
%type <boolean> boolean
%type <declare_node> declare_node
%type <constant> constant
%type <component_name> component_name
%type <node_name> node_name

%type <concat> concat

// El símbolo inicial de la gramatica.
%start program

%%

program: declaration												{ $$ = ProgramGrammarAction($1); }
	;

declaration: declare_type SEMICOLON declaration						{ $$ = DeclarationTypeGrammarAction($1, $3);	}
	| declare_node SEMICOLON declaration							{ $$ = DeclarationNodeGrammarAction($1, $3);	}
	| concat SEMICOLON declaration									{ $$ = DeclarationConcatGrammarAction($1, $3);	}
	| %empty														{ $$ = NULL; }
	;

declare_type: component_type component_def_onlyname						{ $$ = DeclareTypeGrammarAction($1, $2, NULL); 	}
	| component_type component_def params								{ $$ = DeclareTypeGrammarAction($1, $2, $3); 	}
	;

component_def_onlyname: component_name COMA component_def_onlyname		{ $$ = ComponentNameRecGrammarAction($1, $3);	 }
	| component_name													{ $$ = ComponentNameRecGrammarAction($1, NULL);	 }
	;

component_def: component_def_onlyname EQUAL OPEN_BRACKET constant_def_rec CLOSE_BRACKET		{ $$= ComponentDefGrammarAction($1, $4); }
	;

constant_def_rec: constant COMA constant_def_rec						{ $$ = ConstantRecGrammarAction($1, $3);	 }
	| constant															{ $$ = ConstantRecGrammarAction($1, NULL);	 }
	;

component_type: RESISTOR											{ $$ = ComponentTypeResistorGrammarAction();	}
	| BATTERY														{ $$ = ComponentTypeBatteryGrammarAction();		}
	| INDUCTOR														{ $$ = ComponentTypeInductorGrammarAction(); 	}
	| CAPACITOR														{ $$ = ComponentTypeCapacitorGrammarAction(); 	}
	| AMMETER														{ $$ = ComponentTypeAmmeterGrammarAction(); 	}
	| VOLTMETER														{ $$ = ComponentTypeVoltmeterGrammarAction(); 	}
	| LAMP															{ $$ = ComponentTypeLampGrammarAction(); 	}
	| GENERIC_COMP													{ $$ = ComponentTypeGenericGrammarAction(); }
	;

params: OPEN_PARENTHESIS parameter_def_rec CLOSE_PARENTHESIS		{ $$ = ParamsGrammarAction($2); }
	| %empty														{ $$ = NULL; }
	;

parameter_def_rec: parameter COMA parameter_def_rec					{ $$ = ComaParameterGrammarAction($1, $3); 	}
	| parameter														{ $$ = ComaParameterGrammarAction($1, NULL);}
	;

parameter: SHOW_NAME EQUAL boolean									{ $$ = ParameterShowNameGrammarAction($3); }
	;

boolean: TRUE														{ $$ = BooleanTrueGrammarAction();	 }
	| FALSE															{ $$ = BooleanFalseGrammarAction();	 }
	;

node_def_rec: COMA node_name node_def_rec							{ $$ = DeclareNodeGrammarAction($2, $3); }
	| %empty														{ $$ = NULL; }
	;

declare_node: NODE node_name node_def_rec							{ $$ = DeclareNodeGrammarAction($2, $3); }
	;

concat: component_name GREATER_THAN concat							{ $$ = ConcatToGrammarAction($1, $3); }
	| node_name ADD concat											{ $$ = ConcatPlusGrammarAction($1, $3); }
	| component_name 												{ $$ = ConcatToGrammarAction($1, NULL); }
	;

constant: INTEGER													{ $$ = ConstantIntegerGrammarAction($1); }
	| REAL															{ $$ = ConstantRealGrammarAction($1); }
	;

component_name: TEXT												{ $$ = ComponentNameGrammarAction($1); }
	;

node_name: TEXT														{ $$ = ComponentNameGrammarAction($1); }
	;
%%
