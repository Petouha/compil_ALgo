SHELL=/bin/sh
LEX=flex
YACC=bison
CC=gcc
CFLAGS=-g -std=c11 -pedantic -Wall -D_POSIX_C_SOURCE=200809L
INC_DIR = ./src
SRC_DIR = ./src
LDFLAGS=
# --nounput: ne g�n�re pas la fonction yyunput() inutile
# --DYY_NO_INPUT: ne prend pas en compte la fonction input() inutile
# -D_POSIX_SOURCE: d�clare la fonction fileno()
LEXOPTS= -D_POSIX_C_SOURCE=200809L -DYY_NO_INPUT --nounput
YACCOPTS=

# REMPLACER ICI "fichier" PAR LE NOM DE VOS FICHIERS
PROG=test


vpath %.h $(INC_DIR)
vpath %.c $(SRC_DIR)


$(PROG): lex.yy.o $(PROG).tab.o functions.o
	$(CC) $+ -o $@ $(LDFLAGS) 

lex.yy.c: $(PROG).l $(PROG).tab.h
	$(LEX) $(LEXOPTS) $<

lex.yy.h: $(PROG).l
	$(LEX) $(LEXOPTS) --header-file=$@ $<

$(PROG).tab.c $(PROG).tab.h: $(PROG).y lex.yy.h
	$(YACC) $(YACCOPTS) $< -d -v --graph

%.o: %.c
	$(CC) -DYYDEBUG $(CFLAGS) $< -c

clean:
	-rm $(PROG) *.o lex.yy.* $(PROG).tab.* *.err *.log *.output *.out *.dot
