#include "backend/code-generation/generator.h"
#include "backend/support/logger.h"
#include "backend/support/shared.h"
#include "frontend/syntactic-analysis/bison-parser.h"
#include "backend/semantic-analysis/symbol-table.h"
#include <stdio.h>

// Estado de la aplicación.
CompilerState state;

// Punto de entrada principal del compilador.
const int main(const int argumentCount, const char ** arguments) {
	// Inicializar estado de la aplicación.
	state.program = NULL;
	state.result = 0;
	state.succeed = false;

	// Mostrar parámetros recibidos por consola.
	for (int i = 0; i < argumentCount; ++i) {
		LogInfo("Argumento %d: '%s'", i, arguments[i]);
	}

	state.symbolTable = symbolTableInit();

	// Compilar el programa de entrada.
	LogInfo("Compilando...\n");
	int result = yyparse();

	int preResult = result;

	if (checkUnlinked() != 0 || hasErrors()) {
		state.succeed = false;
	}

	switch (preResult) {
		case 0:
			// La variable "succeed" es la que setea Bison al identificar el símbolo
			// inicial de la gramática satisfactoriamente.
			if (state.succeed) {
				LogInfo("La compilacion fue exitosa.");
				Generator(state.result, state.symbolTable);
			}
			else {
				LogError("Se produjo un error en la aplicacion.");
				printErrors();
				printUnlinked();
				result = -1;
				break;
			}
			break;
		case 1:
			LogError("Bison finalizo debido a un error de sintaxis.");
			break;
		case 2:
			LogError("Bison finalizo abruptamente debido a que ya no hay memoria disponible.");
			break;
		default:
			LogError("Error desconocido mientras se ejecutaba el analizador Bison (codigo %d).", result);
	}

	symbolTableFree();

	LogInfo("Fin.");
	return result;
}
