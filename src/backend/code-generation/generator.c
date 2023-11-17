#include "../support/logger.h"
#include "generator.h"

/**
 * Implementación de "generator.h".
 */

ComponentTypeLatex typeTable[] = {
	{T_RESISTOR, 	"R",		"l",	"\\Omega"},
	{T_BATTERY, 	"battery",	"l",	"V"},
	{T_INDUCTOR, 	"inductor",	"l",	"H"}, 
	{T_CAPACITOR, 	"C", 		"l",	"F"}, 
	{T_AMMETER, 	"ammeter", 	"i_",	"mA"},
	{T_VOLTMETER, 	"voltmeter","l",	"kV"},
	{T_OHMMETER, 	"R",		"l",	"sus"},
	{T_SINGLEPHASEVOL, "R", 	"l",	"sus"},
	{0 , NULL}
};


char * getComponent(ComponentType * component_type) {
	int i = 0;
    while(typeTable[i].latex_value != NULL){
        if(typeTable[i].component_type == *component_type){
            return typeTable[i].latex_value;
        }
        i++;
    }

	return "Unknown";
}

char * getMeasurement(ComponentType * component_type) {
	int i = 0;
    while(typeTable[i].latex_value != NULL){
        if(typeTable[i].component_type == *component_type){
            return typeTable[i].measurement;
        }
        i++;
    }

	return "Unknown";
}

int x = 0;
int y = 0;

char * current_coord;


char * currentPos() {
	snprintf(current_coord, 20, "(%d,%d)", x, y);
	return current_coord;
}

char * goRight() {
	x += 2;
	snprintf(current_coord, 20, "(%d,%d)", x, y);
	return current_coord;
}


void generateComponent(component_t * component, FILE * filePointer) {
	if (component->constant == 0) {
		fprintf(filePointer,"\n\t\tto[%s, l=$%s$] %s", getComponent(component->component_type), component->component_name, goRight());
	} else {
		fprintf(filePointer,"\n\t\tto[%s, l=$%d\\:%s$] %s", getComponent(component->component_type), component->constant, getMeasurement(component->component_type), goRight());
	}
}

void closeComponent(FILE * filePointer) {
	fprintf(filePointer,"\t\\draw %s -- (%d,%d) -- (%d,%d) -- (0,0);\n", current_coord, x, y-2, 0, y-2);
}

#define MAX_LINES 10

