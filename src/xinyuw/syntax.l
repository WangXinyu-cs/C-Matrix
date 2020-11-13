%option noyywrap nodefault
%option outfile="syntax.lex.c"

%{
#include <cstring>
#include "dstruct.h"
#include "syntax.tab.h"
%}

EXP     ([Ee][-+]?[0-9]+)

%%

"+" |
"-" |
"*" |
"/" |
"\\" |
"=" |
";" |
"," |
"[" |
"]" |
"<" |
">" |
"(" |
")"     { return yytext[0]; }

quit    { return QUIT; }

zeros |
ones  |
eye   |
rand  |
diag  |
blkdiag   |
cat   |
inv   |
transpose |
det         { yylval.name = strdup(yytext); return FUNCTION; }

[a-zA-Z][a-zA-Z0-9]*    { yylval.name = strdup(yytext); return VARNAME; }

[0-9]+  { yylval.number = strdup(yytext); return INTEGER; }

[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}?        { yylval.number = strdup(yytext); return FLOAT; }

'(\\'|[^'])*'   { yylval.string = strndup(yytext + 1, yyleng - 2); return STRING; }

[ \t]        /* ignore whitespace */

\n      { return EOL; }

.       { return yytext[0]; }

<<EOF>> { return YYEOF; }

