%{
#include <stdio.h>
%}

%%
[0-9]+              { printf("Number: %s\n", yytext); }
[a-zA-Z_][a-zA-Z0-9_]* { printf("Word: %s\n", yytext); }
[^a-zA-Z0-9 \t\n]+  { printf("Special Symbol: %s\n", yytext); }
[ \t\n]             ; // ignore whitespace
%%

int yywrap() { return 1; }

int main() {
    printf("Enter input (Ctrl+D to end):\n");
    yylex();
    return 0;
}
