#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define OPLEN 18

struct OPTAB{
	char name[10];
	int opcode;
}OPTAB[]={{"STL",0x14},{"JSUB",0x48},{"LDA",0x00},{"COMP",0x28},{"JEQ",0x30},{"J",0x3c},{"STA",0x0c},{"LDL",0x08},{"RSUB",0x4c},{"LDX",0x04},{"TD",0xE0},{"RD",0xD8},{"STCH",0x54},{"TIX",0x2c},{"JLT",0x38},{"STX",0x10},{"LDCH",0x50},{"WD",0xdc}};

typedef struct SYMTAB{
	char symbol[10]; //pass1에서 symbol 저장 
	
	int address;
}SYMTAB;

int LOCCTR; // location counter (pc)

SYMTAB symTAB[100];

int check_SYM(char *test,int n){
// symbol 테이블에 이미 저장되어 있는 symbol이 있는지 확인. 
// 테이블 내에 겹치는 symbol이 있다면 return 0;
// 겹치는게 없다면 return 1; 
	int i;
	for(i=0;i<n;i++){
		if(!strcmp(symTAB[i].symbol,test)) return 0;
	}
	return 1;
}

void cal_LOCCTR_A(int *n,int num, char tmp[][10]){
	int i;
	int tmp_num; 
	for(i=0;i<OPLEN;i++){
		if(!strcmp(OPTAB[i].name,tmp[2])){
			LOCCTR += 0x3;
			symTAB[(*n)++].address = LOCCTR;
			return;
		}
	}	
	
	if(!strcmp("BYTE",tmp[2])){
		//LOCCTR += ;
		//symTAB[(*n)++].address = LOCCTR;
	}
	else if(!strcmp("WORD",tmp[2])){	
		LOCCTR += 0x3;
		symTAB[(*n)++].address = LOCCTR;
	}
	else if(!strcmp("RESW",tmp[2])){
		LOCCTR += strtol(tmp[3],NULL,16) * 3;
		symTAB[(*n)++].address = LOCCTR;
	}
	else if(!strcmp("RESB",tmp[2])){
		LOCCTR += strtol(tmp[3],NULL,16);
		symTAB[(*n)++].address = LOCCTR;
	}
}
void cal_LOCCTR_B(){
	LOCCTR += 0x3;
}
void cal_LOCCTR_C(){
	LOCCTR += 0x3;
}

void pass1(){
//pass1으로 symbol 테이블과 operand 테이블을 완성시키고 프로그램 전체 길이를 측정하는 함수 
	FILE *f = fopen("sample.s","r");
	FILE *s = fopen("data.o","w");
	char buf[100]; // 파일에서 한 줄을 받을 배열 
	char *tmp_a; //buf에서 " \t\n"으로 strtok하여 잘라낸 문자열을 저장하는 변수 
	char tmp[4][10]; // tmp[종류를 분류하는 부분][문자열 길이]
	int count=0; // 종류를 분류하는 변수 
	int sy=0; // symbol 테이블의 갯수
	int op=0; // operand 테이블의 갯수
	int i;
	int num;
	
	if(f==NULL){ 
		printf("해당하는 파일이 없습니다.\n");
		exit(1);
	}
	
	//맨 처음 부분이 copy start locctr인지 확인하는 코드를 삽입 == 중복을 피하려고
	fgets(buf,sizeof(buf),f);
	for(tmp_a = strtok(buf," \t\n");tmp_a != NULL;){
		strcpy(tmp[count++],tmp_a);
		tmp_a = strtok(NULL, " \t\n");
	}
	if(!strcmp(tmp[1],"COPY")){
		if(!strcmp(tmp[2],"START")) LOCCTR = strtol(tmp[3],NULL,16);
		else{
			printf("ERROR in FIRST LINE...(line 83)\n");
			exit(1);
		}
	}
	
	else{
		printf("ERROR in FIRST LINE...(line 88)\n");
		exit(1);
	}
	//=================================
	
	while(fgets(buf,sizeof(buf),f) != NULL)	{ // 파일 내부의 데이터를 100개씩 읽는거
		count = 0;
		for(tmp_a = strtok(buf," \t\n");tmp_a != NULL;){
			strcpy(tmp[count++],tmp_a);
			tmp_a = strtok(NULL, " \t\n");
		}
		
		if(check_SYM(tmp[1],sy) == 0) {
			printf("ERROR in symbol checking(same symbol in table)...(line 101)\n");
			exit(1);
		}
		
		switch(count){
			case 4:
				strcpy(symTAB[sy].symbol,tmp[1]);
				cal_LOCCTR_A(&sy,num,tmp);
				printf("%x\n",LOCCTR);
				break;
			case 3:
				cal_LOCCTR_B();
				break;
			case 2:
				if('.' == tmp[1]){
					//주석처리를 해야합니다. 
				}
				else{
					cal_LOCCTR_C();
				}
				break;
			default:
				break;
		}
	}
	
	fclose (f);
	fclose (s);
}

void pass2(){
}

main(){
	pass1();
	pass2();
	
	printf("%x",LOCCTR);
}
