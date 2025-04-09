SHELL=/bin/sh
LEX=flex
YACC=bison
CC=gcc
CFLAGS=-g -std=c11 -pedantic -Wall -D_POSIX_C_SOURCE=200809L

# Répertoires du projet
SRC_DIR = ./src
INC_DIR = ./include
PARSER_DIR = ./parser
BUILD_DIR = ./build

# Options
LDFLAGS=
LEXOPTS= -D_POSIX_C_SOURCE=200809L -DYY_NO_INPUT --nounput
YACCOPTS= -Wcounterexamples

# Nom du programme final
PROG=compiler

# Chemins de recherche
vpath %.h $(INC_DIR)
vpath %.c $(SRC_DIR)
vpath %.l $(PARSER_DIR)
vpath %.y $(PARSER_DIR)

# Règle par défaut
all: directories $(BUILD_DIR)/$(PROG)

# Création des répertoires s'ils n'existent pas
directories:
	mkdir -p $(BUILD_DIR) $(SRC_DIR) $(INC_DIR) $(PARSER_DIR)

# Compilation du programme final
$(BUILD_DIR)/$(PROG): $(BUILD_DIR)/lex.yy.o $(BUILD_DIR)/$(PROG).tab.o $(BUILD_DIR)/functions.o $(BUILD_DIR)/intermediare.o
	$(CC) $^ -o $@ $(LDFLAGS)

# Génération du lexer (flex)
$(BUILD_DIR)/lex.yy.c: $(PARSER_DIR)/$(PROG).l $(BUILD_DIR)/$(PROG).tab.h
	$(LEX) $(LEXOPTS) -o $@ $<

$(BUILD_DIR)/lex.yy.h: $(PARSER_DIR)/$(PROG).l
	$(LEX) $(LEXOPTS) --header-file=$@ $<

# Génération du parser (bison)
$(BUILD_DIR)/$(PROG).tab.c $(BUILD_DIR)/$(PROG).tab.h: $(PARSER_DIR)/$(PROG).y $(BUILD_DIR)/lex.yy.h
	$(YACC) $(YACCOPTS) $< -d -v --graph -o $(BUILD_DIR)/$(PROG).tab.c

# Compilation des sources
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -DYYDEBUG $(CFLAGS) -I$(INC_DIR) -I$(BUILD_DIR) -c $< -o $@

# Règle spéciale pour les fichiers générés
$(BUILD_DIR)/%.o: $(BUILD_DIR)/%.c
	$(CC) -DYYDEBUG $(CFLAGS) -I$(INC_DIR) -I$(BUILD_DIR) -c $< -o $@

# Nettoyage
clean:
	rm -rf $(BUILD_DIR)
	rm -f *.err *.log *.output *.out *.dot *.gv exp*
	rm lex.yy.c

.PHONY: all clean directories