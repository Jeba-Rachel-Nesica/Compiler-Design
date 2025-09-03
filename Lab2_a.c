#include <stdio.h>
#include <string.h>

int main(void){
    char line[512];
    printf("Enter production (e.g., E->E+T|T|E-T): ");
    if(!fgets(line,sizeof(line),stdin)) return 0;

    char A = line[0];
    char *arrow = strstr(line,"->");
    if(!arrow){ puts("Bad input"); return 0; }

    char rhs[512]; strcpy(rhs, arrow+2);
    rhs[strcspn(rhs,"\r\n")] = 0;

    // split alts
    char* alpha[64]; int na=0;   // parts after A (for A->Aα)
    char* beta[64];  int nb=0;   // non-left-recursive alts β
    char* p = strtok(rhs,"|");
    while(p){
        if(p[0]==A) alpha[na++] = p+1;  // skip the A
        else        beta[nb++]  = p;
        p = strtok(NULL,"|");
    }

    if(na==0){
        // no immediate left recursion
        printf("%c->", A);
        for(int i=0;i<nb;i++){ printf("%s%s", beta[i], (i==nb-1)?"\n":"|"); }
        return 0;
    }

    // Print transformed grammar:
    // A -> β A'
    // A'-> α A' | ... | ε
    printf("%c->", A);
    for(int i=0;i<nb;i++){
        printf("%s%c'%s", beta[i], A, (i==nb-1)?"\n":"|");
    }
    printf("%c'->", A);
    for(int i=0;i<na;i++){
        printf("%s%c'%s", alpha[i], A, (i==na-1)?"|ε\n":"|");
    }
    return 0;
}