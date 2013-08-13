#ifndef __PARSER_HEADER__
#define __PARSER_HEADER__

#include <stdio.h>
#include "lexer.h"


struct ast {
  	struct token *token;
	struct ast *parent;
	struct ast *left_node;
	struct ast *right_node;
};

struct parser {
  	struct lexer *lexer;
	struct ast *tree;
};


struct ast *ast_init();
struct parser *parser_init();
void parser_free(struct parser *parser);
void ast_free(struct ast *ast);
void ast_debug(struct ast *ast);
//struct ast *ast_parse(struct lexer *lexer);
struct ast *parser_parse_tokens(struct token **, int);
struct parser *parser_parse(struct lexer *lexer);

#endif

