#ifndef __EXECUTOR_HEADER__
#define __EXECUTOR_HEADER__

#include <stdlib.h>
#include "debug.h"
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

struct executor {
  	struct lexer *lexer;
	struct interpreter *interpreter;
};

struct executor *executor_init();
void executor_free(struct executor *executor);
struct executor *executor_execute(struct lexer *lexer);

#endif

