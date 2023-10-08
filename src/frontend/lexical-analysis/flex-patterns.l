%{

#include "flex-actions.h"

%}

/* Obliga a crear una regla explícita para manejar lexemas desconocidos. */
%option nodefault

/* Escanea un único archivo y se detiene. */
%option noyywrap

/* Dispone una variable global "yylineno" con el número de línea actual. */
%option yylineno

/* Contextos de Flex. */
%x COMMENT

/* Patrones reutilizables. */
digit			[0-9]
text            [A-Za-z0-9]
endline			[\n]
whitespace		[ \f\n\r\t\v]

%%

"/*"										{ BEGIN(COMMENT); BeginCommentPatternAction(); }
<COMMENT>.									{ IgnoredPatternAction(yytext, yyleng); }
<COMMENT>{endline}+							{ IgnoredPatternAction(yytext, yyleng); }
<COMMENT>"*/"								{ EndCommentPatternAction(); BEGIN(INITIAL); }

"showName"						    		{ return ShowNamePatternAction(); }
"true"						        		{ return TruePatternAction(); }
"false"					        			{ return FalsePatternAction(); }

"resistor"							    	{ return ResistorPatternAction(); }
"battery"									{ return BatteryPatternAction(); }
"inductor"								{ return InductorPatternAction(); }
"capacitor"								{ return CapacitorPatternAction(); }
"ammeter"                                { return AmmeterPatternAction(); }
"voltmeter"                              { return VoltmeterPatternAction(); }
"ohmmeter"                               { return OhmmeterPatternAction(); }

","                                         { return ComaPatternAction(); }

"="                                         { return EqualPatternAction(); }

"node"					        			{ return NodePatternAction(); }

">"											{ return GreaterThanPatternAction(); }
"+"											{ return AdditionOperatorPatternAction(); }

"("											{ return OpenParenthesisPatternAction(yytext); }
")"											{ return CloseParenthesisPatternAction(yytext); }

"["											{ return OpenBracketPatternAction(); }
"]"											{ return CloseBracketPatternAction(); }

";"											{ return SemicolonPatternAction(); }

{digit}+									{ return IntegerPatternAction(yytext, yyleng); }

{text}+	    			                	{ return TextPatternAction(yytext, yyleng); }

{whitespace}+								{ IgnoredPatternAction(yytext, yyleng); }

.											{ return UnknownPatternAction(yytext, yyleng); }

%%
