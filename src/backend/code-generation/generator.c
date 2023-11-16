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


void generateComponent(component_t * component) {
	if (component->constant == 0) {
		printf("\n\t\tto[%s, l=$%s$] %s", getComponent(component->component_type), component->component_name, goRight());
	} else {
		printf("\n\t\tto[%s, l=$%d\\:%s$] %s", getComponent(component->component_type), component->constant, getMeasurement(component->component_type), goRight());
	}
}

void semicolonEnter() {
	printf(";\n");
}

void closeComponent() {
	printf("\t\\draw %s -- (%d,%d) -- (%d,%d) -- (0,0);\n", current_coord, x, y-2, 0, y-2);
}

void Generator(int result, symbol_t * symbolTable) {
	LogInfo("La expresion genera un circuito valido. Compilacion terminada con codigo: '%d'.", result);

	current_coord = (char *)malloc(20 * sizeof(char));

	printf("\\begin{circuitikz}");

	Node * current_node = (Node *)symbolTable->nodes->head;

	while (current_node != NULL) {
		for (int i = 0; i < 4; i++){
			if (		((node_t *)current_node->data)->dir[i] != NULL &&
						((node_t *)current_node->data)->dir_type[i] == COMPONENT_TYPE && 
						((component_t *)((node_t *)current_node->data)->dir[i])->next_type == COMPONENT_TYPE) 
				{
				printf("\n\t\\draw %s", currentPos());

				component_t * current_component = (component_t *)((node_t *)current_node->data)->dir[i];
				int finished = 0;
				while (!finished) {
					generateComponent(current_component);

					if (current_component->next_type == NODE_TYPE) {
						finished = 1;
					}

					current_component = (component_t *)current_component->next;
				}

				semicolonEnter();
				x = 0;
				y -= 2;
			}
		}

		current_node = current_node->next;
	}

	// closeComponent();

	printf("\\end{circuitikz}\n");

	free(current_coord);
}