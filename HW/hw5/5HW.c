#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int total=0;

void getLine(char line[]){
	int len = strlen(line)-1;
	int i;
	
	printf("%.2X", total);
	printf(": ");
	for(i=0;i<len;i++) printf("%c",line[i]);
	printf("\n");
	
	total+=len;
}

main(){
	FILE *f = fopen("sample.s","r");
	char line[80];
	
	while(fgets(line,80,f)!=NULL){
		getLine(line);
	}
	
	printf("%.2X\n",total);
	
	fclose(f);
}
