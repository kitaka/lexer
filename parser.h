#ifndef __PARSER_HEADER__
#define __PARSER_HEADER__

#include <stdio.h>
#include "lexer.h"

static struct ast *hanging_node;

struct ast {
  	struct token *token;
	struct ast *parent;
	struct ast *left_node;
	struct ast *right_node;
};

struct parser {
  	struct lexer *lexer;
	struct ast *main_node;
	struct ast *hanging_node;
	struct ast *current_node;
	struct ast *current_hanging_node;

};


struct ast *ast_init();
struct parser *parser_init();
void parser_free(struct parser *parser);
void ast_free(struct ast *ast);
void ast_debug(struct ast *ast);
struct ast *ast_parse(struct lexer *lexer);
struct parser *parser_parse(struct lexer *lexer);

#endif

