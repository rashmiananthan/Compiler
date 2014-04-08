#include <stdio.h>
#include <stdlib.h>
#include "symtab.h"
int array[100];
static char *mystrdup(const char *s)
{
        char *b;
        if(!(b=malloc(strlen(s)+1))) return NULL;
        strcpy(b, s);
        return b;
}

unsigned int hashfunction(hash *hashtable, char* str){
        unsigned int hashval;
        hashval = 0;
	while(*str) hashval += (unsigned char)*str++;
        return hashval;
}

hash *ST_Create(int size){
        hash *hashtbl;
        if(size < 1)return NULL;

        if(!(hashtbl = malloc(sizeof(hash)))) return NULL;

        if(!(hashtbl->nodes = calloc(size, sizeof(struct idhash*)))){
                free(hashtbl);
                return NULL;
        }
        hashtbl->size = size;
        return hashtbl;
}

int ST_Insert(hash *hashtbl, char *data, int posloc, int scope, int type){
        struct idhash *idnode;
        unsigned int pos = hashfunction(hashtbl, data);
        idnode = hashtbl->nodes[pos];
	/* Checking for multiple defined variables and functions */
        while(idnode){
                if((!strcmp(idnode->key, data))&&(idnode->scope == scope)){
                        return -1;
                }
                idnode = idnode->next;
        }

        if(!(idnode = malloc(sizeof(struct idhash)))) return -1;
        if(!(idnode->key=mystrdup(data))) {
                free(idnode);
                return -1;
        }
	idnode->pos = posloc;
	idnode->scope = scope;
	idnode->type = type;
        idnode->next = hashtbl->nodes[pos];
        hashtbl->nodes[pos] = idnode;
        return 1;
}

int ST_Insert_Array(hash *hashtbl, char *data, int posloc, int scope, int type, int limit, int isArray){
        struct idhash *idnode;
        unsigned int pos = hashfunction(hashtbl, data);
        idnode = hashtbl->nodes[pos];
        /* Checking for multiple defined variables and functions */
        while(idnode){
                if((!strcmp(idnode->key, data))&&(idnode->scope == scope)){
                        return -1;
                }
                idnode = idnode->next;
        }

        if(!(idnode = malloc(sizeof(struct idhash)))) return -1;
        if(!(idnode->key=mystrdup(data))) {
                free(idnode);
                return -1;
        }
        idnode->pos = posloc;
        idnode->scope = scope;
        idnode->type = type;
	idnode->limit = limit;
	idnode->isArray = 1;
        idnode->next = hashtbl->nodes[pos];
        hashtbl->nodes[pos] = idnode;
        return 1;
}

int ST_IndexExists(hash *hashtbl, char *data, int scope){
	struct idhash *node;
        int flag = -1;
        int pos = hashfunction(hashtbl, data);
        /* Check for variable in current scope */
        node = hashtbl->nodes[pos];
        while(node){
                if((!strcmp(node->key, data))&&(node->scope == scope)){
                        if(node->isArray == 1)
				flag = 1;
			else
				flag = -1;
                }
                node = node->next;
        }
        /* Check for variables in global scope */
        node = hashtbl->nodes[pos];
        if(node){
                if((!strcmp(node->key, data))&&(node->scope == 0)){
			if(node->isArray == 1)
                                flag = 1;
                        else
                                flag = -1;
                }
        }
        if(flag == 1)
                return 1;
        else
                return -1;
}

int ST_Insert_Function(hash *hashtbl, char *data, int posloc, int scope, int type, int num_param, tree *node){
	int i;
	struct idhash *idnode;
	unsigned int pos = hashfunction(hashtbl, data);	
	
	idnode = hashtbl->nodes[pos];
	while(idnode){
		 if((!strcmp(idnode->key, data))&&(idnode->scope == scope)){
                        return -1;
                 }
                idnode = idnode->next;
	}

        if(!(idnode = malloc(sizeof(struct idhash)))) return -1;
        if(!(idnode->key=mystrdup(data))) {
                free(idnode);
                return -1;
        }

        idnode->pos = posloc;
        idnode->scope = scope;
        idnode->type = type;
	idnode->num_parameter = num_param;
	InitializeParam(node, idnode);
        idnode->next = hashtbl->nodes[pos];
        hashtbl->nodes[pos] = idnode;
        return 1;
}

