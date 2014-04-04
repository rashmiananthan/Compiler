//#include<strtab.h>
#include<stdio.h>
#include<tree.h>

extern tree *ast;
extern id *p;

int main() {
    if (!yyparse()){
	 printAst(ast, 1);
	 generateSymbolTable(ast);
	 display_ID(p);
	// display_string(s);
    }
  return 0;
}
