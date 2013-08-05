#include <stdio.h>
#include "lexer.h"


int main(void)
{
  	struct lexer *lexer;

	lexer = lexer_init("$var = 234 + 567;");
	lexer_analyze(lexer);
	lexer_print_tokens(lexer);	
	lexer_free(lexer);
  	return 0;
}
