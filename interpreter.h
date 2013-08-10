#ifndef __INTERPRETER_HEADER__
#define __INTERPRETER_HEADER__

#include "parser.h"
#include "symbol_table.h"

struct interpreter {
  	struct ast *ast;
	struct symbol_table *symbols;
};

struct intepreter *intepreter_init();
struct interpreter *interpreter_interprete(struct ast *ast);
int interprete_node(struct interpreter *interpreter, struct ast *ast);
void interpreter_free(struct interpreter *interpreter);

#endif


