#include<tree.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "symtab.h"
/* string values for ast node types, makes tree output more readable */

char *nodeNames[33] = {"program","declList","decl","vardecl","typeSpecfier","funDecl","formalDeclList","formalDecl","funBody","localDeclList","statementList","statement","compoundStmt","assignStmt","condStmt","withElse","loopStmt","returnStmt","var","expression","relop","addExpr","addop","term","mulop","factor","funcCallExpr","argList","identifier","integer","character","string","None"};

id *p;
static int count = 0, funccount = 0;;
int rreturn, lreturn;
extern hash *hashtbl;
int fun[1000], error = 0;

tree *maketree(int kind, int l) {
  tree *this = (tree *) malloc(sizeof(struct treenode));
  this->nodeKind = kind;
  this->numChildren = 0;
  this->lineno = l;
  return this;

}

tree *maketreeArray(int kind, int val, int lineno) {
  tree *this = (tree *) malloc(sizeof(struct treenode));
  this->nodeKind = kind;
  this->isArray = val;
  this->lineno = lineno;
  this->numChildren = 0;
  return this;
}

tree *maketreeFactor(int kind, int val, int l) {
  tree *this = (tree *) malloc(sizeof(struct treenode));
  this->nodeKind = kind;
  this->factor = val;
  this->lineno = l;
  this->numChildren = 0;
  return this;
}

tree *maketreeWithVal(int kind, int val, int l) {
tree *this = (tree *) malloc(sizeof(struct treenode));
  this->nodeKind = kind;
  this->numChildren = 0;
  this->lineno = l;
  this->val = val;
  return this;

}

tree *maketreeWithString(int kind, int s, int l){
tree *this = (tree *) malloc(sizeof(struct treenode));
  this->nodeKind = kind;
  this->numChildren = 0;
  this->lineno = l;
  this->strval = (char *)s;
  return this;
}

void addChild(tree *parent, tree *child) {
  if (parent->numChildren == MAXCHILDREN) {
    printf("Cannot add child to parent node\n");
    exit(1);
  }
  nextAvailChild(parent) = child;
  parent->numChildren++;
}

