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

"," {return VIRGULE;}

"{" {return OPEN_ACCO;}

"}" {return CLOSE_ACCO;}

"(" {return OPEN_PARENT;}

")" {return CLOSE_PARENT;}

"+" {return ADD;}
"-" {return SUB;}
"*" {return MULT;}
"/" {return DIV;}

"!=" {return DIF;}
"&&" {return AND;}
"==" {return EGAL;}
"||" {return OR;}
"!"  {return NOT;}

"true" {return TRUE;}
"false" {return FALSE;}

[0-9]+ {yylval.entier=atoi(yytext);return NUM;}

[a-zA-Z]+ {yylval.idf=strdup(yytext);return IDF;}

. {printf("erreur de token,%s non reconnu\n",yytext);}
%%