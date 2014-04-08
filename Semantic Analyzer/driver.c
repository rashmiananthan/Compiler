#include<stdio.h>
#include<tree.h>
#include<symtab.h>

extern tree *ast;
hash *hashtbl;
extern int error;

int main() {
    if (!yyparse()){
//        printAst(ast, 1);
    	if(!(hashtbl=ST_Create(1000))) {
        	 fprintf(stderr, "ERROR: hashtbl_create() failed\n");
	         exit(EXIT_FAILURE);
    	}
	generateSymbolTable(ast);
	printf("Total errors in the program:%d\n", error);
	if(error == 0)
		printf("Congratulations! The given input program has passed the semantic check\n");
    }
  return 0;
}
