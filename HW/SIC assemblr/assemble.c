#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define OPLEN 18

struct OPTAB{
	char name[10];
	int opcode;
}OPTAB[]={{"STL",0x14},{"JSUB",0x48},{"LDA",0x00},{"COMP",0x28},{"JEQ",0x30},{"J",0x3c},{"STA",0x0c},{"LDL",0x08},{"RSUB",0x4c},{"LDX",0x04},{"TD",0xE0},{"RD",0xD8},{"STCH",0x54},{"TIX",0x2c},{"JLT",0x38},{"STX",0x10},{"LDCH",0x50},{"WD",0xdc}};

typedef struct SYMTAB{
	char symbol[10]; //pass1���� symbol ���� 
	
	int address;
}SYMTAB;

int LOCCTR; // location counter (pc)

SYMTAB symTAB[100];

int check_SYM(char *test,int n){
// symbol ���̺� �̹� ����Ǿ� �ִ� symbol�� �ִ��� Ȯ��. 
// ���̺� ���� ��ġ�� symbol�� �ִٸ� return 0;
// ��ġ�°� ���ٸ� return 1; 
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
//pass1���� symbol ���̺�� operand ���̺��� �ϼ���Ű�� ���α׷� ��ü ���̸� �����ϴ� �Լ� 
	FILE *f = fopen("sample.s","r");
	FILE *s = fopen("data.o","w");
	char buf[100]; // ���Ͽ��� �� ���� ���� �迭 
	char *tmp_a; //buf���� " \t\n"���� strtok�Ͽ� �߶� ���ڿ��� �����ϴ� ���� 
	char tmp[4][10]; // tmp[������ �з��ϴ� �κ�][���ڿ� ����]
	int count=0; // ������ �з��ϴ� ���� 
	int sy=0; // symbol ���̺��� ����
	int op=0; // operand ���̺��� ����
	int i;
	int num;
	
	if(f==NULL){ 
		printf("�ش��ϴ� ������ �����ϴ�.\n");
		exit(1);
	}
	
	//�� ó�� �κ��� copy start locctr���� Ȯ���ϴ� �ڵ带 ���� == �ߺ��� ���Ϸ���
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
	
	while(fgets(buf,sizeof(buf),f) != NULL)	{ // ���� ������ �����͸� 100���� �д°�
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
					//�ּ�ó���� �ؾ��մϴ�. 
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
