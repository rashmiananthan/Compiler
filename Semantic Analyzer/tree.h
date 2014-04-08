#ifndef TREE_H
#define TREE_H

#define MAXCHILDREN 1000

typedef struct treenode tree; 

struct treenode {
  int nodeKind;
  int numChildren;
  int val;
  int isArray;
  int factor;
  char* strval;
  int lineno;
  tree *parent;
  tree *children[MAXCHILDREN];
};

typedef struct identifier{
        int type;
        char *idval;
        int scope;
        struct identifier *next;
}id;

typedef struct strings{
	char *ip;
	struct strings *next;
}string;

//void append_ID(id **q, int t, char *ip, int s);

//void display_ID(id *q);

//int search_ID(id *q, char *up);

void add_string(string **s, char *p);

tree *maketree(int kind, int l);

tree *maketreeFactor(int kind, int type, int l);

tree *maketreeArray(int kind, int type, int lineno);

/* builds sub tree with leaf node */
tree *maketreeWithVal(int kind, int val, int l);

tree *maketreeWithString(int kind, int val, int l);

void addChild(tree *parent, tree *child);

void printAst(tree *node, int nestLevel);

void get_num_parameters(tree *root);

void generateSymbolTable(tree *root);

void RetValueExpr(tree *node, int scope);

void CheckProperIndexType(tree *node, tree *node2, int scope);

void get_num_parameters(tree *node);

enum nodeTypes {PROGRAM,DECLLIST,DECL,VARDECL,TYPESPECIFIER,FUNDECL,FORMALDECLLIST,FORMALDECL,FUNBODY,LOCALDECLLIST,STATEMENTLIST,STATEMENT,COMPOUNDSTMT,ASSIGNSTMT,CONDSTMT,WITHELSE,LOOPSTMT,RETURNSTMT,VAR,EXPRESSION,RELOP,ADDEXPR,ADDOP,TERM,MULOP,FACTOR,FUNCCALLEXPR,ARGLIST,IDENTIFIER,INTEGER,CHARACTER,STRINGCONS,NONE,OPERATOR,KWDTYPE};

enum dataTypes {INT_TYPE, CHAR_TYPE, VOID_TYPE};

enum operatorTypes {LESSTHAN,GREATERTHAN,GREATERTHANEQUAL,LESSTHANEQUAL,EQUALEQUAL,NOTEQUAL,ADD,SUB,MUL,DIV,EQUAL};

enum kwdTypes {IF,ELSE,WHILE,RETURN};

/* tree manipulation macros */ 
/* if you are writing your compiler in C, you would want to have a large collection of these */

#define nextAvailChild(node) node->children[node->numChildren] 
#define getChild(node, index) node->children[index]

#endif
