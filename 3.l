%{
#include <stdio.h>
#include <ctype.h>
%}

%%
[A-Z]   { putchar(tolower(yytext[0])); }
[a-z]   { putchar(toupper(yytext[0])); }
.|\n    { putchar(yytext[0]); }
%%

int yywrap() { return 1; }

int main() {
    printf("Enter text (Ctrl+D to end):\n");
    yylex();
    return 0;
}
