#include <stdio.h>
#include <string.h>

static const char* M[1][2] = {
/* S */ { "aS", "n" }  // cols: a, $
};

int col(char t){ return (t=='a') ? 0 : (t=='$') ? 1 : -1; }
int is_term(char x){ return (x=='a' || x=='$'); }

int main(void){
    char input[256], stack[256];
    printf("Enter a-string (empty allowed): ");
    if(!fgets(input,sizeof input,stdin)) return 0;
    input[strcspn(input,"\r\n")] = 0;   // strip newline
    strcat(input,"$");

    int top=0, ip=0;                    // init stack: $ S
    stack[top++]='$';
    stack[top++]='S';

    printf("\n%-12s%-12s\n","STACK","INPUT");
    while(top>0){
        for(int k=0;k<top;k++) putchar(stack[k]);
        printf("%-12s","");  printf("%s\n", input+ip);

        char X=stack[top-1], a=input[ip];

        if(X=='$' && a=='$'){ puts("\nSUCCESS"); break; }

        if(is_term(X)){                   // terminal must match
            if(X==a){ top--; ip++; } else { puts("ERROR"); return 0; }
            continue;
        }

        // X == 'S' here
        int c=col(a); if(c<0){ puts("ERROR"); return 0; }
        const char* rhs = M[0][c];

        top--;                            // pop S
        if(rhs[0]=='n'){ /* epsilon: push nothing */ }
        else {                            // push "aS" in reverse -> 'S','a'
            for(int i=(int)strlen(rhs)-1;i>=0;--i) stack[top++]=rhs[i];
        }
    }
    return 0;
}