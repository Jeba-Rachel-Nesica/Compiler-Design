#include <stdio.h>
#include <string.h>
#define MAXSYM 100
typedef struct { char name[32]; char type[16]; int addr; } Symbol;
Symbol table[MAXSYM]; int count=0;

int find(const char*name){ for(int i=0;i<count;i++) if(strcmp(table[i].name,name)==0) return i; return -1; }
void insert(const char*name,const char*type,int addr){
    if(count>=MAXSYM){ puts("Table full!"); return; }
    if(find(name)!=-1){ puts("Duplicate!"); return; }
    strcpy(table[count].name,name); strcpy(table[count].type,type); table[count].addr=addr; count++; puts("Inserted.");
}
void lookup(const char*name){
    int i=find(name); if(i==-1) puts("Not found.");
    else printf("FOUND -> name=%s, type=%s, addr=%d\n", table[i].name, table[i].type, table[i].addr);
}
void update(const char*name,const char*type,int addr){
    int i=find(name); if(i==-1){ puts("Not found."); return; }
    strcpy(table[i].type,type); table[i].addr=addr; puts("Updated.");
}
void delete(const char*name){
    int i=find(name); if(i==-1){ puts("Not found."); return; }
    for(int j=i;j<count-1;j++) table[j]=table[j+1]; count--; puts("Deleted.");
}
void display(void){
    if(count==0){ puts("Table empty."); return; }
    printf("\nNo.  Name                 Type       Addr\n");
    for(int i=0;i<count;i++) printf("%-4d %-20s %-10s %-6d\n", i, table[i].name, table[i].type, table[i].addr);
}
int main(void){
    int ch,addr; char name[32],type[16];
    for(;;){
        printf("\n1.Insert 2.Lookup 3.Update 4.Delete 5.Display 0.Exit\nChoice: ");
        if(scanf("%d",&ch)!=1) break; if(ch==0) break;
        switch(ch){
            case 1: printf("name type addr: "); scanf("%31s %15s %d",name,type,&addr); insert(name,type,addr); break;
            case 2: printf("name: "); scanf("%31s",name); lookup(name); break;
            case 3: printf("name new_type new_addr: "); scanf("%31s %15s %d",name,type,&addr); update(name,type,addr); break;
            case 4: printf("name: "); scanf("%31s",name); delete(name); break;
            case 5: display(); break;
            default: puts("Invalid.");
        }
    } return 0;
}

