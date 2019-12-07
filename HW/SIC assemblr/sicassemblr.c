#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct typedef OPTAB{
	char opcode[10];
	char type[10];
	int mCode;//machine code
}OPTAB;

struct typedef SYMTAB{
	char symbol[10];
	int value;
}SYMTAB;

int LOCCTR=0; // location counter


void pass1(){
	FILE *f = fopen("sample.s","r");
	char buf[100];
	char *tmp_a;
	char tmp[4][10];
	int count;
	
	if(f==NULL){
		printf("해당하는 파일이 없습니다.\n");
		exit(1);
	}
	
	while(fgets(buf,sizeof(buf),f) != NULL)	{ // 파일을 하나씩 읽는거 
		count = 0;
		for(tmp_a = strtok(buf," \t\n");cp != NULL;){
			strcpy(tmp[count++],tmp_a);
			cp = strtok(NULL, " \t\n");
		}
		
		switch(count){
			case 4:
				strcpy();
				break;
			case 3:
			case 2:
			default:
				break;
		}
	}
}

void pass2(){
}


main(){
	pass1();
	pass2();
	
	inToTheUnkwon();
}
