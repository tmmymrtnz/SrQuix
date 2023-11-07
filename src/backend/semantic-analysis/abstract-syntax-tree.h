#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

/**
* Se realiza este tipo de definiciones cuando el tipo de dato debe
* auto-referenciarse, como es el caso de una "Expression", que está compuesta
* de otras 2 expresiones.
*/
typedef struct Expression Expression;

/**
* Para cada no-terminal se define una nueva estructura que representa su tipo
* de dato y, por lo tanto, su nodo en el AST (Árbol de Sintaxis Abstracta).
*/
typedef struct {
	int value;
} Constant;

/**
* En caso de que un no-terminal ("Factor" en este caso), posea más de una
* regla de producción asociada, se crea además de la estructura un enumerado
* que identitifque cada reducción posible (cada regla de producción). Luego,
* se agrega una instancia de este enumerado dentro del nodo del no-terminal.
*
* De este modo, al recorrer el AST, es posible determinar qué nodos hijos
* posee según el valor de este enumerado.
*/

typedef enum {
	T_RESISTOR,
	T_BATTERY,
	T_INDUCTOR,
	T_CAPACITOR,
	T_AMMETER,
	T_VOLTMETER,
	T_OHMMETER,
	T_SINGLEPHASEVOL
} ComponentType;

typedef enum {
	T_FALSE,
	T_TRUE
} Boolean;

typedef enum {
	T_SHOW_NAME
} ParameterType;

typedef struct {
	const ParameterType * parameter;
	const Boolean * boolean;
} Parameter;

typedef struct ComaParameter ComaParameter;

struct ComaParameter{
	const Parameter * parameter;

	const ComaParameter * coma_parameter;
};

typedef struct ComponentDefRec ComponentDefRec;

struct ComponentDefRec{
	const char * component_name;
	int constant;

	ComponentDefRec * component_def_rec;
};

typedef struct {
	const ComaParameter * coma_parameter;
} Params;

typedef struct {
	ComponentType * component_type;
	ComponentDefRec * component_def_rec; // handles both cases, with and without values
	const Params * params;
} DeclareType;

typedef struct DeclareNode DeclareNode;

struct DeclareNode {
	const char * name;
	DeclareNode * declare_node;
};

typedef struct Concat Concat;

typedef enum {
	T_GREATER_THAN,
	T_ADD
} ConcatType;

struct Concat {
	char * name;
	ConcatType concat_type;
	const Concat * concat;
};

typedef enum {
	T_TYPE,
	T_NODE,
	T_CONCAT
} DeclarationType;

typedef struct Declaration Declaration;

struct Declaration {
	DeclarationType declaration_type;
	union {
		const DeclareType * declare_type;
		const DeclareNode * declare_node;
		const Concat * concat;
	} content;
	const Declaration * declaration;
};

typedef struct {
	const Declaration * declaration;
} Program;

#endif
