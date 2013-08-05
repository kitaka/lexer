#include "lexer.h"


struct lexer *lexer_init(char *code)
{
  	struct lexer *lexer = calloc(1, sizeof(struct lexer));

	if (lexer == NULL) {
	  	perror("Failed to init lexer");
	}
	
	lexer->token_count = 0;
	lexer->code = strdup(code);
	lexer->tokens = calloc(100, sizeof(struct token *));

	return lexer;
}

void lexer_free(struct lexer *lexer)
{
  	free(lexer->code);
	// foreach token remove
	free(lexer->tokens);
	free(lexer);
}

void lexer_add_token(struct lexer *lexer, struct token *token)
{
	lexer->tokens[lexer->token_count++] = token;
}

void lexer_analyze(struct lexer *lexer)
{
  	int i, j, k;
	char c;
  	ssize_t len = strlen(lexer->code);
	char *code = lexer->code;
	

	for (i = 0; i < len; i++) {
		if (code[i] == '$') {
		  	
			struct token *token = token_init();
		  	
			token->type = VARIABLE_TOKEN;	
			token->string = malloc(255 * sizeof(char));
			
			j = 0;
			while (!isspace(code[++i])) {
				token->string[j++] = code[i];	
			}	
			
			lexer_add_token(lexer, token);
		} 
		else if (code[i] == '=') {
			
		  	struct token *token = token_init();
		
		  	token->type = ASSIGNMENT_OP_TOKEN;
			token->character = '=';
			
			lexer_add_token(lexer, token);
		}
		else if (code[i] == ';') {
			
		  	struct token *token = token_init();
			
			token->type = STATEMENT_END_TOKEN;
		       	token->character = ';';	
			
			lexer_add_token(lexer, token);
		}
		else if (isdigit(code[i])) {
	  		  
			struct token *token = token_init();
		  	
			token->type = INTEGER_TOKEN;
			token->integer = 0;

			--i;
			while (isdigit(code[++i])) {
			  	k = code[i] - '0';
			  	token->integer = (token->integer * 10) + k;
			}

			lexer_add_token(lexer, token);
		}
	}
}

void lexer_print_tokens(struct lexer *lexer)
{
  	int i;
  	for (i = 0; i < lexer->token_count; i++) {
	  	switch (lexer->tokens[i]->type) {
		case VARIABLE_TOKEN:
			sdebug(lexer->tokens[i]->string);
			break;
		case INTEGER_TOKEN:
			idebug(lexer->tokens[i]->integer);
			break;
		case ASSIGNMENT_OP_TOKEN:
			cdebug(lexer->tokens[i]->character);
			break;
		case STATEMENT_END_TOKEN:
			cdebug(lexer->tokens[i]->character);
			break;
		default:
			break;
		} 
	}
}

struct token *token_init()
{
  	struct token *token = calloc(1, sizeof(struct token));

	if (token == NULL) {
		perror("Failed to allocate memory for token");
	}

	return token;
}

void token_free(struct token *token)
{
  	if (token->string != NULL) {
	  	free(token->string);
	}

	free(token);
}

