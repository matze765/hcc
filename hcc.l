%{

#include "hcc.h"
#include "hcc.tab.h"
#include <string.h>

int lineNumber=1;

%}

%option nounput

ID ([A-Z][a-zA-Z0-9_]*)
id ([a-z][a-zA-Z0-9_]*)

%%

{ID} {yylval.sval = strdup(yytext); return ID;};
{id} {yylval.sval = strdup(yytext); return id;};  

\. return DOT;
:- return IFTHEN;
\( return OPA;
\) return CPA;
\, return COMMA;
\n ++lineNumber;
[ \t] 
. {char* a= malloc(8*1000); sprintf(a,"Unexpected Character %s",yytext); yyerror(a);};


%%
