#include <stdio.h>
#include "lexer.h"
#include "parser.h"
#include "symbol_table.h"
#include "interpreter.h"

void test_symbol_table();

int main(void)
{
  	struct lexer *lexer;

	lexer = lexer_parse("$var = (5 - 2) + 1;");
	//lexer = lexer_parse("$var = 5 - (2 + 1);");
	lexer_analyze(lexer);
	//lexer_print_tokens(lexer); return;

	struct parser *parser = parser_parse(lexer);
	ast_debug(parser->main_node); return;

	struct interpreter *interpreter = interpreter_interprete(parser->main_node);	
	struct symbol_table *found = symbol_table_find(interpreter->symbols, "var");
	idebug(found->key_val->integer_val);
	
	interpreter_free(interpreter);	
	parser_free(parser);
	lexer_free(lexer);
	
	//test_symbol_table();

  	return 0;
}

void test_that_computation_returns_number_four(struct lexer *lexer)
{
	//lexer = lexer_parse("$var = 234 + 567 + 4 - 3;");
	lexer = lexer_parse("$var = (5 - 2) + 1;");
	lexer_analyze(lexer);
	//lexer_print_tokens(lexer); return;

	struct parser *parser = parser_parse(lexer);
	//ast_debug(parser->main_node); return;

	struct interpreter *interpreter = interpreter_interprete(parser->main_node);	
	struct symbol_table *found = symbol_table_find(interpreter->symbols, "var");
	idebug(found->key_val->integer_val);
	
	interpreter_free(interpreter);	
	parser_free(parser);
	lexer_free(lexer);
}

void test_symbol_table()
{
	struct symbol_table *symbols = symbol_table_init();
	struct key_val *keyval = key_val_init();
	keyval->val_type = STRING_VALUE;
	keyval->string_val = "Felix Kitaka";	
	keyval->key = "name";

	symbol_table_insert(symbols, keyval);

	struct symbol_table *found = symbol_table_find(symbols, "name");
	sdebug(found->key_val->string_val);
}
