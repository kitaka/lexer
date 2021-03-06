#include "symbol_table.h"

struct symbol_table *symbol_table_init()
{
  	struct symbol_table *symbols = calloc(1, sizeof(struct symbol_table));

	if (symbols == NULL) {
	  	perror("Could not allocate memory for symbol table");
		return NULL;
	}

	symbols->key_val = NULL;
	symbols->next = NULL;
	symbols->prev = NULL;
	symbols->scope = 0;

	return symbols;
}

struct key_val *key_val_init()
{
  	struct key_val *keyval = calloc(1, sizeof(struct key_val));

	if (keyval == NULL) {
		perror("Could not allocate memory for key val");
		return NULL;
	}

	return keyval;
}

void key_val_free(struct key_val *keyval)
{
  	free(keyval->key);
	free(keyval->string_val);
	free(keyval);
}

int key_val_compare(struct key_val *keyval1, struct key_val *keyval2)
{
  	if ((keyval1 == NULL) || (keyval2 == NULL)) return 0;
  	return strcmp(keyval1->key, keyval2->key) == 0;
}

void symbol_table_free(struct symbol_table *symbols)
{
  	if (symbols == NULL) return;

	key_val_free(symbols->key_val);
	symbol_table_free(symbols->next);
	symbol_table_free(symbols->prev);
	free(symbols);
}

void symbol_table_insert(struct symbol_table *symbols, struct key_val *keyval)
{
  	if (symbols == NULL) return ;

	// we could find if the key_val already exists before we insert it
	struct symbol_table *search = symbol_table_find_keyval(symbols, keyval);
	if (search != NULL) {
		search->key_val = keyval;
		return ;
	}

	if (symbols->head == NULL) symbols->head = symbols;

	if (symbols->head->key_val != NULL)
		symbols->head = symbols->head->next = symbol_table_init();

	symbols->head->key_val = keyval;
}

struct symbol_table *symbol_table_find_keyval(struct symbol_table *symbols, struct key_val *keyval)
{
  	if (symbols == NULL) return NULL;
	
	if (key_val_compare(symbols->key_val, keyval)) return symbols;

	return symbol_table_find_keyval(symbols->next, keyval);

	return NULL;
}

struct symbol_table *symbol_table_find(struct symbol_table *symbols, char *key)
{
	struct key_val keyval;
	keyval.key = key;

	return symbol_table_find_keyval(symbols, &keyval);	
}

