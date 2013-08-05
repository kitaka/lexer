#include "debug.h"

void sdebug(const char *str)
{
  	printf("[%s]\n", str);
}

void idebug(int i)
{
  	printf("[%d]\n", i);
}

void cdebug(char c)
{
  	printf("[%c]\n", c);
}
