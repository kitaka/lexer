#ifndef __LEXER_HEADER__
#define __LEXER_HEADER__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "token_types.h"
#include "debug.h"

struct token {
  	int type;
	int data_type;
	union {
	  	int integer;
		char * string;
		double doubleval;
		char character;
	};
};

struct lexer {
  	char *code;
	struct token **tokens;
	int token_count;
};

struct lexer *lexer_init();
struct lexer *lexer_parse(char *code);
void lexer_free(struct lexer *lexer);
void lexer_analyze(struct lexer *lexer);
void lexer_print_tokens(struct lexer *lexer);

struct token *token_init();
void token_free(struct token *token);
void token_debug(struct token *token);
int is_functional_token(struct token *token);

#endif
