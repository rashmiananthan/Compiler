#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

/* For hash Table */
typedef struct hashtable{
	int size;
	struct idhash **nodes;
}hash;

/* For identifiers */
struct idhash{
	char *key;
	int pos;
	int type;
	int scope;
	int limit;
	int isArray;
	int param[1000];
	int num_parameter;
	struct idhash *next;
};

typedef struct funchash{
	int type;
	struct funchash *next;
}func;

/* Hash table functions */
hash *ST_Create(int size);
int ST_Insert(hash *symtab, char *data, int pos, int scope, int type);
int ST_Insert_Array(hash *symtab, char *data, int pos, int scope, int type, int limit, int isArray);
int ST_Lookup(hash *symtab, char *data, int scope);
int ST_Insert_Function(hash *hashtbl, char *data, int posloc, int scope, int type, int num_param, tree *node);
int ST_IndexExists(hash *hashtbl, char *data, int scope);
int ST_LookupReturnValue(hash *hashtbl, char *data, int scope);
int ST_CheckOutOfBounds(hash *hashtbl, char *data, int scope, int index);

