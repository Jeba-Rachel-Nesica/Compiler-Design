#include <stdio.h>
#include <string.h>

int main(void){
    char line[512];
    printf("Enter production (e.g., A->abcX|abcY|abcZ): ");
    if(!fgets(line,sizeof(line),stdin)) return 0;

    char A = line[0];
    char *arrow = strstr(line,"->");
    if(!arrow){ puts("Bad input"); return 0; }

    // Split RHS by '|'
    char rhs[512]; strcpy(rhs, arrow+2);
    rhs[strcspn(rhs,"\r\n")] = 0;

    char* alts[64]; int n=0;
    char *p = strtok(rhs,"|");
    while(p && n<64){ alts[n++] = p; p = strtok(NULL,"|"); }
    if(n < 2){ puts("Need at least two alternatives"); return 0; }

    // Find longest common prefix among all alts
    int prefix = 0, ok = 1;
    while(ok){
        char c = alts[0][prefix];
        if(!c){ break; }
        for(int i=1;i<n;i++){
            if(alts[i][prefix] != c){ ok = 0; break; }
        }
        if(ok) prefix++;
    }

    if(prefix == 0){
        // Nothing to factor
        printf("%c->%s", A, alts[0]);
        for(int i=1;i<n;i++) printf("|%s", alts[i]);
        printf("\n");
        return 0;
    }

    // Print factored form
    printf("%c->%.*s%c'\n", A, prefix, alts[0], A);
    printf("%c'->", A);
    for(int i=0;i<n;i++){
        const char* tail = alts[i] + prefix;
        if(*tail == 0) printf("ε"); else printf("%s", tail);
        if(i != n-1) printf("|");
    }
    printf("\n");
    return 0;
}