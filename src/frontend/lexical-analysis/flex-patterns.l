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
character		[A-Za-z]
endline			[\n]
whitespace		[ \f\n\r\t\v]

%%

"/*"										{ BEGIN(COMMENT); BeginCommentPatternAction(); }
<COMMENT>.									{ IgnoredPatternAction(yytext, yyleng); }
<COMMENT>{endline}+							{ IgnoredPatternAction(yytext, yyleng); }
<COMMENT>"*/"								{ EndCommentPatternAction(); BEGIN(INITIAL); }

"resistance"								{ return ResistancePatternAction(); }
"source"									{ return SourcePatternAction(); }
"inductance"								{ return InductancePatternAction(); }
"capacitance"								{ return CapacitancePatternAction(); }

","                                         { return ComaPatternAction(); }

"="                                         { return EqualPatternAction(); }

"node"					        			{ return NodePatternAction(); }

">"											{ return GreaterThanPatternAction(); }
"+"											{ return AdditionOperatorPatternAction(); }

"-"											{ return SubtractionOperatorPatternAction(yytext); }
"*"											{ return MultiplicationOperatorPatternAction(yytext); }
"/"											{ return DivisionOperatorPatternAction(yytext); }

"("											{ return OpenParenthesisPatternAction(yytext); }
")"											{ return CloseParenthesisPatternAction(yytext); }

"["											{ return OpenBracketPatternAction(); }
"]"											{ return CloseBracketPatternAction(); }

";"											{ return SemicolonPatternAction(); }

{digit}+									{ return IntegerPatternAction(yytext, yyleng); }

{character}+						    	{ return TextPatternAction(yytext, yyleng); }

{whitespace}+								{ IgnoredPatternAction(yytext, yyleng); }

.											{ return UnknownPatternAction(yytext, yyleng); }

%%
