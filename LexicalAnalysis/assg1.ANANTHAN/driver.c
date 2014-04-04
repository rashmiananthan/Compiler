#include <stdio.h>
#include "tokendef.h"

extern char* yytext;
extern char ip;
extern int yylval, yylineno, yycol;
extern char str_buf[1024];

void printToken(int tokenNum){

  switch(tokenNum){
    case KWD_IF:
      printf("<IF , KEYWORD >\t");
      break;
    case KWD_ELSE:
      printf("<ELSE , KEYWORD>\t");
      break;
    case KWD_WHILE:
      printf("<WHILE , KEYWORD>\t");
      break;
    case KWD_INT:
      printf("<INT , KEYWORD>\t");
      break;
    case KWD_STRING:
      printf("<STRING , KEYWORD>\t");
      break;
    case KWD_CHAR:
      printf("<CHAR , KEYWORD>\t");
      break;
    case KWD_RETURN:
      printf("<RETURN , KEYWORD>\t");
      break;
    case KWD_VOID:
      printf("<VOID , KEYWORD>\t");
      break;
    case OPER_ADD:
      printf("<ADD , OPERATOR>\t");
      break;
    case OPER_SUB:
      printf("<SUB , OPERATOR>\t");
      break;
    case OPER_MUL:
      printf("<MUL , OPERATOR>\t");
      break;
    case OPER_DIV:
      printf("<DIV , OPERATOR>\t");
      break;
    case OPER_LT:
      printf("<LT , OPERATOR>\t");
      break;
    case OPER_GT:
      printf("<GT , OPERATOR>\t");
      break;
    case OPER_GTE:
      printf("<GTE , OPERATOR>\t");
      break;
    case OPER_LTE:
      printf("<LTE , OPERATOR>\t");
      break;
    case OPER_EQ:
      printf("<EQ , OPERATOR>\t");
      break;
    case OPER_NEQ:
      printf("<NEQ , OPERATOR>\t");
      break;
    case OPER_ASGN:
      printf("<ASGN , OPERATOR>\t");
      break;
    case LSQ_BRKT:
      printf("<LSQ_BRKT , BRACKET>\t");
      break;
    case RSQ_BRKT:
      printf("<RSQ_BRKT , BRACKET>\t");
      break;
    case LCRLY_BRKT:
      printf("<LCRLY_BRKT , BRACKET>\t");
      break;
    case RCRLY_BRKT:
      printf("<RCRLY_BRKT , BRACKET>\t");
      break;
    case LPAREN:
      printf("<LPAREN , BRACKET>\t");
      break;
    case RPAREN:
      printf("<RPAREN , BRACKET>\t");
      break;
    case COMMA:
      printf("<COMMA>\t");
      break;
    case SEMICLN:
      printf("<SEMICLN>\t");
      break;
    case ID:
	printf("<%s , IDENTIFIER>\t", yytext);
	break;
    case INTCONST:
	printf("<%d , INTCONST>\t", yylval);
	break;
    case CHARCONST:
	printf("<%c , CHARCONST>\t", ip);
	break;
    case STRCONST:
	printf("<%s , STRCONST>\t", str_buf);
	break;
    case ERROR:
	printf("<%s , ERROR>\t", yytext);
	break;
  }
}

int main() {
  int ret = yylex();
    printf("\n");
  while (ret) {
    printToken(ret);
    ret = yylex();
  }
    printf("\n\n");
  return 0;
}
