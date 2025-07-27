%{
#include <stdio.h>
%}

%%
[0-9]+        { printf("Number: %s\n", yytext); }
[\+\-\*/=]    { printf("Operator: %s\n", yytext); }
[\(\)]        { printf("Parenthesis: %s\n", yytext); }
[ \t\n]       ; // ignore whitespace
%%

int yywrap() { return 1; }

int main() {
    printf("Enter arithmetic expressions (Ctrl+D to end):\n");
    yylex();
    return 0;
}
