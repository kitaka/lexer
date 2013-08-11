#include "parser.h"

struct ast *ast_init()
{
	struct ast *ast = calloc(1, sizeof(struct ast));

	if (ast == NULL) {
	  	perror("Failed to allocate memory for AST");
		return ;
	}

	ast->parent = NULL;
	ast->left_node = NULL;
	ast->right_node = NULL;

	return ast;
}

struct parser *parser_init()
{
	struct parser *parser = calloc(1, sizeof(struct parser));

	if (parser == NULL) {
	  	perror("Failed to allocate memory for Parser");
		return ;
	}
	
	parser->use_working_node = 0;

	return parser;
}

void parser_free(struct parser *parser)
{
  	ast_free(parser->main_node);
	free(parser);
}


/*
 * Finds the previous node. Sets it to a hanging node if previous token was a closing braket
 */
struct ast *get_left_node(struct parser *parser, struct ast *current_node, int *idx)
{
  	int prev_idx = (*idx) - 1;	
	struct token *token = parser->lexer->tokens[prev_idx];

	if (token->type == BRACKET_CLOSE_TOKEN) {
	  	return parser->hanging_node;
	}

	struct ast *node = ast_init();
	node->token = token;
	node->parent = current_node;

	return node;
}

struct ast *get_right_node(struct parser *parser, struct ast *current_node, int *idx)
{
  	int next_idx = (*idx) + 1;	
	struct token *token = parser->lexer->tokens[next_idx];

	if (token->type == BRACKET_OPEN_TOKEN) {
	  	parser->working_node = ast_init();
		parser->use_working_node = 1;
	  	return parser->working_node;
	}

	struct ast *node = ast_init();
	node->token = token;
	node->parent = current_node;

	return node;
}

void parse_token(struct parser *parser, struct ast **current_node, int *idx)
{
  	/* this should hold the value of the sign like +, -, =, * etc */
	(*current_node)->token = parser->lexer->tokens[*idx];
	

	(*current_node)->left_node = get_left_node(parser, (*current_node), idx);	

	//(*current_node)->right_node = get_right_node(parser, (*current_node), idx);
	(*current_node)->right_node = ast_init();
	(*current_node)->right_node->token = parser->lexer->tokens[(*idx) + 1];
	(*current_node)->right_node->parent = (*current_node);

	(*current_node) = (*current_node)->right_node;
}

void ast_parse(struct parser *parser, int *idx)
{
	if (parser->lexer->tokens[*idx]->type == BRACKET_OPEN_TOKEN) {
		parser->current_hanging_node = parser->hanging_node = (parser->use_working_node == 1) ? parser->working_node : ast_init();
		  	
		while (parser->lexer->tokens[*idx]->type != BRACKET_CLOSE_TOKEN) {
			if (is_functional_token(parser->lexer->tokens[*idx])) {
				//parse_token(parser, &parser->current_hanging_node, idx);		
				parse_token(parser, &parser->current_node, idx);		
			}
			++(*idx);
		}
		
		// restore current_node to the current_hanging_node
		if (parser->use_working_node == 10) {
		  	parser->current_node = parser->current_hanging_node;
			parser->use_working_node = 0;
		}
	}
	else if (is_functional_token(parser->lexer->tokens[*idx])) {
	  	parse_token(parser, &parser->current_node, idx);
	}
}

struct parser *parser_parse(struct lexer *lexer)
{
  	struct parser *parser = parser_init();
	if (parser == NULL) return NULL;
	parser->lexer = lexer;
	
	parser->current_node = parser->main_node = ast_init();
	if (parser->main_node == NULL) return NULL;

	int i;
	for (i = 0; i < lexer->token_count; i++) {
		ast_parse(parser, &i);
	}

	return parser;
}

void ast_free(struct ast *ast)
{
  	if (ast == NULL) return;

	ast_free(ast->left_node);
	ast_free(ast->right_node);
}

int ast_has_parent(struct ast *ast)
{
  	return ast->parent != NULL;
}

void ast_debug(struct ast *ast)
{
  	if (ast == NULL) return;

	if (ast_has_parent(ast) && ast == ast->parent->left_node) printf("LEFT:");
	else if (ast_has_parent(ast) && ast == ast->parent->right_node) printf("RIGHT:");

	token_debug(ast->token);
	
	ast_debug(ast->left_node);
	ast_debug(ast->right_node);
}
