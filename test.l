%{
#include "functions.h"
#include "test.tab.h"
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
%}

%option noyywrap

%%
"\\begin{algo}" {return BEG;}
"\\end{algo}"   {return END;}
"\\SET"         {return SET;}
"\\INCR"        {return INCR;}
"\\DECR"        {return DECR;}

[0-9]+ {yylval.entier=atoi(yytext);return NUM;}

[a-zA-Z]+ {yylval.idf=strdup(yytext);return IDF;}

"," {return VIRGULE;}

"{" {return OPEN_ACCO;}

"}" {return CLOSE_ACCO;}

"+" {return ADD;}
. {printf("non\n");}
%%