#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include "../semantic-analysis/symbol-table.h"

typedef struct ComponentTypeLatex {
	ComponentType component_type;
	char * latex_value;
    char * label_type;
    char * measurement;
} ComponentTypeLatex;

void Generator(int result, symbol_t * symbolTable);

#endif
