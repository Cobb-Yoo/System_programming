#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct OPTAB{
	char name[8];
	int len;
}Wordtab[]={{"LDA",3},{"STA",4},{"TIX",2},{"STX",3},{"LDX",3}};

int count =0;
int total=0;

void getLine(char line[]){
	int i;
	int flag=0;
	
	for(i=0;i<5;i++){
		if(!strcmp(Wordtab[i].name,line)){
			count++;
			printf("%d, ",count);
			printf("%.2x, ",total);
			printf("%s, ",line);
			printf("%.2d\n",Wordtab[i].len);
			
			total+=Wordtab[i].len;
			
			flag=1;
		}
	}
	
	if(flag != 1){
		printf("Undefined word",line);
	}
}

main(){
	FILE *f = fopen("sample.s","r");
	if(f==NULL){
		printf("ERROR\n");
	}
	
	char line[80];
	
	while(!feof(f)){
		fscanf(f,"%s",line);
		getLine(line);
	}
	
	fclose(f);
}
