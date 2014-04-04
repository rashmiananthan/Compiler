%{
#include <stdio.h>
#include <stdlib.h>
#include <tree.h>
#include <string.h>
extern int yylineno;
char* strval;
tree *ast;
id *p;
string *s;
%}

%union
{
  int value;
  struct treenode *node;
  char *strval;
}

%token <value> ID CHAR STRING NUM KWD_IF KWD_ELSE KWD_WHILE KWD_INT KWD_STRING KWD_CHAR KWD_RETURN KWD_VOID OPER_ADD OPER_SUB OPER_MUL OPER_DIV OPER_LT OPER_GT OPER_GTE OPER_LTE OPER_EQ OPER_NEQ OPER_ASGN LSQ_BRKT RSQ_BRKT LCRLY_BRKT RCRLY_BRKT LPAREN RPAREN COMMA SEMICLN ILLEGAL_TOK

%type <node> program declList decl varDecl typeSpecifier funDecl formalDeclList formalDecl funBody localDeclList statementList statement compoundStmt assignStmt condStmt loopStmt returnStmt var expression relop addExpr addop term mulop factor funcCallExpr argList 

%start program

%%

program		:declList
		{
		  tree *progNode = maketree(PROGRAM);
                  addChild(progNode, $1);
	          ast = progNode;
	        }
		;
declList	:
		{
		  tree *declNode = maketree(NONE);
		  $$ = declNode;
		}
		|decl declList
		{
		 tree *declNode = maketree(DECLLIST);
		 addChild(declNode, $1);
		 addChild(declNode, $2);
		 $$ = declNode;
		}
		;
decl		:varDecl
		{
		 tree *declNode = maketree(DECL);
		 addChild(declNode, $1);
		 $$ = declNode;
		}
		|funDecl
		{
		 tree *declNode = maketree(DECL);
                 addChild(declNode, $1);
                 $$ = declNode;
		}
		;
varDecl		:typeSpecifier ID LSQ_BRKT NUM RSQ_BRKT SEMICLN
	        {	
		 tree *varDeclNode = maketree(VARDECL);
		 addChild(varDeclNode, $1);
		 addChild(varDeclNode, maketreeWithString(IDENTIFIER, $2));
		 addChild(varDeclNode, maketreeWithVal(INTEGER, $4));
                 $$ = varDeclNode;
		}
		|typeSpecifier ID SEMICLN
		{
		 tree *varDeclNode = maketree(VARDECL);
                 addChild(varDeclNode, $1);
                 addChild(varDeclNode, maketreeWithString(IDENTIFIER, $2));
                 $$ = varDeclNode;
		}
		;
typeSpecifier	:KWD_INT
		{
		 $$ = maketreeWithVal(TYPESPECIFIER, INT_TYPE);
		}
		|KWD_CHAR
		{
		 $$ = maketreeWithVal(TYPESPECIFIER, CHAR_TYPE);
		}
		|KWD_VOID
		{
		 $$ = maketreeWithVal(TYPESPECIFIER, VOID_TYPE);
		}
		;
funDecl		:typeSpecifier ID LPAREN formalDeclList RPAREN funBody
		{
		 tree *funDeclNode = maketree(FUNDECL);
		 addChild(funDeclNode, $1);
                 addChild(funDeclNode, maketreeWithString(IDENTIFIER, $2));
		 addChild(funDeclNode, $4);
		 addChild(funDeclNode, $6);
		 $$ = funDeclNode;
		}
		|typeSpecifier ID LPAREN RPAREN funBody
		{
		 tree *funDeclNode = maketree(FUNDECL);
                 addChild(funDeclNode, $1);
                 addChild(funDeclNode, maketreeWithString(IDENTIFIER, $2));
                 addChild(funDeclNode, $5);
                 $$ = funDeclNode;
		}
		;
formalDeclList	:formalDecl
		{
		 tree *formaldecllistNode = maketree(FORMALDECLLIST);
		 addChild(formaldecllistNode, $1);
		 $$ = formaldecllistNode;
		}
		|formalDecl COMMA formalDeclList
		{
		 tree *formaldecllistNode = maketree(FORMALDECLLIST);
		 addChild(formaldecllistNode, $1);
		 addChild(formaldecllistNode, $3);
                 $$ = formaldecllistNode;
		}
		;
formalDecl	:typeSpecifier ID
		{
		 tree *formalDeclNode = maketree(FORMALDECL);
		 addChild(formalDeclNode, $1);
		 addChild(formalDeclNode, maketreeWithString(IDENTIFIER, $2)); 
		 $$ = formalDeclNode;
		}
		|typeSpecifier ID LSQ_BRKT RSQ_BRKT
		{
		 tree *formalDeclNode = maketree(FORMALDECL);
                 addChild(formalDeclNode, $1);
                 addChild(formalDeclNode, maketreeWithString(IDENTIFIER, $2));
                 $$ = formalDeclNode;
		}
		;
