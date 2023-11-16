#include "../support/logger.h"
#include "generator.h"

/**
 * Implementación de "generator.h".
 */

void Generator(int result, symbol_t * symbolTable) {
	LogInfo("La expresion genera un circuito valido. Compilacion terminada con codigo: '%d'.", result);

	printf("1st elem: %s\n", ((component_t *)symbolTable->components->head->next->data)->component_name);
}
