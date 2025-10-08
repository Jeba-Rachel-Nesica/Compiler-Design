#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAX 256
int is_op(char c){ return (c=='+'||c=='-'||c=='*'||c=='/'); }
int prec(char c){ return (c=='+'||c=='-')?1:(c=='*'||c=='/')?2:0; }
void infix_to_postfix(const char*in,char*out){
    char st[MAX]; int top=-1,i=0,k=0;
    while(in[i]){
        if(isspace((unsigned char)in[i])){ i++; continue; }
        if(isalpha((unsigned char)in[i])||in[i]=='_'||isdigit((unsigned char)in[i])){
            while(isalpha((unsigned char)in[i])||isdigit((unsigned char)in[i])||in[i]=='_') out[k++]=in[i++];
            out[k++]=' '; continue;
        }
        if(in[i]=='('){ st[++top]=in[i++]; continue; }
        if(in[i]==')'){ while(top>=0&&st[top]!='('){ out[k++]=st[top--]; out[k++]=' '; } if(top>=0) top--; i++; continue; }
        if(is_op(in[i])){ while(top>=0&&is_op(st[top])&&prec(st[top])>=prec(in[i])){ out[k++]=st[top--]; out[k++]=' '; } st[++top]=in[i++]; continue; }
        i++;
    } while(top>=0){ if(st[top]!='('){ out[k++]=st[top]; out[k++]=' '; } top--; } out[k]=0;
}
void gen_target(const char*lhs,const char*post){
    char st[MAX][32]; int sp=0; int tid=1; char tok[64]; int i=0;
    printf("\n; --- Target Code (Accumulator) ---\n");
    while(sscanf(post+i,"%63s",tok)==1){
        i += (int)strcspn(post+i," ")+1;
        if(strlen(tok)==1 && is_op(tok[0])){
            if(sp<2){ puts("; ERROR"); return; }
            char b[32],a[32],T[32]; strcpy(b,st[--sp]); strcpy(a,st[--sp]); snprintf(T,sizeof T,"T%d",tid++);
            printf("LOAD %s\n", a);
            switch(tok[0]){ case '+': printf("ADD %s\n", b); break; case '-': printf("SUB %s\n", b); break;
                            case '*': printf("MUL %s\n", b); break; case '/': printf("DIV %s\n", b); break; }
            printf("STORE %s\n", T);
            strcpy(st[sp++], T);
        }else strcpy(st[sp++], tok);
    }
    if(sp==1){ printf("LOAD %s\n", st[0]); printf("STORE %s\n", lhs); }
}
int main(void){
    char line[MAX]; printf("Enter assignment (e.g., x = a + b * c): ");
    if(!fgets(line,sizeof line,stdin)) return 0;
    char *eq=strchr(line,'='); if(!eq){ puts("Bad input"); return 0; }
    char lhs[64], rhs[MAX]; int i=0,j=0;
    while(line[i] && isspace((unsigned char)line[i])) i++;
    while(line[i] && line[i] != '=' && j < (int)sizeof(lhs)-1) lhs[j++] = line[i++];
    while(j>0 && isspace((unsigned char)lhs[j-1])) j--; lhs[j]=0;
    eq++; while(*eq && isspace((unsigned char)*eq)) eq++; strncpy(rhs,eq,sizeof rhs-1); rhs[sizeof rhs-1]=0;
    for(int k=(int)strlen(rhs)-1;k>=0 && isspace((unsigned char)rhs[k]); --k) rhs[k]=0;
    char post[MAX]={0}; infix_to_postfix(rhs,post);
    printf("\n; Infix RHS: %s\n; Postfix  : %s\n", rhs, post[0]?post:"<empty>");
    gen_target(lhs,post); return 0;
}