void InitializeParam(tree *node, struct idhash *id){
int i;
static int j = 0;
	
		if(node->nodeKind == FORMALDECL){
			id->param[j++] = node->children[0]->val;
		}

	for(i = 0; i < node->numChildren; i++)
		InitializeParam(getChild(node, i), id);
}

int ST_Lookup(hash *hashtbl, char *data, int scope){
        struct idhash *node;
	int flag = -1;
        int pos = hashfunction(hashtbl, data);
	/* Check for variable in current scope */
        node = hashtbl->nodes[pos];
        while(node){
                if((!strcmp(node->key, data))&&(node->scope == scope)){
			flag = 1;
		}
                node = node->next;
        }
	/* Check for variables in global scope */
	node = hashtbl->nodes[pos];
	if(node){
		if((!strcmp(node->key, data))&&(node->scope == 0)){
			flag = 0;
		}
	}
	if((flag == 0)||(flag == 1))
		return 1;
	else
		return -1;
}

int ST_FunctionParamMatch(hash *hashtbl, char *data, int scope, int count){
	struct idhash *node;
	int limit = 0;
	int flag = -1;
	int pos = hashfunction(hashtbl, data);
	 node = hashtbl->nodes[pos];
        while(node){
                if((!strcmp(node->key, data))&&(node->scope == scope)){
                      limit = node->num_parameter;  
		      if(limit < count)
				flag = 2;
		      else if(limit > count)
				flag = 1;
			else
				flag = 3;
                }
                node = node->next;
        }
        /* Check for variables in global scope */
        node = hashtbl->nodes[pos];
        if(node){
                if((!strcmp(node->key, data))&&(node->scope == 0)){
                        limit = node->num_parameter;
                      if(limit < count)
                                flag = 2;
                      else if(limit > count)
                                flag = 1;
                        else
                                flag = 3;
                }
        }
        if(flag == 1)
                return 1;
	else if(flag == 2)
		return 2;
        else
                return -1;
}

int ST_LookupReturnValue(hash *hashtbl, char *data, int scope){
	struct idhash *node;
        int pos = hashfunction(hashtbl, data);
        /* Check for variable in current scope */
        node = hashtbl->nodes[pos];
        while(node){
        	if((!strcmp(node->key, data))&&(node->scope == scope)){
                	return node->type;
                }
                node = node->next;
        }
        node = hashtbl->nodes[pos];
	if(node){
                 if((!strcmp(node->key, data))&&(node->scope == 0)){
                        return node->type;
                }
        }
        return -1;
}

int ST_GenerateSignatureCheck(hash *hashtbl, char *data, int scope, int p[]){
	struct idhash *node;
	int i = 0;
        int pos = hashfunction(hashtbl, data);
        /* Check for variable in current scope */
        node = hashtbl->nodes[pos];
        while(node){
                if((!strcmp(node->key, data))&&(node->scope == scope)){
                        for(i = 0; i < node->num_parameter; i++){
				if(node->param[i] != p[i])
					return -1;
			}
                }
                node = node->next;
        }
        node = hashtbl->nodes[pos];
        if(node){
                 if((!strcmp(node->key, data))&&(node->scope == 0)){
                	 for(i = 0; i < node->num_parameter; i++){
                                if(node->param[i] != p[i])
                                        return -1;
                     }
		}
        }
        return 1;
}

int ST_CheckOutOfBounds(hash *hashtbl, char *data, int scope, int index){
	struct idhash *node;
	int pos = hashfunction(hashtbl, data);
	node = hashtbl->nodes[pos];
	while(node){
                if((!strcmp(node->key, data))&&(node->scope == scope)&&(node->isArray == 1)){
                        if(node->limit < index) return -1;
                }
                node = node->next;
        }
        node = hashtbl->nodes[pos];
        if(node){
                 if((!strcmp(node->key, data))&&(node->scope == 0)&&(node->isArray == 1)){
                        if(node->limit < index) return -1;
                }
        }
        return -1;
}

