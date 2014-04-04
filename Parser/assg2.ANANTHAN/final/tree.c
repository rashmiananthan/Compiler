#include<tree.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/* string values for ast node types, makes tree output more readable */

char *nodeNames[33] = {"program","declList","decl","vardecl","typeSpecfier","funDecl","formalDeclList","formalDecl","funBody","localDeclList","statementList","statement","compoundStmt","assignStmt","condStmt","withElse","loopStmt","returnStmt","var","expression","relop","addExpr","addop","term","mulop","factor","funcCallExpr","argList","identifier","integer","character","string","None"};

id *p;
tree *maketree(int kind) {
  tree *this = (tree *) malloc(sizeof(struct treenode));
  this->nodeKind = kind;
  this->numChildren = 0;
  return this;

}

tree *maketreeWithVal(int kind, int val) {
tree *this = (tree *) malloc(sizeof(struct treenode));
  this->nodeKind = kind;
  this->numChildren = 0;
  this->val = val;
  return this;

}

tree *maketreeWithString(int kind, int s){
tree *this = (tree *) malloc(sizeof(struct treenode));
  this->nodeKind = kind;
  this->numChildren = 0;
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

void printAst(tree *node, int nestLevel) {
//printf("%d with %s with %d children\n",node->nodeKind, nodeNames[node->nodeKind],node->numChildren);
    if (node->nodeKind == INTEGER) {
	printf("<%d>\n", node->val);
    }    
    else if(node->nodeKind == IDENTIFIER) {
	printf("%s\n", node->strval);
    }else if(node->nodeKind == CHARACTER){
        printf("%c\n", (char *)node->val);
   }else if(node->nodeKind == STRINGCONS){
        printf("%s\n", node->strval);
   }else if(node->nodeKind == TYPESPECIFIER){
	if(node->val == 0){
		printf("INT\n");
	}else if(node->val == 1){
		printf("CHAR\n");
	}else{
		printf("VOID\n");
	}
   }else if(node->nodeKind == NONE){
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
	printf("%s\n", nodeNames[node->nodeKind]);		
   }
  int i, j;
  for (i = 0; i < node->numChildren; i++)  {
    for (j = 0; j < nestLevel; j++){ 
	printf("\t");
    }
	tree *n = getChild(node, i);
//printf("%d with %d children\n",n->nodeKind,n->numChildren);
	printAst(getChild(node, i), nestLevel + 1);
  }
}

void generateSymbolTable(tree *node){
int i, check = 0;
static int flag = 0;
tree *child;
//printf("---Name: %s\n", nodeNames[node->nodeKind]);
	if(node->nodeKind == DECL){
		flag = 1;
	}
	if(node->nodeKind == VARDECL){
		if(flag == 1){
			append_ID(&p, node->children[0]->val, node->children[1]->strval, 1);
		}else
			append_ID(&p, node->children[0]->val, node->children[1]->strval, 0);
	}
	if(node->nodeKind == FUNDECL){
                        append_ID(&p, node->children[0]->val, node->children[1]->strval, 1);
        }
	if(node->nodeKind == FORMALDECL)
                        append_ID(&p, node->children[0]->val, node->children[1]->strval, 0);
	if(node->nodeKind == FUNBODY ){		
		flag = 0;
	}
	if((node->nodeKind == VAR)||(node->nodeKind == FUNCCALLEXPR)){
		check = search_ID(p,node->children[0]->strval);
		if(check!=1) append_ID(&p, 3, node->children[0]->strval, 0);
	}
	for(i = 0; i < node->numChildren; i++)
		generateSymbolTable(getChild(node, i));

}
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

void display_string(string *s){
printf("\n\n\n-------------------------STRINGS USED------------------------\n\n\n");
while(s!=NULL){
	printf("%s\n",s->ip);
	s = s->next;
}
}
