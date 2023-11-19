#include "../../backend/support/logger.h"
#include "flex-actions.h"
#include <stdlib.h>
#include <string.h>

/**
 * Implementación de "flex-actions.h".
 *
 * Cada función debe realizar 2 operaciones básicas para lograr el streaming
 * de tokens hacia Bison:
 *
 *	1) Computar los atributos del token y almacenarlos donde sea conveniente
 *		(en la tabla de símbolos, en "yylval", o en ambos).
 *	2) Retornar el token que identifica el terminal identificado.
 *
 * Bison utilizará el token retornado en el punto (2) para matchear dicho
 * terminal en la gramática. Por otro lado, el valor almacenado en "yylval" es
 * el que Bison proveerá como valor semántico al realizar una reducción
 * (mediante $1, $2, $3, etc.).
 */

char * copyLexeme(const char * lexeme, const int length) {
	char * lexemeCopy = (char *) calloc(length + 1, sizeof(char));
	strncpy(lexemeCopy, lexeme, length);
	return lexemeCopy;
}

void BeginCommentPatternAction() {
	LogDebug("[Flex] [COMMENT] BeginCommentPatternAction............................");
}

void EndCommentPatternAction() {
	LogDebug("[Flex] [COMMENT] EndCommentPatternAction..............................");
}

token ResistorPatternAction() {
	LogDebug("[Flex] ResistorPatternAction: 'resistor'.");
	yylval.token = RESISTOR;
	return RESISTOR;
}

token BatteryPatternAction() {
	LogDebug("[Flex] BatteryPatternAction: 'battery'.");
	yylval.token = BATTERY;
	return BATTERY;
}

token InductorPatternAction() {
	LogDebug("[Flex] InductorPatternAction: 'inductor'.");
	yylval.token = INDUCTOR;
	return INDUCTOR;
}

token CapacitorPatternAction() {
	LogDebug("[Flex] CapacitorcePatternAction: 'capacitor'.");
	yylval.token = CAPACITOR;
	return CAPACITOR;
}

token ComaPatternAction() {
	LogDebug("[Flex] ComaTypePatternAction: ','.");
	yylval.token = COMA;
	return COMA;
}

token EqualPatternAction() {
	LogDebug("[Flex] EqualTypePatternAction: '='.");
	yylval.token = EQUAL;
	return EQUAL;
}

token OpenBracketPatternAction() {
	LogDebug("[Flex] OpenBracketPatternAction: '['.");
	yylval.token = OPEN_BRACKET;
	return OPEN_BRACKET;
}

token CloseBracketPatternAction() {
	LogDebug("[Flex] CloseBracketPatternAction: ']'.");
	yylval.token = CLOSE_BRACKET;
	return CLOSE_BRACKET;
}

token NodePatternAction() {
	LogDebug("[Flex] NodePatternAction: 'node'.");
	yylval.token = NODE;
	return NODE;
}

token SemicolonPatternAction() {
	LogDebug("[Flex] SemicolonPatternAction: ';'.");
	yylval.token = SEMICOLON;
	return SEMICOLON;
}

token GreaterThanPatternAction() {
	LogDebug("[Flex] GreaterThanPatternAction: '>'.");
	yylval.token = GREATER_THAN;
	return GREATER_THAN;
}

token AdditionOperatorPatternAction() {
	LogDebug("[Flex] AdditionOperatorPatternAction: '+'.");
	yylval.token = ADD;
	return ADD;
}

token ShowNamePatternAction() {
	LogDebug("[Flex] ShowNamePatternAction: 'showName'.");
	yylval.token = SHOW_NAME;
	return SHOW_NAME;
}

token TruePatternAction() {
	LogDebug("[Flex] TruePatternAction: 'true'.");
	yylval.token = TRUE;
	return TRUE;
}

token FalsePatternAction() {
	LogDebug("[Flex] FalsePatternAction: 'false'.");
	yylval.token = FALSE;
	return FALSE;
}

token CloseParenthesisPatternAction() {
	LogDebug("[Flex] CloseParenthesisPatternAction: ')'.");
	yylval.token = CLOSE_PARENTHESIS;
	return CLOSE_PARENTHESIS;
}

token IntegerPatternAction(const char * lexeme, const int length) {
	LogDebug("[Flex] IntegerPatternAction: '%s' (length = %d).", lexeme, length);
	char * lexemeCopy = copyLexeme(lexeme, length);
	yylval.integer = atoi(lexemeCopy);
	free(lexemeCopy);
	return INTEGER;
}

token FloatPatternAction(const char * lexeme, const int length) {
	LogDebug("[Flex] FloatPatternAction: '%s' (length = %d).", lexeme, length);
	char * lexemeCopy = copyLexeme(lexeme, length);
	yylval.real = atof(lexemeCopy);
	free(lexemeCopy);
	return REAL;
}

token TextPatternAction(const char * lexeme, const int length) {
	LogDebug("[Flex] TextPatternAction: '%s' (length = %d).", lexeme, length);
	char * lexemeCopy = copyLexeme(lexeme, length);
	yylval.text = lexemeCopy;
	// free(lexemeCopy);
	return TEXT;
}

token OpenParenthesisPatternAction() {
	LogDebug("[Flex] OpenParenthesisPatternAction: '('.");
	yylval.token = OPEN_PARENTHESIS;
	return OPEN_PARENTHESIS;
}

token AmmeterPatternAction() {
	LogDebug("[Flex] AmmeterPatternAction: 'ammeter'.");
	yylval.token = AMMETER;
	return AMMETER;
}

token VoltmeterPatternAction() {
	LogDebug("[Flex] VoltmeterPatternAction: 'voltmeter'.");
	yylval.token = VOLTMETER;
	return VOLTMETER;
}

token LampPatternAction() {
	LogDebug("[Flex] LampPatternAction: 'lamp'.");
	yylval.token = LAMP;
	return LAMP;
}

token GenericCompPatternAction() {
	LogDebug("[Flex] GenericCompPatternAction: 'generic'.");
	yylval.token = GENERIC_COMP;
	return GENERIC_COMP;
}

token UnknownPatternAction(const char * lexeme, const int length) {
	char * lexemeCopy = copyLexeme(lexeme, length);
	LogDebug("[Flex] UnknownPatternAction: '%s' (length = %d).", lexemeCopy, length);
	free(lexemeCopy);
	yylval.token = ERROR;
	// Al emitir este token, el compilador aborta la ejecución.
	return ERROR;
}

void IgnoredPatternAction(const char * lexeme, const int length) {
	char * lexemeCopy = copyLexeme(lexeme, length);
	LogRaw("[DEBUG] [Flex] IgnoredPatternAction: '");
	LogText(lexemeCopy, length);
	LogRaw("' (length = %d).\n", length);
	free(lexemeCopy);
	// Como no debe hacer nada con el patrón, solo se loguea en consola.
	// No se emite ningún token.
}