void generateSymbolTable(tree *node){
int i = 0, check = 0, rettype, func;
static int scope = 0, pos = 1;
tree *child;

	if(node->nodeKind == DECL){
		scope = 0;
	}
	if(node->nodeKind == VARDECL){
			if(scope == 0){
				if(node->isArray == 1){
					if(ST_Insert_Array(hashtbl, node->children[1]->strval, pos++, scope, node->children[0]->val, node->children[2]->val, 1) == -1){
						error++;
                        	                printf("Line:%d Multiple array declaration of %s in the global scope\n", node->children[1]->lineno, node->children[1]->strval);
                                	        return;
                                        }
//	                                printf("Inserting Array %s of type %d limit %d pos %d and scope %d\n", node->children[1]->strval, node->children[0]->val, node->children[2]->val, pos, scope);
				}else{
//					printf("Inserting %s of type %d pos %d and scope %d\n", node->children[1]->strval, node->children[0]->val, pos, scope);
					if(ST_Insert(hashtbl, node->children[1]->strval, pos++, scope, node->children[0]->val) == -1){
						error++;
	 	                        	printf("Line:%d Multiple variable declaration of %s in the global scope\n", node->children[1]->lineno, node->children[1]->strval);
						return;
					} 
				}
			}else{
				if(node->isArray == 1){
                                        if(ST_Insert_Array(hashtbl, node->children[1]->strval, 0, scope, node->children[0]->val, node->children[2]->val, 1) == -1){
						error++;
                                                printf("Line:%d Multiple array declaration of %s in the local scope\n", node->children[1]->lineno, node->children[1]->strval);
                                                return;
                                        }
//                                      printf("Inserting Array %s of type %d limit %d pos %d and scope %d\n", node->children[1]->strval, node->children[0]->val, node->children[2]->val, pos, scope);
                                }else{
					if(ST_Insert(hashtbl, node->children[1]->strval, 0, scope, node->children[0]->val) == -1){
						error++;
        	                        	printf("Line:%d Multiple variable declaration of %s in the local scope\n", node->children[1]->lineno,  node->children[1]->strval);
						return;
				 	}
//				printf("Inserting %s of type %d pos 0 and scope %d\n", node->children[1]->strval, node->children[0]->val, scope);
				}
		
			}
	}
		
	if(node->nodeKind == FUNDECL){
			scope = 0;
			count = 0;
//			printf("Inserting %s of type %d pos %d and scope %d\n", node->children[1]->strval, node->children[0]->val, pos, scope);
			if(node->children[2]->nodeKind == FORMALDECLLIST){
				get_num_parameters(node->children[2]);
//				printf("Total parameters in %s function is %d\n", node->children[1]->strval, count);
				if(ST_Insert_Function(hashtbl, node->children[1]->strval, pos++, scope, node->children[0]->val, count, node->children[2]) == -1){
						error++;
					printf("Line:%d Multiple declaration of function %s \n", node->children[1]->lineno, node->children[1]->strval);
					return;
				}
				scope = pos-1;
			}
			else{
				if(ST_Insert(hashtbl, node->children[1]->strval, pos++, scope, node->children[0]->val) == -1){
						error++;
                                	printf("Line:%d Multiple declaration of function %s \n", node->children[1]->lineno, node->children[1]->strval);
	                                return;
                        	}
				scope = pos-1;
			}
        }
	if(node->nodeKind == FORMALDECL){
			if(ST_Insert(hashtbl, node->children[1]->strval, 0, scope, node->children[0]->val) == -1){
						error++;
                                printf("Line:%d Multiple declaration of variable %s in the same scope\n", node->children[1]->lineno, node->children[1]->strval);
				return;
			}
//			printf("Inserting %s of type %d pos 0 and scope %d\n", node->children[1]->strval, node->children[0]->val, scope);
	}
	if(node->nodeKind == ASSIGNSTMT){		
			if(node->children[0]->nodeKind == VAR){
				if((ST_Lookup(hashtbl, node->children[0]->children[0]->strval, scope)) == -1){
						error++;
	                                printf("Line:%d Undefined Variable %s\n", node->children[0]->children[0]->lineno, node->children[0]->children[0]->strval);
        	                        return;
                	        }
				if(node->children[0]->numChildren > 1){
					if(node->children[0]->children[1]->nodeKind == ADDEXPR){
						if(ST_IndexExists(hashtbl, node->children[0]->children[0]->strval, scope) == 1)
							CheckProperIndexType(node->children[1], node->children[0], scope);
						else{
						error++;
							printf("Line:%d Accessing scalar variable %s as an array\n", node->children[0]->children[0]->lineno, node->children[0]->children[0]->strval);
							return;
						}
					}
				}
				lreturn = ST_LookupReturnValue(hashtbl, node->children[0]->children[0]->strval, scope);
				RetValueExpr(node->children[2], scope);
			}
	}
	if(node->nodeKind == VAR){
			if((ST_Lookup(hashtbl, node->children[0]->strval, scope)) == -1){
						error++;
				printf("Line:%d Undeclared Variable %s\n", node->children[0]->lineno, node->children[0]->strval);
				return;
			}
//			printf("%s is present in symbol table\n", node->children[0]->strval);
			if(node->numChildren > 1){
				if(node->children[1]->nodeKind == ADDEXPR){
					if(ST_IndexExists(hashtbl, node->children[0]->strval, scope) == 1)
						CheckProperIndexType(node->children[1], node->children[0], scope);	
					else{
						error++;
						printf("Line:%d Accessing scalar variable %s as an array\n", node->children[0]->lineno, node->children[0]->strval);
						return;
					}
				}
			}
	}
	if(node->nodeKind == FUNCCALLEXPR){
			if((ST_Lookup(hashtbl, node->children[0]->strval, scope)) == -1){
						error++;
        	        	printf("Line:%d Undeclared Function %s\n", node->children[0]->lineno, node->children[0]->strval);
                	        return;
                        }
			if(node->numChildren == 1){
				 func = ST_FunctionParamMatch(hashtbl, node->children[0]->strval, scope, 0);
                                        if(func == 1){
						error++;
                                                printf("Line:%d Too few parameters for function:%s\n", node->children[0]->lineno, node->children[0]->strval);
                                                return;
                                        }
                                        else if(func == 2){
						error++;
                                                printf("Line:%d Too many parameters for function:%s\n", node->children[0]->lineno, node->children[0]->strval);
                                                return;
                                        }else{
					}			
			}
			if(node->numChildren > 1){
				if(node->children[1]->nodeKind == ARGLIST){
					funccount = 0;
					getparam(node->children[1], scope);
					func = ST_FunctionParamMatch(hashtbl, node->children[0]->strval, scope, funccount);
					if(func == 1){
						error++;
						printf("Line:%d Too few parameters for function:%s\n", node->children[0]->lineno, node->children[0]->strval);
						return;
					}
					else if(func == 2){
						error++;
						printf("Line:%d Too many parameters for function:%s\n", node->children[0]->lineno, node->children[0]->strval);
						return;
					}else{
						//Get_Types(node->children[1], scope);
						if((ST_GenerateSignatureCheck(hashtbl, node->children[0]->strval, scope, fun)) == -1){
						error++;
							printf("Line:%d Mismatch in arguments to the function %s\n", node->children[0]->lineno, node->children[0]->strval);
							return;
						}
					}		
				}
			}
	}	
	for(i = 0; i < node->numChildren; i++)
		generateSymbolTable(getChild(node, i));

}