void Generator(int result, symbol_t * symbolTable) {
	LogInfo("La expresion genera un circuito valido. Compilacion terminada con codigo: '%d'.", result);

 	FILE *filePointer; 

    filePointer = fopen("sirquix.tex", "w");

	 if (filePointer == NULL) {
        printf("Unable to create the LaTeX file.\n");
        return ;
    }



    ComponentLine *circuit_lines = (ComponentLine *)malloc(MAX_LINES * sizeof(ComponentLine));
	int line_count = 0;
	int current_line = 0;

    if (circuit_lines == NULL) {
        printf("Memory allocation failed\n");
        return ;
    }

	fprintf(filePointer,"\\documentclass{article}\n\\usepackage{circuitikz}\n\\title{srquix out}\n\\begin{document}\n\n");

	current_coord = (char *)malloc(20 * sizeof(char));

	fprintf(filePointer,"\\begin{circuitikz}");

	Node * current_node = (Node *)symbolTable->nodes->head;

	while (current_node != NULL) {
		for (int i = 0; i < 4; i++){
			if (		((node_t *)current_node->data)->dir[i] != NULL &&
						((node_t *)current_node->data)->dir_type[i] == COMPONENT_TYPE && 
						((component_t *)((node_t *)current_node->data)->dir[i])->next_type == COMPONENT_TYPE) 
				{
				line_count++;
				circuit_lines[current_line].start_node = (node_t *)current_node->data;
				circuit_lines[current_line].start_coords.x = x;
				circuit_lines[current_line].start_coords.y = y;
				circuit_lines[current_line].line_size = 0;

				fprintf(filePointer,"\n\t\\draw %s node[circ, label={above: %s}] {}", currentPos(), circuit_lines[current_line].start_node->name);

				component_t * current_component = (component_t *)((node_t *)current_node->data)->dir[i];
				int finished = 0;
				while (!finished) {
					generateComponent(current_component,filePointer);
					circuit_lines[current_line].line_size++;

					if (current_component->next_type == NODE_TYPE) {
						finished = 1;
					} else {
						current_component = (component_t *)current_component->next;
					}
				}

				circuit_lines[current_line].end_node = (node_t *)current_component->next;
				circuit_lines[current_line].end_coords.x = x;
				circuit_lines[current_line].end_coords.y = y;

				fprintf(filePointer,"\n\t\tnode[circ, label={above: %s}] {};", circuit_lines[current_line].end_node->name );

				// printLine(circuit_lines[current_line]);

				x = 0;
				y -= 2;
				current_line++;
				
			} else if (((node_t *)current_node->data)->dir[i] != NULL && ((node_t *)current_node->data)->dir_type[i] == NODE_TYPE) {
				// printf("\nonly nodes in direction\n");

				int exists = 0;
				for (int k = 0; k < current_line; k++) {
					if (circuit_lines[k].line_size == 0 
						&& circuit_lines[k].start_node == (node_t *)((node_t *)current_node->data)->dir[i]
						&& circuit_lines[k].end_node == (node_t *)current_node->data) {
						exists = 1;
					}
				}

				if ( !exists ) {
					line_count++;
					circuit_lines[current_line].start_node = (node_t *)current_node->data;
					circuit_lines[current_line].start_coords.x = x;
					circuit_lines[current_line].start_coords.y = y;
					circuit_lines[current_line].line_size = 0;
					fprintf(filePointer,"\n\t\\draw %s node[circ, label={above: %s}] {} ", currentPos(), circuit_lines[current_line].start_node->name);

					x += 2;

					circuit_lines[current_line].end_node = (node_t *)((node_t *)current_node->data)->dir[i];
					circuit_lines[current_line].end_coords.x = x;
					circuit_lines[current_line].end_coords.y = y;

					fprintf(filePointer,"-- %s node[circ, label={above: %s}] {};", currentPos() ,circuit_lines[current_line].end_node->name );

					x = 0;
					y -= 2;
					current_line++;
				}
			}
		}

		current_node = current_node->next;
	}

	connectNodes(circuit_lines, current_line, filePointer);

	fprintf(filePointer,"\n\\end{circuitikz}\n\n\\end{document}");

	fclose(filePointer);
	free(current_coord);
}

void connectNodes(ComponentLine component_lines[], int line_count, FILE * filePointer) {

	// Connect current lines
	for(int i=0; i < line_count;i++){
		for(int j=i+1; j<line_count;j++){
			if(strcmp(component_lines[i].start_node->name,component_lines[j].start_node->name)==0){
				fprintf(filePointer,"\n\t\\draw (%d,%d)  -- (%d,%d); ", component_lines[i].start_coords.x,component_lines[i].start_coords.y, component_lines[j].start_coords.x,component_lines[j].start_coords.y );
			}
			if(strcmp(component_lines[i].start_node->name,component_lines[j].end_node->name)==0){
				fprintf(filePointer,"\n\t\\draw (%d,%d)  -- (%d,%d); ", component_lines[i].start_coords.x,component_lines[i].start_coords.y, component_lines[j].end_coords.x,component_lines[j].end_coords.y );
			}
			if(strcmp(component_lines[i].end_node->name,component_lines[j].start_node->name)==0){
				fprintf(filePointer,"\n\t\\draw (%d,%d)  -- (%d,%d); ", component_lines[i].end_coords.x, component_lines[i].end_coords.y, component_lines[j].start_coords.x,component_lines[j].start_coords.y );
			}
			if(strcmp(component_lines[i].end_node->name,component_lines[j].end_node->name)==0){
				fprintf(filePointer,"\n\t\\draw (%d,%d)  -- (%d,%d); ", component_lines[i].end_coords.x, component_lines[i].end_coords.y, component_lines[j].end_coords.x,component_lines[j].end_coords.y );
			
			}
		}
	}
}

void printLine(ComponentLine component_line) {
	printf("\nStart node name: %s\n", component_line.start_node->name);
	printf("Start node coords: %d x - %d y\n", component_line.start_coords.x, component_line.start_coords.y);
	printf("End node name: %s\n", component_line.end_node->name);
	printf("End node coords: %d x - %d y\n", component_line.end_coords.x, component_line.end_coords.y);
}