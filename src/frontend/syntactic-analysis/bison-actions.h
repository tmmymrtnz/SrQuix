#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../backend/support/shared.h"
#include "../../backend/semantic-analysis/abstract-syntax-tree.h"

/**
 * Se definen las acciones a ejecutar sobre cada regla de producción de la
 * gramática. El objetivo de cada acción debe ser el de construir el nodo
 * adecuado que almacene la información requerida en el árbol de sintaxis
 * abstracta (i.e., el AST).
 */

Program * ProgramGrammarAction(const Declaration * declaration);

Declaration * DeclarationTypeGrammarAction(const DeclareType * declare_type, const Declaration * declaration);

Declaration * DeclarationNodeGrammarAction(const DeclareNode * declare_node, const Declaration * declaration);

Declaration * DeclarationConcatGrammarAction(const Concat * concat, const Declaration * declaration);

DeclareType * DeclareTypeGrammarAction(const ComponentType * componentType, const ComponentDefRec * component_def_rec, const Params * params);

ComponentDefRec * ComaTextGrammarAction(const char * componentName, const ComponentDefRec * component_def_rec);

ComponentDefRec * ComponentDefGrammarAction(const char * componentName, const int value, const ComponentDefRec * component_def_rec);

ComponentType * ComponentTypeResistorGrammarAction();
ComponentType * ComponentTypeBatteryGrammarAction();
ComponentType * ComponentTypeInductorGrammarAction();
ComponentType * ComponentTypeCapacitorGrammarAction();
ComponentType * ComponentTypeAmmeterGrammarAction();
ComponentType * ComponentTypeVoltmeterGrammarAction();
ComponentType * ComponentTypeOhmMeterGrammarAction();
ComponentType * ComponentTypeSinglePhaseVolGrammarAction();

Params * ParamsGrammarAction(const ComaParameter * comaParameter);

ComaParameter * ComaParameterGrammarAction(const Parameter * parameter, const ComaParameter * comaParameter);

Parameter * ParameterShowNameGrammarAction(const Boolean * boolean);

Boolean * BooleanTrueGrammarAction();
Boolean * BooleanFalseGrammarAction();

DeclareNode * DeclareNodeGrammarAction(const char * nodeName, const DeclareNode * declareNode);

Concat * ConcatToGrammarAction(const char * componentName, const Concat * concat);

Concat * ConcatPlusGrammarAction(const char * componentName, const Concat * concat);

int ConstantGrammarAction(const int value);

char * ComponentNameGrammarAction(const char * componentName);

#endif