funBody		:LCRLY_BRKT localDeclList statementList RCRLY_BRKT
		{
		 tree *funbodynode = maketree(FUNBODY);
		 addChild(funbodynode, $2);
		 addChild(funbodynode, $3);
		 $$ = funbodynode;
		}
		;
localDeclList	:
		{
		  tree *declNode = maketree(NONE);
                  $$ = declNode;
		}
		|varDecl localDeclList
		{
		 tree *localdecllist = maketree(LOCALDECLLIST);
		 addChild(localdecllist, $1);
		 addChild(localdecllist, $2);
		 $$ = localdecllist;
		}
		;
statementList	:
		{
		 tree *declNode = maketree(NONE);
                 $$ = declNode;
		}
		|statement statementList
		{
		 tree *stmtNode = maketree(STATEMENTLIST);
		 addChild(stmtNode, $1);
		 addChild(stmtNode, $2);
		 $$ = stmtNode;
		}
		;
statement	:compoundStmt
		{
		 tree *stmtNode = maketree(STATEMENT);
                 addChild(stmtNode, $1);
                 $$ = stmtNode;
		}
		|assignStmt
		{
		 tree *stmtNode = maketree(STATEMENT);
                 addChild(stmtNode, $1);
                 $$ = stmtNode;
		}
		|condStmt
		{
		 tree *stmtNode = maketree(STATEMENT);
                 addChild(stmtNode, $1);
                 $$ = stmtNode;
		}
		|loopStmt
		{
		 tree *stmtNode = maketree(STATEMENT);
                 addChild(stmtNode, $1);
                 $$ = stmtNode;
		}
		|returnStmt
		{
		 tree *stmtNode = maketree(STATEMENT);
                 addChild(stmtNode, $1);
                 $$ = stmtNode;
		}
		;
compoundStmt	:LCRLY_BRKT statementList RCRLY_BRKT
		{
		 tree *stmtNode = maketree(COMPOUNDSTMT);
                 addChild(stmtNode, $2);
                 $$ = stmtNode;
		}
		;
assignStmt	:var OPER_ASGN expression SEMICLN
		{
		 tree *assignStmtNode = maketree(ASSIGNSTMT);
		 addChild(assignStmtNode, $1);
		 addChild(assignStmtNode, maketreeWithVal(OPERATOR, EQUAL));
		 addChild(assignStmtNode, $3);
		 $$ = assignStmtNode;
		}
		|expression SEMICLN
		{
		 tree *assignStmtNode = maketree(ASSIGNSTMT);
                 addChild(assignStmtNode, $1);
                 $$ = assignStmtNode;
		}
		;
condStmt	:KWD_IF LPAREN expression RPAREN compoundStmt
		{
		 tree *ifNode = maketree(CONDSTMT);
		 addChild(ifNode, maketreeWithVal(KWDTYPE, IF));
		 addChild(ifNode, $3);
                 addChild(ifNode, $5);
		 $$ = ifNode;
		}
		|KWD_IF LPAREN expression RPAREN compoundStmt KWD_ELSE compoundStmt
		{
		 tree *elseNode = maketree(CONDSTMT);
		 addChild(elseNode, maketreeWithVal(KWDTYPE, IF));
		 addChild(elseNode, $3);
                 addChild(elseNode, $5);
		 addChild(elseNode, maketreeWithVal(KWDTYPE, ELSE));
                 addChild(elseNode, $7);
		 $$ = elseNode;
		}
		;
loopStmt	:KWD_WHILE LPAREN expression RPAREN statement
		{
		 tree *loopStmtNode = maketree(LOOPSTMT);
		 addChild(loopStmtNode, maketreeWithVal(KWDTYPE, WHILE));
		 addChild(loopStmtNode, $3);
		 addChild(loopStmtNode, $5);
		 $$ = loopStmtNode;
		}
		;
returnStmt	:KWD_RETURN SEMICLN
		{
		 tree *returnStmtNode = maketree(RETURNSTMT);
		 addChild(returnStmtNode, maketreeWithVal(KWDTYPE, RETURN));
		 $$ = returnStmtNode;
		}
		|KWD_RETURN expression SEMICLN        
		{
		 tree *returnStmtNode = maketree(RETURNSTMT);
		 addChild(returnStmtNode, maketreeWithVal(KWDTYPE, RETURN));
		 addChild(returnStmtNode, $2);
		 $$ = returnStmtNode;
		}
		;
var		:ID	
		{
		 tree *varNode = maketree(VAR);
		 addChild(varNode, maketreeWithString(IDENTIFIER, $1));
		 $$ = varNode;
		}
		|ID LSQ_BRKT addExpr RSQ_BRKT
		{
		 tree *varNode = maketree(VAR);
		 addChild(varNode, maketreeWithString(IDENTIFIER, $1));
		 addChild(varNode, $3);
		 $$ = varNode;
		}
		;
