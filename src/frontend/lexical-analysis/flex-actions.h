#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../backend/support/shared.h"
#include "../syntactic-analysis/bison-parser.h"

/**
 * Se definen las acciones a ejecutar sobre cada patrón hallado mediante el
 * analizador léxico Flex. Este analizador solo puede identificar
 * construcciones regulares, ya que utiliza un autómata finito determinístico
 * (a.k.a. DFA), como mecanismo de escaneo y reconocimiento.
 *
 * Debería existir una acción por cada patrón disponible en Flex (es decir,
 * por cada regla).
 *
 * Solo aquellas acciones que tengan interés en conocer el largo del lexema
 * emplean el parámetro adicional "length". Esto es útil cuando se desea
 * asignar memoria dinámica, y por lo tanto, se debe saber exactamente el
 * largo de aquello que se va a almacenar.
 */

// Bloque de comentarios multilínea.
void BeginCommentPatternAction();
void EndCommentPatternAction();

// Patrones terminales del lenguaje diseñado.
token ResistorPatternAction();
token BatteryPatternAction();
token InductorPatternAction();
token CapacitorPatternAction();
token AmmeterPatternAction();
token VoltmeterPatternAction();
token OhmmeterPatternAction();
token SinglePhaseVolPatternAction();

token ComaPatternAction();
token EqualPatternAction();

token OpenBracketPatternAction();
token CloseBracketPatternAction();

token SemicolonPatternAction();

token NodePatternAction();

token GreaterThanPatternAction();
token AdditionOperatorPatternAction();

token ShowNamePatternAction();
token TruePatternAction();
token FalsePatternAction();

token TextPatternAction(const char * lexeme, const int length);
token IntegerPatternAction(const char * lexeme, const int length);

token CloseParenthesisPatternAction();
token OpenParenthesisPatternAction();

// Patrón desconocido, permite abortar debido a un error de sintaxis.
token UnknownPatternAction(const char * lexeme, const int length);

// Regla que no hace nada, permite ignorar parte de la entrada.
void IgnoredPatternAction(const char * lexeme, const int length);

#endif
