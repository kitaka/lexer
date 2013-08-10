#ifndef __SYMBOL_TABLE_HEADER__
#define __SYMBOL_TABLE_HEADER__

#include <stdlib.h>

#define STRING_VALUE 1

struct key_val {
  	char *key;
	int val_type;
	union {
	  	char *string_val;
		int integer_val;
	};
};

struct symbol_table {
  	int scope;
  	struct key_val *key_val;	
	struct symbol_table *next;
	struct symbol_table *prev;
};

struct symbol_table *symbol_table_init();
struct key_val *key_val_init();
void key_val_free(struct key_val *key_val);
int key_val_compare(struct key_val *keyval1, struct key_val *keyval2);
void symbol_table_free(struct symbol_table *symbol_table);
void symbol_table_insert(struct symbol_table *symbol_table, struct key_val *key_val);
struct symbol_table *symbol_table_find(struct symbol_table *symbol_table, char *key);

#endif


