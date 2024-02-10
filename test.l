%{
#include "test.tab.h"
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
%}

%option noyywrap

%%
"\\begin{algo}" {return BEG;}
[0-9]+ {yylval.entier=atoi(yytext);return NUM;}

[a-zA-Z]+ {yylval.idf=strdup(yytext);return IDF;}

"," {return VIRGULE;}

"{" {return OPEN_ACCO;}

"}" {return CLOSE_ACCO;}

. {printf("non\n");}
%%