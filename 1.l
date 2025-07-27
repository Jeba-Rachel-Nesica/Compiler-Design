%{
#include <stdio.h>
int spaces = 0, tabs = 0, newlines = 0;
%}

%%
" "      { spaces++; }
"\t"     { tabs++; }
"\n"     { newlines++; }
.        ; // ignore other characters
%%

int yywrap() { return 1; }

int main() {
    printf("Enter text (Ctrl+D to end):\n");
    yylex();
    printf("Spaces: %d\nTabs: %d\nNewlines: %d\n", spaces, tabs, newlines);
    return 0;
}
