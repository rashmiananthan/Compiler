#ifndef TREE_H
#define TREE_H

#define MAXCHILDREN 30

typedef struct treenode tree; 

/* tree node - you may want to add more fields */
struct treenode {
  int nodeKind;
  int numChildren;
  int val;
  char* strval;
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

void append_ID(id **q, int t, char *ip, int s);

void display_ID(id *q);

int search_ID(id *q, char *up);

void add_string(string **s, char *p);

/* builds sub tree with zeor children  */
tree *maketree(int kind);

/* builds sub tree with leaf node */
tree *maketreeWithVal(int kind, int val);

tree *maketreeWithString(int kind, int val);

void addChild(tree *parent, tree *child);

void printAst(tree *root, int nestLevel);

void generateSymbolTable(tree *root);

enum nodeTypes {PROGRAM,DECLLIST,DECL,VARDECL,TYPESPECIFIER,FUNDECL,FORMALDECLLIST,FORMALDECL,FUNBODY,LOCALDECLLIST,STATEMENTLIST,STATEMENT,COMPOUNDSTMT,ASSIGNSTMT,CONDSTMT,WITHELSE,LOOPSTMT,RETURNSTMT,VAR,EXPRESSION,RELOP,ADDEXPR,ADDOP,TERM,MULOP,FACTOR,FUNCCALLEXPR,ARGLIST,IDENTIFIER,INTEGER,CHARACTER,STRINGCONS,NONE,OPERATOR,KWDTYPE};

enum dataTypes {INT_TYPE, CHAR_TYPE, VOID_TYPE};

enum operatorTypes {LESSTHAN,GREATERTHAN,GREATERTHANEQUAL,LESSTHANEQUAL,EQUALEQUAL,NOTEQUAL,ADD,SUB,MUL,DIV,EQUAL};

enum kwdTypes {IF,ELSE,WHILE,RETURN};

/* tree manipulation macros */ 
/* if you are writing your compiler in C, you would want to have a large collection of these */

#define nextAvailChild(node) node->children[node->numChildren] 
#define getChild(node, index) node->children[index]

#endif
