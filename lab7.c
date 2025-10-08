#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define MAXN 200
#define MAXS 32
typedef struct { char res[MAXS]; char op; char a1[MAXS], a2[MAXS]; int alive; } TAC;
static TAC code[MAXN]; static int n=0;
int is_number(const char*s){ if(!*s) return 0; int i=0; if(s[i]=='+'||s[i]=='-') i++; if(!isdigit((unsigned char)s[i])) return 0; for(;s[i];++i) if(!isdigit((unsigned char)s[i])) return 0; return 1; }
long to_long(const char*s){ return strtol(s,NULL,10); }
int is_temp(const char*s){ return s[0]=='t' && isdigit((unsigned char)s[1]); }
void trim(char*s){ int i=0,j=(int)strlen(s)-1; while(i<=j && isspace((unsigned char)s[i])) i++; while(j>=i && isspace((unsigned char)s[j])) j--; memmove(s,s+i,j-i+1); s[j-i+1]=0; }
int parse_line(const char*line,TAC*t){
    char buf[256]; strcpy(buf,line); char*eq=strchr(buf,'=');
    if(!eq) return 0; *eq=0; trim(buf); trim(eq+1);
    strncpy(t->res,buf,MAXS-1); t->res[MAXS-1]=0;
    char rhs[256]; strcpy(rhs,eq+1); char x1[64]={0}, xop[4]={0}, x2[64]={0};
    int k=sscanf(rhs," %63s %3s %63s",x1,xop,x2);
    if(k==1){ t->op='='; strncpy(t->a1,x1,MAXS-1); t->a1[MAXS-1]=0; t->a2[0]=0; t->alive=1; return 1; }
    else if(k==3 && strlen(xop)==1 && strchr("+-*/",xop[0])){
        t->op=xop[0]; strncpy(t->a1,x1,MAXS-1); t->a1[MAXS-1]=0; strncpy(t->a2,x2,MAXS-1); t->a2[MAXS-1]=0; t->alive=1; return 1;
    } return 0;
}
void fold_and_simplify(void){
    for(int i=0;i<n;i++){ TAC*t=&code[i]; if(!t->op) continue;
        if(strchr("+-*/",t->op)&&is_number(t->a1)&&is_number(t->a2)){
            long x=to_long(t->a1),y=to_long(t->a2),r=0;
            switch(t->op){ case '+':r=x+y;break; case '-':r=x-y;break; case '*':r=x*y;break; case '/': if(y!=0) r=x/y; else continue; }
            t->op='='; snprintf(t->a1,sizeof t->a1,"%ld",r); t->a2[0]=0; continue;
        }
        if(t->op=='+'){ if(is_number(t->a1)&&!strcmp(t->a1,"0")){ t->op='='; strcpy(t->a1,t->a2); t->a2[0]=0; }
                         else if(is_number(t->a2)&&!strcmp(t->a2,"0")){ t->op='='; t->a2[0]=0; } }
        else if(t->op=='-'){ if(is_number(t->a2)&&!strcmp(t->a2,"0")){ t->op='='; t->a2[0]=0; } }
        else if(t->op=='*'){ if((is_number(t->a1)&&!strcmp(t->a1,"0"))||(is_number(t->a2)&&!strcmp(t->a2,"0"))){ t->op='='; strcpy(t->a1,"0"); t->a2[0]=0; }
                              else if(is_number(t->a1)&&!strcmp(t->a1,"1")){ t->op='='; strcpy(t->a1,t->a2); t->a2[0]=0; }
                              else if(is_number(t->a2)&&!strcmp(t->a2,"1")){ t->op='='; t->a2[0]=0; }
                              else if(is_number(t->a2)&&!strcmp(t->a2,"2")){ t->op='+'; strcpy(t->a2,t->a1); }
                              else if(is_number(t->a1)&&!strcmp(t->a1,"2")){ t->op='+'; strcpy(t->a1,t->a2); } }
        else if(t->op=='/'){ if(is_number(t->a2)&&!strcmp(t->a2,"1")){ t->op='='; t->a2[0]=0; } }
    }
}
void make_key(const TAC*t,char*out){
    char A[MAXS],B[MAXS]; strcpy(A,t->a1); strcpy(B,t->a2);
    if(t->op=='+'||t->op=='*'){ if(strcmp(A,B)>0){ char tmp[MAXS]; strcpy(tmp,A); strcpy(A,B); strcpy(B,tmp);} }
    snprintf(out,3*MAXS,"%c %s %s",t->op,A,B);
}
void common_subexpr_elim(void){
    typedef struct { char key[3*MAXS]; char temp[MAXS]; } Entry;
    Entry seen[MAXN]; int m=0;
    for(int i=0;i<n;i++){ TAC*t=&code[i]; if(!t->op||t->op=='=') continue;
        char key[3*MAXS]; make_key(t,key); int f=-1; for(int j=0;j<m;j++) if(strcmp(seen[j].key,key)==0){f=j;break;}
        if(f==-1){ strcpy(seen[m].key,key); strcpy(seen[m].temp,t->res); m++; }
        else { t->op='='; strcpy(t->a1,seen[f].temp); t->a2[0]=0; }
    }
}
int is_temp_name(const char*s){ return s[0]=='t' && isdigit((unsigned char)s[1]); }
void dead_code_elim(void){
    typedef struct { char name[MAXS]; int cnt; } Use; Use uses[MAXN]; int u=0;
    for(int i=0;i<n;i++){ TAC*t=&code[i]; if(!t->op) continue;
        char const* list1[] = { t->op=='='?t->a1:t->a1, t->op=='='?"":t->a2, NULL };
        for(int j=0; list1[j]; ++j){ const char* s=list1[j]; if(!*s) continue;
            int k=-1; for(int v=0;v<u;v++) if(strcmp(uses[v].name,s)==0){ k=v; break; }
            if(k==-1){ strcpy(uses[u].name,s); uses[u].cnt=1; u++; } else uses[k].cnt++; }
    }
    for(int i=0;i<n;i++){ TAC*t=&code[i]; if(!t->op) continue;
        if(!is_temp_name(t->res)){ t->alive=1; continue; }
        int used=0; for(int v=0;v<u;v++) if(strcmp(uses[v].name,t->res)==0){ used=1; break; }
        t->alive = used;
    }
}
void print_code(const char*title){
    printf("\n%s\n", title);
    for(int i=0;i<n;i++){ TAC*t=&code[i]; if(!t->op||!t->alive) continue;
        if(t->op=='=') printf("%s = %s\n", t->res, t->a1);
        else printf("%s = %s %c %s\n", t->res, t->a1, t->op, t->a2);
    }
}
int main(void){
    char line[256]; puts("Enter TAC one per line (e.g., t1=a+b). Blank line to end:");
    while(n<MAXN && fgets(line,sizeof line,stdin)){ if(line[0]=='\n'||line[0]=='\r') break;
        TAC t={0}; if(parse_line(line,&t)) code[n++]=t; else puts("! Skipped (bad line)");
    }
    for(int i=0;i<n;i++) code[i].alive=1;
    print_code("---- Original ----"); fold_and_simplify(); common_subexpr_elim(); dead_code_elim(); print_code("---- Optimized ----"); return 0;
}