void RetValueExpr(tree *node, int scope){
int i;

//if(node->nodeKind == FACTOR){
	if((node->factor == 2)||(node->factor == 3)){
		rreturn = ST_LookupReturnValue(hashtbl, node->children[0]->children[0]->strval, scope);
		if(lreturn != rreturn){
						error++;
			printf("Line:%d Type mismatch in assignments\n", node->lineno);
		}
		exit(0);
	}else if(node->factor == 4){
		rreturn = 0;
 		if(lreturn != rreturn){
						error++;
                        printf("Line:%d Type mismatch in assignments\n", node->lineno);
                }
		exit(0);
	}else if((node->factor == 5)||(node->factor == 6)){
		rreturn = 1;
		 if(lreturn != rreturn){
						error++;
                        printf("Line:%d Type mismatch in assignments\n", node->lineno);
                }
                exit(0);
	}
//}
	for(i = 0; i < node->numChildren; i++)
		RetValueExpr(getChild(node, i), scope);
}

void CheckProperIndexType(tree *node, tree *node2, int scope){
int i, value;
	
	if((node->factor == 5)||(node->factor == 6)){
						error++;
		printf("Line:%d Indexing the array variable:%s with non-integer type\n", node2->lineno, node2->strval);
		exit(0);
	}	
	if(node->factor == 4){
		if((ST_CheckOutOfBounds(hashtbl, node2->strval, scope, node->children[0]->val)) == -1){
						error++;
			printf("Line:%d Array index out of bounds for the variable : %s\n", node2->lineno, node2->strval);
			exit(0);
		}
	}
	if((node->factor == 2)||(node->factor == 3)){
		value = ST_LookupReturnValue(hashtbl, node->children[0]->children[0]->strval, scope);
		if((value == 1)|(value == 2)){
						error++;
			printf("Line:%d Indexing the array variable:%s with non-integer type\n", node2->lineno, node2->strval);
			exit(0);
		}else if(value == -1){
						error++;
			printf("Line:%d Invalid type has been passed as array index\n", node2->lineno);
			exit(0);
		}
	}
	for(i = 0; i < node->numChildren; i++)
		CheckProperIndexType(getChild(node, i), node2,scope);
}

void get_num_parameters(tree *node){
int i;
if(node->nodeKind == FORMALDECL) count++;
for(i = 0; i < node->numChildren; i++)
	get_num_parameters(getChild(node, i));
}

void get_num_parameters_inside_function(tree *node, int scope){
int i;
static j = 0;
        if(node->factor == 2) {
                fun[j++] = ST_LookupReturnValue(hashtbl, node->children[0]->children[0]->strval, scope);
		return;
        }else if(node->factor == 3){
                fun[j++] = ST_LookupReturnValue(hashtbl, node->children[0]->children[0]->strval, scope);
		return;
        }else if(node->factor == 4){
                fun[j++] = 0;
		return;
        }else if(node->factor == 5){
                fun[j++] = 1;
		return;
        }else if(node->factor == 6){
                fun[j++] = 1;
		return;
        }

for(i = 0; i < node->numChildren; i++)
        get_num_parameters_inside_function(getChild(node, i), scope);
}

void getparam(tree *node, int scope){

        if(node != NULL){
                if(node->numChildren >= 1){
                        funccount++;
                        get_num_parameters_inside_function(node, scope);
                        getparam(node->children[1], scope);
                }
        }
}

