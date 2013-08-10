#include "interpreter.h"

struct interpreter *interpreter_init()
{
  	struct interpreter *interpreter = calloc(1, sizeof(struct interpreter));

	if (interpreter == NULL) {
		perror("Could not allocate memory for interpreter");
		return NULL;
	}

	return interpreter;
}

struct interpreter *interpreter_interprete(struct ast *ast)
{
  	if (ast == NULL) return NULL;

  	struct interpreter *interpreter = interpreter_init();
	interpreter->symbols = symbol_table_init();
	interpreter->ast = ast;

	interprete_node(interpreter, interpreter->ast);

	return interpreter;

}
void interpreter_free(struct interpreter *interpreter)
{
  	if (interpreter == NULL) return ;

	free(interpreter);
}

int interprete_node(struct interpreter *interpreter, struct ast *ast)
{
  	if (ast->token->type == ASSIGNMENT_OP_TOKEN) {
	  	struct key_val *keyval = key_val_init();
		
		/* assign to the value in the left node */
		keyval->key = ast->left_node->token->string;
		keyval->integer_val = interprete_node(interpreter, ast->right_node);
		
		symbol_table_insert(interpreter->symbols, keyval);
	}
	else if (ast->token->type == ADDITION_ARITHMETIC_TOKEN) {
		return interprete_node(interpreter, ast->left_node) + interprete_node(interpreter, ast->right_node);
	}
	else if (ast->token->type == SUBTRACTION_ARITHMETIC_TOKEN) {
		return interprete_node(interpreter, ast->left_node) - interprete_node(interpreter, ast->right_node);
	}
	else if ((ast->right_node == NULL) && (ast->left_node == NULL)) {
	  	return ast->token->integer;
	}

}
