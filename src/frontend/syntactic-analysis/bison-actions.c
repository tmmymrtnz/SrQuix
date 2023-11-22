#include "../../backend/domain-specific/calculator.h"
#include "../../backend/support/logger.h"
#include "bison-actions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * Implementación de "bison-actions.h".
 */

/**
* Esta función se ejecuta cada vez que se emite un error de sintaxis.
*/
void yyerror(const char * string) {
	LogErrorRaw("[ERROR] Mensaje: '%s', debido a '", string);
	for (int i = 0; i < yyleng; ++i) {
		switch (yytext[i]) {
			case '\n':
				LogErrorRaw("\\n");
			default:
				LogErrorRaw("%c", yytext[i]);
		}
	}
	LogErrorRaw("' (length = %d, linea %d).\n\n", yyleng, yylineno);
}

/**
* Esta acción se corresponde con el no-terminal que representa el símbolo
* inicial de la gramática, y por ende, es el último en ser ejecutado, lo que
* indica que efectivamente el programa de entrada se pudo generar con esta
* gramática, o lo que es lo mismo, que el programa pertenece al lenguaje.
*/
Program * ProgramGrammarAction(const Declaration * declaration) {
	Program * new_program = (Program *) malloc(sizeof(Program));
	new_program->declaration = declaration;
	int value = 0; // como representamos value con nuestra estructura?
	
	LogDebug("[Bison] ProgramGrammarAction(%d)", value);
	/*
	* "state" es una variable global que almacena el estado del compilador,
	* cuyo campo "succeed" indica si la compilación fue o no exitosa, la cual
	* es utilizada en la función "main".
	*/
	state.succeed = true;
	/*
	* Por otro lado, "result" contiene el resultado de aplicar el análisis
	* sintáctico mediante Bison, y almacenar el nood raíz del AST construido
	* en esta variable. Para el ejemplo de la calculadora, no hay AST porque
	* la expresión se computa on-the-fly, y es la razón por la cual esta
	* variable es un simple entero, en lugar de un nodo.
	*/
	state.result = value;
	return new_program;
}

Declaration * DeclarationTypeGrammarAction( const DeclareType * declare_type, const Declaration * declaration){
	Declaration * new_declaration = (Declaration *) malloc(sizeof(Declaration));
	new_declaration->content.declare_type = declare_type;
	new_declaration->declaration_type = T_TYPE;
	new_declaration->declaration = declaration;
	
	return new_declaration;
}

Declaration * DeclarationNodeGrammarAction(const DeclareNode * declare_node, const Declaration * declaration){
	Declaration * new_declaration = (Declaration *) malloc(sizeof(Declaration));
	new_declaration->content.declare_node = declare_node;
	new_declaration->declaration_type = T_NODE;
	new_declaration->declaration = declaration;
	return new_declaration;
}

Declaration * DeclarationConcatGrammarAction(const Concat * concat, const Declaration * declaration){
	Declaration * new_declaration = (Declaration *) malloc(sizeof(Declaration));
	new_declaration->content.concat = concat;
	new_declaration->declaration_type = T_CONCAT;
	new_declaration->declaration = declaration;
	return new_declaration;
}

ComponentType * ComponentTypeResistorGrammarAction(){
	ComponentType * new_component_type = (ComponentType *) malloc(sizeof(ComponentType));
	* new_component_type = T_RESISTOR;
	return new_component_type;
}

ComponentType * ComponentTypeBatteryGrammarAction(){
	ComponentType * new_component_type = (ComponentType *) malloc(sizeof(ComponentType));
	* new_component_type = T_BATTERY;
	return new_component_type;
}

ComponentType * ComponentTypeInductorGrammarAction(){
	ComponentType * new_component_type = (ComponentType *) malloc(sizeof(ComponentType));
	* new_component_type = T_INDUCTOR;
	return new_component_type;
}

ComponentType * ComponentTypeCapacitorGrammarAction(){
	ComponentType * new_component_type = (ComponentType *) malloc(sizeof(ComponentType));
	* new_component_type = T_CAPACITOR;
	return new_component_type;
}

ComponentType * ComponentTypeAmmeterGrammarAction(){
	ComponentType * new_component_type = (ComponentType *) malloc(sizeof(ComponentType));
	* new_component_type = T_AMMETER;
	return new_component_type;
}

ComponentType * ComponentTypeVoltmeterGrammarAction(){
	ComponentType * new_component_type = (ComponentType *) malloc(sizeof(ComponentType));
	* new_component_type = T_VOLTMETER;
	return new_component_type;
}

ComponentType * ComponentTypeLampGrammarAction(){
	ComponentType * new_component_type = (ComponentType *) malloc(sizeof(ComponentType));
	* new_component_type = T_LAMP;
	return new_component_type;
}

ComponentType * ComponentTypeGenericGrammarAction(){
	ComponentType * new_component_type = (ComponentType *) malloc(sizeof(ComponentType));
	* new_component_type = T_GENERIC;
	return new_component_type;
}

DeclareType * DeclareTypeGrammarAction(ComponentType * componentType, ComponentDefRec * component_def_rec, const Params * params){
	DeclareType * new_declare_type = (DeclareType *) malloc(sizeof(DeclareType));
	new_declare_type->component_type = componentType;
	new_declare_type->component_def_rec = component_def_rec;
	new_declare_type->params = params;

	addComponents(new_declare_type->component_def_rec, componentType);

	if(params != NULL){
		addParams(new_declare_type->component_def_rec, params->coma_parameter);
	}

	return new_declare_type;
}


