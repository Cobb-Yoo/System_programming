#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char op[300][10];//operand
char co[300][10];//command
char sy[300][10];//symbol

int sy_c=0;
int co_c=0;
int op_c=0;

void cal(char* bp){
	char *cp;
	int count=0;
	char tmp[4][10];
	
	for(cp = strtok(bp, " \t\n");cp != NULL;){
		strcpy(tmp[count++],cp);
		cp = strtok(NULL, " \t\n");
	}
	
	switch (count){
		case 4:
			strcpy(sy[sy_c++],tmp[1]);
			strcpy(co[co_c++],tmp[2]);
			strcpy(op[op_c++],tmp[3]);
			break;
		case 3:
			strcpy(sy[sy_c++]," ");
			strcpy(co[co_c++],tmp[1]);
			strcpy(op[op_c++],tmp[2]);
			break;
		case 2:
			strcpy(sy[sy_c++]," ");
			strcpy(co[co_c++],tmp[1]);
			strcpy(op[op_c++]," ");
			break;
		default:
			break;
	}
}

int main() {
	 FILE *fp;
	 char buf[80];
	 int i;

	 if ((fp = fopen("sample.s", "r")) == NULL) {
		fprintf(stderr, "file not found...\n");
		exit(1);
	 }

 	while(fgets(buf, sizeof(buf), fp) != NULL) { 
		cal(buf);// 각 배열에 저장. 
 	}

	for(i=0;i<=sy_c;i++){
 		printf("%10s ",sy[i]);
 		if((i+1)%10==0 && i!=0) printf("\n");
	}
	printf("\n---end symbol\n");

	for(i=0;i<=co_c;i++){
 		printf("%10s ",co[i]);
 		if((i+1)%10==0 && i!=0) printf("\n");
	}
	printf("\n---end command\n");

 	for(i=0;i<=op_c;i++){
 		printf("%10s ",op[i]);
 		if((i+1)%10==0 && i!=0) printf("\n");
	}
	printf("\n---end operand\n");
	
	fclose(fp);
}

