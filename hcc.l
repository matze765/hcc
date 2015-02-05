%{

#include "compiler.h"
#include "compiler.tab.h"
#include <string.h>
int lineNumber = 1;

%}

%option nounput
INTEGER ([1-9][0-9]*|0)
FLOAT ({INTEGER}\.[0-9]*)
ID ([a-zA-Z][a-zA-Z0-9_]*)

%%
\/\/.*[\n] lineNumber++;
\/\*.*\*\/   
if return IF;
else return ELSE;
return return RETURN;
while return WHILE;
do return DO;
const return CONST;
void return VOID;
\+\+ return PLUSPLUS;
-- return MINUSMINUS;
!= return NOTEQUAL;
>> return SHIFTR;
\<\< return SHIFTL;
\<= return LESSEQUAL;
\>= return GREATEREQUAL;
\> return GREATER;
\< return LESS;
\|\| return ORL;
&& return ANDL;
== return EQUALL;
!  return NOTL;
\+  return PLUS;
- return MINUS;
\* return TIMES;
\/ return DIV;
\% return MOD;
\( return OPENBR;
\) return CLOSEBR;
\{ return COPENBR;
\} return CCLOSEBR;
, return COMMA; 
= return EQUAL;
; return SEMICOLON;
int return INTT;
float return FLOATT;
{FLOAT} {yylval.fval= atof(yytext);return FLOAT;}
{INTEGER} {yylval.ival=atoi(yytext); return INT;}
{ID} {yylval.sval = strdup(yytext); return identifier;};
\n ++lineNumber;
[ \t] 
. {char* a= malloc(8*1000); sprintf(a,"UNEXPECTED CHARACTED %s Explected Identifier.",yytext); yyerror(a);};


%%