expression 	:addExpr
		{
		 tree *exprNode = maketree(EXPRESSION);
		 addChild(exprNode, $1);
		 $$ = exprNode;
		}
		|expression relop addExpr
		{
		 tree *exprNode = maketree(EXPRESSION);
		 addChild(exprNode, $1);
		 addChild(exprNode, $2);
		 addChild(exprNode, $3);
		 $$ = exprNode;
		}
 		;
relop           :OPER_LT
		{
		 $$ = maketreeWithVal(OPERATOR, LESSTHAN);
		}
                |OPER_GT
		{
		 $$ = maketreeWithVal(OPERATOR, GREATERTHAN);
		}
                |OPER_GTE
		{
		 $$ = maketreeWithVal(OPERATOR, GREATERTHANEQUAL);
		}
                |OPER_LTE
		{
		 $$ = maketreeWithVal(OPERATOR, LESSTHANEQUAL);
		}
                |OPER_EQ
		{
		 $$ = maketreeWithVal(OPERATOR, EQUALEQUAL);
		}
                |OPER_NEQ
		{
		 $$ = maketreeWithVal(OPERATOR, NOTEQUAL);
		}
                ;
addExpr		:term
		{
		 tree *expressionNode = maketree(ADDEXPR);
		 addChild(expressionNode, $1);
		 $$ = expressionNode;
		}
		|addExpr addop term
		{
		 tree *expressionNode = maketree(ADDEXPR);
		 addChild(expressionNode, $1);
		 addChild(expressionNode, $2);
		 addChild(expressionNode, $3);
		 $$ = expressionNode;
		}
		;
addop		:OPER_ADD    
		{
		 $$ = maketreeWithVal(OPERATOR, ADD);
		}
		|OPER_SUB   
		{  
		 $$ = maketreeWithVal(OPERATOR, SUB);
		} 
		;
term		:factor 
		{
		 tree *termNode = maketree(TERM);
		 addChild(termNode, $1);
		 $$ = termNode;
		}
		|term mulop factor
		{
		 tree *termNode = maketree(TERM);
                 addChild(termNode, $1);
                 addChild(termNode, $2);
                 addChild(termNode, $3);
                 $$ = termNode;
		}
		;
mulop		:OPER_MUL    
		{
		 $$ = maketreeWithVal(OPERATOR, MUL);
		}
		|OPER_DIV    	
		{
		 $$ = maketreeWithVal(OPERATOR, DIV);
		}
		;
factor		:LPAREN expression RPAREN
		{
		 tree *factorNode = maketree(FACTOR);
		 addChild(factorNode, $2);
		 $$ = factorNode;
		}
		|var
		{
		 tree *factorNode = maketree(FACTOR);
                 addChild(factorNode, $1);
                 $$ = factorNode;
		}
		|funcCallExpr
		{
		 tree *factorNode = maketree(FACTOR);
                 addChild(factorNode, $1);
                 $$ = factorNode;
		}
		|NUM
		{
		 tree* factorNode = maketree(FACTOR);
		 addChild(factorNode, maketreeWithVal(INTEGER, $1));
		 $$ = factorNode;
		}	
		|CHAR
		{
		 printf("Inside CHAR\n");
		 tree* factorNode = maketree(FACTOR);
                 addChild(factorNode, maketreeWithVal(CHARACTER, yylval.value));
                 $$ = factorNode;
		}
		|STRING
		{
		 tree* factorNode = maketree(FACTOR);
                 addChild(factorNode, maketreeWithString(STRINGCONS, $1));
                 $$ = factorNode;
		}
		;
funcCallExpr	:ID LPAREN argList RPAREN
		{
         	 tree* funcCallExprNode = maketree(FUNCCALLEXPR);
                 addChild(funcCallExprNode, maketreeWithString(IDENTIFIER, $1));
		 addChild(funcCallExprNode, $3);
                 $$ = funcCallExprNode;
		}
		|ID LPAREN RPAREN
		{
		 tree* funcCallExprNode = maketree(FUNCCALLEXPR);
		 addChild(funcCallExprNode, maketreeWithString(IDENTIFIER, $1));
		 $$ = funcCallExprNode;
		}
		;
argList		:expression 
		{
		 tree * argListNode = maketree(ARGLIST);
		 addChild(argListNode, $1);
		 $$ = argListNode;
		}
		|argList COMMA expression
		{
		 tree * argListNode = maketree(ARGLIST);
		 addChild(argListNode, $1);
		 addChild(argListNode, $3);
		 $$ = argListNode;
		}
		;
%%

void yyerror(char *s)
{
printf("Error: %d: %s\n",yylineno, s);
return;
}

