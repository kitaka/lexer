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


struct ast *ast_parse(struct lexer *lexer)
{
  	struct ast *ast, *current_node;
	current_node = ast = ast_init();
	if (ast == NULL) return NULL;

	int i;
	for (i = 0; i < lexer->token_count; i++) {

		if (is_functional_token(lexer->tokens[i])) {
			current_node->token = lexer->tokens[i];

			current_node->left_node = ast_init();
			current_node->left_node->token = lexer->tokens[i - 1];
			current_node->left_node->parent = current_node;

			current_node->right_node = ast_init();
			current_node->right_node->token = lexer->tokens[i+1];
			current_node->right_node->parent = current_node;

			current_node = current_node->right_node;
		}
	}

	return ast;
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
