%{
#include <stdio.h>
#include <string.h>

/* very small keyword list to keep it simple */
int is_kw(const char*s){
  const char* kw[] = {"int","float","char","if","else","for","while","return",NULL};
  for(int i=0; kw[i]; ++i) if(strcmp(kw[i],s)==0) return 1;
  return 0;
}
%}

/* ---- macros ---- */
WS    [ \t\r\n]+
ID    [A-Za-z_][A-Za-z0-9_]*
NUM   [0-9]+

%%
/* ignore spaces/newlines */
{WS}                       ;

/* comments */
"//".*                     { printf("COMMENT\n"); }
"/"([^]|\+[^/])"*/"    { printf("COMMENT\n"); }

/* strings (simple) */
\"([^\\\"]|\\.)*\"         { printf("STRING: %s\n", yytext); }

/* identifiers / keywords */
{ID}                       {
                             if(is_kw(yytext)) printf("KEYWORD: %s\n", yytext);
                             else              printf("IDENT: %s\n",   yytext);
                           }

/* numbers */
{NUM}                      { printf("NUMBER: %s\n", yytext); }

/* operators (multi-char first, then single-char) */
"=="|">="|"<="|"!="        { printf("OP: %s\n", yytext); }
"="|"+"|"-"|"*"|"/"|"<"|">"{ printf("OP: %s\n", yytext); }

/* delimiters */
"("|")"|"{"|"}"|"\\["|"\\]"|","|";"  { printf("DELIM: %s\n", yytext); }

/* anything else: ignore (keeps program tiny) */
.                          ;
%%

int yywrap(void){ return 1; }

int main(int argc, char** argv){
  if(argc > 1){
    yyin = fopen(argv[1], "r");
    if(!yyin){ perror("open"); return 1; }
  }else{
    yyin = stdin;        /* type/paste code directly, Ctrl+D to end */
  }
  yylex();
  if(yyin && yyin!=stdin) fclose(yyin);
  return 0;
}