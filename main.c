#include <stdio.h>
#include "lexer.h"
#include "parser.h"
#include "symbol_table.h"
#include "interpreter.h"

void test_symbol_table();

int main(void)
{
  	struct lexer *lexer;

	lexer = lexer_parse("$var = 234 + 567 + 2 + 4;");
	lexer_analyze(lexer);
	//lexer_print_tokens(lexer);

	struct ast *ast = ast_parse(lexer);
	//ast_debug(ast);

	struct interpreter *interpreter = interpreter_interprete(ast);	
	struct symbol_table *found = symbol_table_find(interpreter->symbols, "var");
	idebug(found->key_val->integer_val);
	
	interpreter_free(interpreter);	
	ast_free(ast);
	lexer_free(lexer);
	
	//test_symbol_table();

  	return 0;
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
