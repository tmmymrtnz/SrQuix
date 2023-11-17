#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include "../semantic-analysis/symbol-table.h"

typedef struct ComponentTypeLatex {
	ComponentType component_type;
	char * latex_value;
    char * label_type;
    char * measurement;
} ComponentTypeLatex;

typedef struct Coords {
    int x;
    int y;
} Coords;

typedef struct ComponentLine {
    node_t * start_node;
    Coords start_coords;

    node_t * end_node;
    Coords end_coords;

    int line_size;
} ComponentLine;

void Generator(int result, symbol_t * symbolTable);


void connectNodes(ComponentLine component_lines[], int line_count,  FILE * filePointer);

void printLine(ComponentLine component_line);

#endif
