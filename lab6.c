#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAX 256
int is_op(char c){ return (c=='+'||c=='-'||c=='*'||c=='/'); }
int prec(char c){ return (c=='+'||c=='-')?1:(c=='*'||c=='/')?2:0; }
void infix_to_postfix(const char*in,char*out){
    char stk[MAX]; int top=-1,i=0,k=0;
    while(in[i]){
        if(isspace((unsigned char)in[i])){ i++; continue; }
        if(isalpha((unsigned char)in[i])||in[i]=='_'||isdigit((unsigned char)in[i])){
            while(isalpha((unsigned char)in[i])||isdigit((unsigned char)in[i])||in[i]=='_') out[k++]=in[i++];
            out[k++]=' '; continue;
        }
        if(in[i]=='('){ stk[++top]=in[i++]; continue; }
        if(in[i]==')'){ while(top>=0&&stk[top]!='('){ out[k++]=stk[top--]; out[k++]=' '; } if(top>=0) top--; i++; continue; }
        if(is_op(in[i])){
            while(top>=0 && is_op(stk[top]) && prec(stk[top])>=prec(in[i])){ out[k++]=stk[top--]; out[k++]=' '; }
            stk[++top]=in[i++]; continue;
        } i++;
    } while(top>=0){ if(stk[top]!='('){ out[k++]=stk[top]; out[k++]=' '; } top--; } out[k]=0;
}
void gen_tac(const char*lhs,const char*post){
    char st[MAX][32]; int sp=0; int t=1, q=1; char tok[64]; int i=0;
    printf("\nThree-Address Code:\n");
    while(sscanf(post+i,"%63s",tok)==1){
        i += (int)strcspn(post+i," ")+1;
        if(strlen(tok)==1 && is_op(tok[0])){
            char b[32],a[32],temp[32]; if(sp<2){ puts("ERROR"); return; }
            strcpy(b,st[--sp]); strcpy(a,st[--sp]); snprintf(temp,sizeof temp,"t%d",t++);
            printf("%-2d) %s = %s %c %s\n", q++, temp, a, tok[0], b);
            strcpy(st[sp++], temp);
        }else strcpy(st[sp++], tok);
    }
    if(sp>0) printf("%-2d) %s = %s\n", q++, lhs, st[sp-1]);
}
int main(void){
    char line[MAX]; printf("Enter assignment (e.g., x=a+b*c): ");
    if(!fgets(line,sizeof line,stdin)) return 0;
    char *eq=strchr(line,'='); if(!eq){ puts("Bad input"); return 0; }
    char lhs[64], rhs[MAX]; int i=0,j=0;
    while(line[i] && !isspace((unsigned char)line[i]) && line[i]!='=') lhs[j++]=line[i++]; lhs[j]=0;
    eq++; while(*eq && isspace((unsigned char)*eq)) eq++; strncpy(rhs,eq,sizeof rhs-1); rhs[sizeof rhs-1]=0;
    for(int k=(int)strlen(rhs)-1;k>=0 && isspace((unsigned char)rhs[k]);--k) rhs[k]=0;
    char post[MAX]={0}; infix_to_postfix(rhs,post);
    printf("\nRHS (postfix): %s\n", post[0]?post:"<empty>"); gen_tac(lhs,post); return 0;
}
