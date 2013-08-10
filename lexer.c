#include "lexer.h"


struct lexer *lexer_init()
{
  	struct lexer *lexer = calloc(1, sizeof(struct lexer));

	if (lexer == NULL) {
	  	perror("Failed to init lexer");
		return;
	}
	
	lexer->token_count = 0;
	lexer->tokens = calloc(100, sizeof(struct token *));
	
	return lexer;
}

struct lexer *lexer_parse(char *code)
{
  	struct lexer *lexer = lexer_init();

	if (lexer == NULL) return NULL;
	
	lexer->code = strdup(code);

	return lexer;	
}

void lexer_free(struct lexer *lexer)
{
  	free(lexer->code);
	
	int i;
	for (i = 0; i < lexer->token_count; i++) {
		token_free(lexer->tokens[i]);
	}
	
	free(lexer->tokens);
	free(lexer);
}

struct token *token_init()
{
  	struct token *token = calloc(1, sizeof(struct token));

	if (token == NULL) {
		perror("Failed to allocate memory for token");
		return;
	}

	token->data_type = 0;

	return token;
}

void token_free(struct token *token)
{
  	if (token == NULL) return;

  	if ((token->data_type == STRING_TYPE) &&  (token->string != NULL)) {
	  	free(token->string);
	}

	free(token);
}

/*
 * The name might need to be changed
 */
int is_functional_token(struct token *token)
{
  	return token->type == ASSIGNMENT_OP_TOKEN || token->type == ADDITION_ARITHMETIC_TOKEN;
}

void lexer_add_token(struct lexer *lexer, struct token *token)
{
	lexer->tokens[lexer->token_count++] = token;
}

void parse_variable(struct lexer *lexer, int *idx)
{
	struct token *token = token_init();
		  	
	token->type = VARIABLE_TOKEN;	
	token->string = malloc(255 * sizeof(char));
			
	int j = 0;
	while (!isspace(lexer->code[++(*idx)]))
		token->string[j++] = lexer->code[*idx];		
			
	lexer_add_token(lexer, token);
}

void parse_digit(struct lexer *lexer, int *idx)
{
  	int k;
	struct token *token = token_init();
		  	
	token->type = INTEGER_TOKEN;
	token->integer = 0;

	--(*idx);
	while (isdigit(lexer->code[++(*idx)])) {
		k = lexer->code[*idx] - '0';
		token->integer = (token->integer * 10) + k;
	}

	--(*idx);

	lexer_add_token(lexer, token);
}

void parse_character(struct lexer *lexer, int *idx, int type)
{
	struct token *token = token_init();
		
	token->type = type;
	token->character = lexer->code[*idx];
			
	lexer_add_token(lexer, token);
}

void lexer_analyze(struct lexer *lexer)
{
  	int i;
	ssize_t len;
	
	len = strlen(lexer->code);
	
	for (i = 0; i < len; i++) {
		if (lexer->code[i] == '$') parse_variable(lexer, &i);  	
		else if (lexer->code[i] == '=') parse_character(lexer, &i, ASSIGNMENT_OP_TOKEN);	
		else if (lexer->code[i] == ';') parse_character(lexer, &i, STATEMENT_END_TOKEN);
		else if (lexer->code[i] == '+') parse_character(lexer, &i, ADDITION_ARITHMETIC_TOKEN);
		else if (isdigit(lexer->code[i])) parse_digit(lexer, &i);  		  
	}
}

void token_debug(struct token *token)
{
  	if (token == NULL) return;

	switch (token->type) {
	case VARIABLE_TOKEN:
		sdebug(token->string);
		break;
	case INTEGER_TOKEN:
		idebug(token->integer);
		break;
	case ASSIGNMENT_OP_TOKEN:
		cdebug(token->character);
		break;
	case STATEMENT_END_TOKEN:
		cdebug(token->character);
		break;
	case ADDITION_ARITHMETIC_TOKEN:
		cdebug(token->character);
		break;
	default:
		sdebug("unknown");
		break;
	} 
}

void lexer_print_tokens(struct lexer *lexer)
{
  	int i;

  	for (i = 0; i < lexer->token_count; i++)
		token_debug(lexer->tokens[i]);
}