ComponentDefRec * ComponentNameRecGrammarAction(char * componentName, ComponentDefRec * component_def_rec){
	ComponentDefRec * new_component_def_rec = (ComponentDefRec *) malloc(sizeof(ComponentDefRec));
	new_component_def_rec->component_name = componentName;
	new_component_def_rec->component_def_rec = component_def_rec;
	return new_component_def_rec;
}

ComponentDefRec * ConstantRecGrammarAction(const float value, ComponentDefRec * component_def_rec) {
	ComponentDefRec * new_component_def_rec = (ComponentDefRec *) malloc(sizeof(ComponentDefRec));
	new_component_def_rec->constant = value;
	new_component_def_rec->component_def_rec = component_def_rec;
	return new_component_def_rec;
}

ComponentDefRec * ComponentDefGrammarAction(ComponentDefRec * component_def_names, ComponentDefRec * component_def_values) {
	assignValuesToNames(component_def_names, component_def_values);
	return component_def_names;
}


Params * ParamsGrammarAction(ComaParameter * comaParameter){
	Params * new_params = (Params *) malloc(sizeof(Params));
	new_params->coma_parameter = comaParameter;
	return new_params;
}

ComaParameter * ComaParameterGrammarAction(const Parameter * parameter, const ComaParameter * comaParameter){
	ComaParameter * new_coma_parameter = (ComaParameter *) malloc(sizeof(ComaParameter));
	new_coma_parameter->parameter = parameter;
	new_coma_parameter->coma_parameter = comaParameter;
	return new_coma_parameter;
}

Parameter * ParameterShowNameGrammarAction(const Boolean * boolean){
	Parameter * new_parameter = (Parameter *) malloc(sizeof(Parameter));
	new_parameter->parameter = T_SHOW_NAME;
	new_parameter->boolean = boolean;
	return new_parameter;
}

Boolean * BooleanTrueGrammarAction(){
	Boolean * new_boolean = (Boolean *) malloc(sizeof(Boolean));
	* new_boolean = T_TRUE;
	return new_boolean;
}

Boolean * BooleanFalseGrammarAction(){
	Boolean * new_boolean = (Boolean *) malloc(sizeof(Boolean));
	* new_boolean = T_FALSE;
	return new_boolean;
}

DeclareNode * DeclareNodeGrammarAction(const char * nodeName, DeclareNode * declare_node){
	DeclareNode * new_declare_node = (DeclareNode *) malloc(sizeof(DeclareNode));
	new_declare_node->name = nodeName;
	new_declare_node->declare_node = declare_node;

	addNode(new_declare_node);

	return new_declare_node;
}

Concat * ConcatToGrammarAction(char * componentName, const Concat * concat){
	Concat * new_concat = (Concat *) malloc(sizeof(Concat));
	new_concat->name = componentName;
	new_concat->concat_type= T_GREATER_THAN;
	new_concat->concat = concat;

	if (concat != NULL) {
		concatTo(componentName, concat->name);
	}

	return new_concat;
}

Concat * ConcatPlusGrammarAction(char * componentName, const Concat * concat){
	Concat * new_concat = (Concat *) malloc(sizeof(Concat));
	new_concat->name = componentName;
	new_concat->concat_type = T_ADD;
	new_concat->concat = concat;

	if (concat != NULL) {
		concatNodes(componentName, concat->name);
	}

	return new_concat;
}


float ConstantIntegerGrammarAction(const int value){
	return (float)value;
}

float ConstantRealGrammarAction(const float value){
	return value;
}

char * ComponentNameGrammarAction(char * componentName){
    return componentName;
}

// int AdditionExpressionGrammarAction(const int leftValue, const int rightValue) {
// 	LogDebug("[Bison] AdditionExpressionGrammarAction(%d, %d)", leftValue, rightValue);
// 	return Add(leftValue, rightValue);
// }

// int SubtractionExpressionGrammarAction(const int leftValue, const int rightValue) {
// 	LogDebug("[Bison] SubtractionExpressionGrammarAction(%d, %d)", leftValue, rightValue);
// 	return Subtract(leftValue, rightValue);
// }

// int MultiplicationExpressionGrammarAction(const int leftValue, const int rightValue) {
// 	LogDebug("[Bison] MultiplicationExpressionGrammarAction(%d, %d)", leftValue, rightValue);
// 	return Multiply(leftValue, rightValue);
// }

// int DivisionExpressionGrammarAction(const int leftValue, const int rightValue) {
// 	LogDebug("[Bison] DivisionExpressionGrammarAction(%d, %d)", leftValue, rightValue);
// 	return Divide(leftValue, rightValue);
// }

// int FactorExpressionGrammarAction(const int value) {
// 	LogDebug("[Bison] FactorExpressionGrammarAction(%d)", value);
// 	return value;
// }

// int ExpressionFactorGrammarAction(const int value) {
// 	LogDebug("[Bison] ExpressionFactorGrammarAction(%d)", value);
// 	return value;
// }

// int ConstantFactorGrammarAction(const int value) {
// 	LogDebug("[Bison] ConstantFactorGrammarAction(%d)", value);
// 	return value;
// }

// int IntegerConstantGrammarAction(const int value) {
// 	LogDebug("[Bison] IntegerConstantGrammarAction(%d)", value);
// 	return value;
// }
