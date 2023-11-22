#include "../support/logger.h"
#include "generator.h"
#define MAX_LINES 10
#define EPSILON 0.0001
/**
 * Implementación de "generator.h".
 */

void generateComponent(component_t * component, FILE * filePointer);
void connectNodes(ComponentLine component_lines[], int line_count,  FILE * filePointer);
void printLine(ComponentLine component_line);

static int compareFloat(float a, float b) {
	if (fabs(a - b) < EPSILON) {
		return 0;
	} else if (a > b) {
		return 1;
	} else {
		return -1;
	}
}

ComponentTypeLatex typeTable[] = {
	{T_RESISTOR, 	"R",		"l",	"\\Omega"},
	{T_BATTERY, 	"battery",	"l",	"V"},
	{T_INDUCTOR, 	"inductor",	"l",	"H"}, 
	{T_CAPACITOR, 	"C", 		"l",	"F"}, 
	{T_AMMETER, 	"ammeter", 	"i_",	"mA"},
	{T_VOLTMETER, 	"voltmeter","l",	"V"},
	{T_LAMP, 		"lamp",		"l",	"W"},
	{T_GENERIC, 	"generic", 	"l",	""},
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
int reachedComponents = 0;

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

void closeComponent(FILE * filePointer) {
	fprintf(filePointer,"\n\t\\draw %s -- (%d,%d) -- (0,%d) -- (0,%d);", current_coord, x, y-2, y-2, y);
}

void generateComponentList(Node * component_node, FILE * filePointer) {
	if (component_node == NULL) return;

	component_t * start_component = (component_t *)component_node->data;

	fprintf(filePointer,"\n\t\\draw %s", currentPos());

	generateComponent(start_component, filePointer);
	component_t * current_component = start_component->next;

	while (current_component != NULL && current_component != start_component) {
		generateComponent(current_component, filePointer);
		current_component = current_component->next;
	}
	
	fprintf(filePointer,";");

	closeComponent(filePointer);
}


void generateComponent(component_t *component, FILE *filePointer) {
    char nameFormat[MAX_LABEL_SIZE] = "";
	char valueFormat[MAX_LABEL_SIZE] = "";

    char* componentType = getComponent(component->component_type);
    char* goTo = goRight();

	if (compareFloat(component->constant, 0) == 0) {
		// no value assigned
	} else if (compareFloat(component->constant, floor(component->constant)) == 0){
		sprintf(valueFormat, "\\:%d%s", (int)component->constant, getMeasurement(component->component_type));
	} else {
		sprintf(valueFormat, "\\:%.3f%s", component->constant, getMeasurement(component->component_type));
	}

    if (component->showName) {
        sprintf(nameFormat, "%s", component->component_name);
    }

    fprintf(filePointer, "\n\t\tto[%s, l=$%s%s$] %s",
            componentType, nameFormat, valueFormat, goTo);

	reachedComponents++;
}


component_t * visited_components[4];
int visit_count = 0;

// Solo aplica para el caso prev y next son el mismo nodo
int visitComponent(component_t * component) {
	for(int i = 0; i < 4; i++) {
		if (visited_components[i] == NULL) {
			visited_components[i] = component;
			visit_count++;
			return 1;
		} 
		
		if (visited_components[i] == component) {
			return 0;
		}
	}

	return -1;
}

void resetVisits() {
	for(int i = 0; i < 4; i++) {
		visited_components[i] = NULL;
	}
	visit_count = 0;
}

void Generator(int result, symbol_t * symbolTable) {
	LogInfo("La expresion genera un circuito valido. Compilacion terminada con codigo: '%d'.", result);

 	FILE *filePointer; 

    filePointer = fopen("SrQuix.tex", "w");

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

	if (current_node == NULL) {
		generateComponentList(symbolTable->components->head, filePointer);
	}

	while (current_node != NULL) {
		int reg_prev_and_next = 0;
		for (int i = 0; i < 4; i++){
			if (		((node_t *)current_node->data)->dir[i] != NULL &&
						((node_t *)current_node->data)->dir_type[i] == COMPONENT_TYPE && 
						((component_t *)((node_t *)current_node->data)->dir[i])->prev == (node_t *)current_node->data &&
						visitComponent((component_t *)((node_t *)current_node->data)->dir[i])) 
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

				if (visit_count == 1) {
					closeComponent(filePointer);
					y -= 2;
				}

				x = 0;
				y -= 2;
				current_line++;
				
			} else if (((node_t *)current_node->data)->dir[i] != NULL && ((node_t *)current_node->data)->dir_type[i] == NODE_TYPE) {

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
		resetVisits();
	}

	connectNodes(circuit_lines, current_line, filePointer);

	fprintf(filePointer,"\n\\end{circuitikz}\n");

	if (getListSize(symbolTable->components) != reachedComponents) fprintf(filePointer, "\n\tRuntime error, could not reach all components"); 

	fprintf(filePointer, "\n\\end{document}");

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