void printAst(tree *node, int nestLevel) {
//printf("%d with %s with %d children\n",node->nodeKind, nodeNames[node->nodeKind],node->numChildren);
    if (node->nodeKind == INTEGER) {
        printf("Line:%d <%d>\n", node->lineno, node->val);
    }
    else if(node->nodeKind == IDENTIFIER) {
        printf("Line:%d %s\n", node->lineno, node->strval);
    }else if(node->nodeKind == CHARACTER){
        printf("Line:%d %c\n", node->lineno, (char *)node->val);
   }else if(node->nodeKind == STRINGCONS){
        printf("Line:%d %s\n", node->lineno, node->strval);
   }else if(node->nodeKind == TYPESPECIFIER){
        if(node->val == 0){
                printf("INT\n");
        }else if(node->val == 1){
                printf("CHAR\n");
        }else{
                printf("VOID\n");
        }
   }//else if(node->nodeKind == FACTOR){
//		printf("factor value : %d\n", node->factor);
//	}
   else if(node->nodeKind == NONE){
        return;
   }/*else if(node->nodeKind ==DECLLIST){
   }else if(node->nodeKind ==FORMALDECLLIST){
   }else if(node->nodeKind == LOCALDECLLIST){
   }else if(node->nodeKind == STATEMENTLIST){
   }else if(node->nodeKind == ADDEXPR){
   }else if(node->nodeKind == TERM){
   }else if(node->nodeKind == FACTOR){
   }*/else if(node->nodeKind == OPERATOR){
        if(node->val == 0){
                printf("<\n");
        }else if(node->val == 1){
                printf(">\n");
        }else if(node->val == 2){
                printf(">=\n");
        }else if(node->val == 3){
                printf("<=\n");
        }else if(node->val == 4){
                printf("==\n");
        }else if(node->val == 5){
                printf("!=\n");
        }else if(node->val == 6){
                printf("+\n");
        }else if(node->val == 7){
                printf("-\n");
        }else if(node->val == 8){
                printf("*\n");
        }else if(node->val == 9){
                printf("/\n");
        }else if(node->val == 10){
                printf("=\n");
        }
   }else if(node->nodeKind == KWDTYPE){
        if(node->val == IF){
                printf("IF\n");
        }else if(node->val == ELSE){
                printf("ELSE\n");
        }else if(node->val == WHILE){
                printf("WHILE\n");
        }else if(node->val == RETURN){
                printf("RETURN\n");
        }
   }else{
        printf("Line:%d %s\n", node->lineno, nodeNames[node->nodeKind]);
   }

 int i, j;
  for (i = 0; i < node->numChildren; i++)  {
    for (j = 0; j < nestLevel; j++){
        printf("\t");
    }
        tree *n = getChild(node, i);
        printAst(getChild(node, i), nestLevel + 1);
  }
}


/*
void append_ID(id **q, int t, char *ip, int s){
//printf("inserting %d, %s, %d",t, ip, s);
        id *temp,*r;
        if(*q == NULL){
                temp = malloc(sizeof(id));
                temp->type = t;
                temp->idval = ip;
                temp->scope = s;
                temp->next = NULL;
                *q = temp;
        }else{
                temp = *q;
                while(temp->next != NULL)
                        temp = temp->next;
                r = malloc(sizeof(id));
                r->type = t;
                r->idval = ip;
                r->scope = s;
                r->next = NULL;
                temp->next = r;
        }
}

int search_ID(id *q, char *p){
	if(q == NULL)
		return 0;
	else{
		while(q!=NULL){
			if(!strcmp(q->idval, p))
				return 1;	
			q = q->next;
		}
		return 0;
	}
}

void display_ID(id *q){
char *ty;
printf("\n\n----------------------------------SYMBOL TABLE-----------------------------\n\n");
printf("DATA TYPE\t\tIDENTIFIER NAME\t\tSCOPE\n\n");
printf("\n\n---------------------------------------------------------------------------\n\n");
        while(q!=NULL){
		if(q->type == 0) ty = "INT";
		else if(q->type == 1) ty = "CHAR";
		else if(q->type == 2) ty = "VOID";
		else ty = "UNDEFINED";
		if(q->scope == 1)
	                printf("%s\t\t\t%s\t\t\tGLOBAL\n",ty, q->idval);
		else
	                printf("%s\t\t\t%s\t\t\tLOCAL\n",ty, q->idval);
                q = q->next;
        }
}
*/
void add_string(string **s, char *inp){
 string *temp,*r;
        if(*s == NULL){
                temp = malloc(sizeof(string));
                temp->ip = inp;
                temp->next = NULL;
                *s = temp;
        }else{
                temp = *s;
                while(temp->next != NULL)
                        temp = temp->next;
                r = malloc(sizeof(string));
                r->ip = inp;
                r->next = NULL;
                temp->next = r;
        }
}
/*
void display_string(string *s){
printf("\n\n\n-------------------------STRINGS USED------------------------\n\n\n");
while(s!=NULL){
	printf("%s\n",s->ip);
	s = s->next;
}
}*/
