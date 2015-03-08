%{

#include "hcc.h"
#include "hcc.tab.h"
#include <string.h>

int lineNumber=1;

%}

%option nounput

ID ([A-Z][a-zA-Z0-9_]*)
id ([a-z][a-zA-Z0-9_]*)
num ([1-9][0-9]*(\.[0-9]+)?)

%%

{ID} {yylval.sval = strdup(yytext); return ID;};
{id} {yylval.sval = strdup(yytext); return id;};  
{num} {yylval.sval = strdup(yytext); return num;}
\%.*\n {++lineNumber; }   
\. return DOT;
:- return IFTHEN;
\( return OPA;
\) return CPA;
\[ return OBR;
\] return CBR;
\| return PIPE;
\, return COMMA;
> return GT;
>= return GEQ;
\<  return LT;
\<= return LEQ;
= return U;
\\= return NU;
== return EQ;
\\== return NEQ;
\+ return PLUS;
- return MINUS;
\* return TIMES;
\/ return DIV;


\n ++lineNumber;
[ \t] 
. {char* a= malloc(8*1000); sprintf(a,"Unexpected Character %s",yytext); yyerror(a);};


%%
