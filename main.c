#include <stdio.h>
#include <assert.h>

#include "lexer.h"
#include "parser.h"
#include "symbol_table.h"
#include "interpreter.h"
#include "executor.h"

#define _assert_variable_value(x, y, z) printf("Testing: %s on line: %d\n", __func__, __LINE__); assert_variable_value(x, y, z)

void test_symbol_table();
void run_all_tests();

int main(void)
{
  	//printf("Test: %s, %d", __func__, __LINE__);
	run_all_tests();
  	
  	return 0;
}

void assert_variable_value(char *code, char *var, int result)
{
  	struct lexer *lexer;
	struct executor *executor; 
	struct symbol_table *found;

	lexer = lexer_parse(code);
	lexer_analyze(lexer);
	//lexer_print_tokens(lexer); 

	executor = executor_execute(lexer);

	found = symbol_table_find(executor->interpreter->symbols, var); 
	
	assert(found != NULL);
	assert(found->key_val->integer_val == result);	

	lexer_free(lexer);
	executor_free(executor);
}

void test_that_two_variables_can_be_initiated()
{
  	char *code = "$var = 1 + 2; $x =  2 + 3;";

	_assert_variable_value(code, "x", 5);
	_assert_variable_value(code, "var", 3);
}

void test_that_nested_brackets_can_be_processed()
{
	char *code = "$var = 1 + (2 + 3) + (10 - (9 - 2));";

	_assert_variable_value(code, "var", 9);
}

void test_that_a_variable_can_be_used_in_another_statement()
{
  	char *code = "$x = 1 + 2; $y = $x + 3;";

	_assert_variable_value(code, "y", 6);
}

void test_that_two_variables_can_be_added()
{
  	char *code = "$x = 1 + 2; $y = $x + 3; $z = $x + $y;";
  	
	_assert_variable_value(code, "z", 9);
}

void run_all_tests()
{
  	test_that_two_variables_can_be_initiated();
	test_that_nested_brackets_can_be_processed();
	test_that_a_variable_can_be_used_in_another_statement();
	test_that_two_variables_can_be_added();
}

void test_symbol_table()
{
	struct symbol_table *symbols = symbol_table_init();
	struct key_val *keyval2, *keyval = key_val_init();
	keyval->val_type = STRING_VALUE;
	keyval->string_val = "Felix Kitaka";	
	keyval->key = "name";

	keyval2 = key_val_init();
	keyval2->val_type = STRING_VALUE;
	keyval2->string_val = "No name";
	keyval2->key = "other";

	symbol_table_insert(symbols, keyval);
	symbol_table_insert(symbols, keyval2);

	struct symbol_table *found = symbol_table_find(symbols, "name");
	struct symbol_table *found2 = symbol_table_find(symbols, "other");
	
	sdebug(found->key_val->string_val);
	sdebug(found2->key_val->string_val);
}
