#include "executor.h"

struct executor *executor_init()
{
  	struct executor *executor = calloc(1, sizeof(struct executor));

	if (executor == NULL) {
		perror("Failed to allocate memory for executor");
		return NULL;
	}

	return executor;
}

void executor_free(struct executor *executor)
{
  	interpreter_free(executor->interpreter);
  	free(executor);
}

/*
 * The function splits the code around semi colons and parses
 * and then interpretes the block before it goes on to the next.
 *
 * What that means you can access variables from the previous block
 * int the current block since they are all interpreted at 
 * different times
 */
struct executor *executor_execute(struct lexer *lexer)
{
  	struct executor *executor;
	struct parser *parser;
	struct ast *tree;
	struct interpreter *interpreter;
	
	int i;
	int tokens_partition_idx = 0;

	executor = executor_init();
	executor->interpreter = interpreter_init();

	for (i = 0; i < lexer->token_count; i++) {
	  	if (lexer->tokens[i]->type == STATEMENT_END_TOKEN) {

			tree = parser_parse_tokens(&lexer->tokens[tokens_partition_idx], i - tokens_partition_idx);
			
			interprete_node(executor->interpreter, tree);

			tokens_partition_idx = i + 0;
		}
		else if (lexer->tokens[i]->type == IF_KEYWORD_TOKEN) {
		  	// we handle the if statement here
			struct if_stmt *stmt = (struct if_stmt *) lexer->tokens[i]->pointer;

			// get the result of the condition
			tree = parser_parse_tokens(stmt->condition_lexer->tokens, stmt->condition_lexer->token_count);
			int output = interprete_node(executor->interpreter, tree);

			// based on the execution choose either the if or else block lexer
			struct lexer *block_lexer;
		       	
			block_lexer = output ? stmt->if_block_lexer : stmt->else_block_lexer;
			tree = parser_parse_tokens(block_lexer->tokens, block_lexer->token_count);
			interprete_node(executor->interpreter, tree);
		}

	}

	return executor;
}


