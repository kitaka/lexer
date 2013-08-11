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
	
	return parser;
}

void parser_free(struct parser *parser)
{
  	ast_free(parser->tree);
	free(parser);
}

int find_pos_closing_bracket(struct token **tokens, int len)
{
  	int i, brackets = 0;

  	for (i = 0; i < len; i++) {
      		
	  	if (tokens[i]->type == BRACKET_OPEN_TOKEN) {
			++brackets;
		}		  
		else if (tokens[i]->type == BRACKET_CLOSE_TOKEN) {
		  	--brackets;

	  		if (brackets == 0) 
			  	return i;
		}
	}

	return -1;	
}

int is_token_semi_colon(struct token *token)
{
  	return token->type == STATEMENT_END_TOKEN;
}

int is_token_closing_bracket(struct token *token)
{
  	return token->type == BRACKET_CLOSE_TOKEN;
}

int is_token_integer(struct token *token) 
{
  	return token->type == INTEGER_TOKEN;
}

int is_token_variable(struct token *token) 
{
  	return token->type == VARIABLE_TOKEN;
}

void parse_functional_token(struct token *token, struct ast **waiting_left_node, struct ast **active_node)
{
	struct ast *node = ast_init();
	
	node->token = token;
	node->parent = *active_node;
	node->left_node = *waiting_left_node;
	(*waiting_left_node)->parent = node;
	(*active_node)->right_node = node;
	(*active_node) = node;
}

void connect_or_hang_node(struct ast *node, struct ast **waiting_left_node, struct ast **active_node, struct token *next_token)
{
	if (is_token_semi_colon(next_token) || is_token_closing_bracket(next_token)) {
	 	node->parent = *active_node;
	 	(*active_node)->right_node = node;
	} else {
	  	*waiting_left_node = node;
	}	  
}

struct ast *parse_tokens(struct parser *parser, struct token **tokens, int count)
{
  	struct ast *node, *waiting_left_node, *root_node, *active_node;
  	int idx, last_idx;

	last_idx = count - 1;
	node = NULL;

	active_node = root_node = ast_init();

	for (idx = 0; idx < count; idx++) {
		
		if (tokens[idx]->type == BRACKET_OPEN_TOKEN) {
		  	int pos = find_pos_closing_bracket(&tokens[idx], count - idx);
			struct ast *subtree = parse_tokens(parser, &tokens[idx + 1], pos - 1);

			idx = idx + pos;

			connect_or_hang_node(subtree->right_node, &waiting_left_node, &active_node, tokens[idx + 1]);
		}	  
		else if (is_token_integer(tokens[idx]) || is_token_variable(tokens[idx])) {
		  	
		  	node = ast_init();
			node->token = tokens[idx];
			node->right_node = NULL;
			node->left_node = NULL;

			connect_or_hang_node(node, &waiting_left_node, &active_node, tokens[idx + 1]);
		}
		else if (is_functional_token(tokens[idx])) {
		  	parse_functional_token(tokens[idx], &waiting_left_node, &active_node);
		}

	}	  

	return root_node;
}

struct parser *parser_parse(struct lexer *lexer)
{
  	struct parser *parser = parser_init();
	
	if (parser == NULL) return NULL;
	
	parser->lexer = lexer;
	parser->tree = parse_tokens(parser, parser->lexer->tokens, parser->lexer->token_count);

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
