%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokendef.h"

int c,c1, c2, i=0, yylval=0, yycol=0;
char ip, str_buf[1024];

%}

identifier [a-zA-Z][a-zA-Z0-9]* 
zero [0]
integer [1-9]+[0-9]*
integerconstant {zero}|{integer}
whitespace [ \t] 
newline \n

yylineno = 1;

%%

if { yycol = yycol + yyleng; return KWD_IF; }
else { yycol = yycol + yyleng; return KWD_ELSE; }
while { yycol = yycol + yyleng; return KWD_WHILE; }
int { yycol = yycol + yyleng; return KWD_INT; }
string { yycol = yycol + yyleng; return KWD_STRING; }
char { yycol = yycol + yyleng; return KWD_CHAR; }
return { yycol = yycol + yyleng; return KWD_RETURN; }
void { yycol = yycol + yyleng; return KWD_VOID; }

"+" { yycol = yycol + yyleng; return OPER_ADD; }
"-" { yycol = yycol + yyleng; return OPER_SUB; }
"*" { yycol = yycol + yyleng; return OPER_MUL; }
"/" { yycol = yycol + yyleng; return OPER_DIV; }
"<" { yycol = yycol + yyleng; return OPER_LT; }
">" { yycol = yycol + yyleng; return OPER_GT; }
">=" { yycol = yycol + yyleng; return OPER_GTE; }
"<=" { yycol = yycol + yyleng; return OPER_LTE; }
"==" { yycol = yycol + yyleng; return OPER_EQ; }
"!=" { yycol = yycol + yyleng; return OPER_NEQ; }
"=" { yycol = yycol + yyleng; return OPER_ASGN; }

"[" { yycol = yycol + yyleng; return LSQ_BRKT; }
"]" { yycol = yycol + yyleng; return RSQ_BRKT; }
"{" { yycol = yycol + yyleng; return LCRLY_BRKT; }
"}" { yycol = yycol + yyleng; return RCRLY_BRKT; }
"(" { yycol = yycol + yyleng; return LPAREN; }
")" { yycol = yycol + yyleng; return RPAREN; }

"," { yycol = yycol + yyleng; return COMMA; }
";" { yycol = yycol + yyleng; return SEMICLN; }

{newline} { yycol = 1; yylineno++; }

{whitespace} { yycol++; }

{identifier} { yycol = yycol + yyleng; return ID; }

{integerconstant} { yylval = atoi(yytext); yycol = yycol + yyleng; return INTCONST; }

"'" {
	c1 = input();
	yycol++;
	ip = (char)c;
	c2 = input();
	yycol++;
	if( c == '\'' ){
		return CHARCONST;
	}
	else{
		unput(c1);
		unput(c2);
		printf("\n\nImproper character constant termination at Line:%d Column:%d\n\n",yylineno, yycol);
		yylineno++;
		yycol = 1;
	}
   }
 
\" {
	i = 0;
        c = input();
        yycol++;
        for(i=0 ;i<1024 ;i++ ){
                if(c == '\"'){
                        str_buf[i]='\0';
                        return STRCONST;
                }
                else if(c == '\n'){
			printf("\n\nIllegal String termination at Line:%d Columnn:%d\n\n",yylineno,yycol);
			yylineno++;
			yycol = 1;
			break;
		}
                else if(c == '\\'){
                        c = input();
                        yycol++;
                        if(c == 't')
                                str_buf[i] = '\t';
                        else if(c == 'n')
                                str_buf[i] = '\n';
                        else if(c == '"')
                                str_buf[i] = '\"';
                        else if(c == '\\')
                                str_buf[i] = '\\';
                }
                else
                        str_buf[i] = (char)c;
                c = input();
		yycol++;
        }
   }

"/*" {
	while(1){
		while(((c = input()) != '*')&&(c != EOF)){
			if(c == '\n')
				continue;
		}
		if(c == '*'){
			c = input();
			if(c != '/') continue;
			else 	break;
		}
		if(c == EOF){
			printf("\n\nUnterminated comment at Line:%d\n\n",yylineno);
			break;
		} 
	}
     }

. {  printf("\n\n<%s , ILLEGAL TOKEN at Line:%d Column:%d\t>\n\n", yytext,yylineno,yycol); yycol++; }

%%

