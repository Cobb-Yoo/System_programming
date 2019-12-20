#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define OPLEN 18

struct OPTAB{
	char name[10];
	int opcode;
}OPTAB[]={{"STL",0x14},{"JSUB",0x48},{"LDA",0x00},{"COMP",0x28},{"JEQ",0x30},{"J",0x3C},{"STA",0x0C},{"LDL",0x08},{"RSUB",0x4C},{"LDX",0x04},{"TD",0xE0},{"RD",0xD8},{"STCH",0x54},{"TIX",0x2C},{"JLT",0x38},{"STX",0x10},{"LDCH",0x50},{"WD",0xDC}};

typedef struct SYMTAB{
	char symbol[10]; //pass1에서 symbol 저장 
	
	int address;
}SYMTAB;

SYMTAB symTAB[100];
int SYMLEN;
int filelen;
int END_LOC;
int OP_LOC;
int startop=0;

int opcodelist[10][2];
int opcodelist_i=-1;

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

int cal_LOCCTR_A(int *n, char tmp[][10], int *LOCCTR){
	int i;
	int sum=0;
	int total=0;
	
	for(i=0;i<OPLEN;i++){
		if(!strcmp(OPTAB[i].name,tmp[2])){
			symTAB[(*n)++].address = *LOCCTR;
			*LOCCTR += 0x3;
			return 0x3;
		}
	}	
	
	if(!strcmp("BYTE",tmp[2])){
		switch (tmp[3][0])	{
			case 'X':
			case 'x':
				symTAB[(*n)++].address = *LOCCTR;
				*LOCCTR += 0x1;
				for(i=2;i<strlen(tmp[2])-1;i++){					
					if('0' <= tmp[2][i] && tmp[2][i] <= '9'){ // 숫자일때 
						sum = sum * 16 + (int)(tmp[2][i] - '0');
					}
					else{//문자일때 
						sum = sum * 16 + (int)(tmp[2][i] - 'A' + 10);
					}
				}
				total += sum;
				sum = 0;
				return 0x1;
				
			case 'C':
			case 'c':
				symTAB[(*n)++].address = *LOCCTR;
				*LOCCTR += 0x3;
				for(i=2;i<strlen(tmp[2])-1;i++){					
					sum = sum << 8; //비트연산이 곱셈연산보다 빠르다는 것을 이용함 
					sum |= tmp[2][i]; // 단순한 덧셈으로 하면 값이 다른 것이 나옴. 
				}
				total += sum;
				sum = 0;
				return 0x3;
		}
	}
	else if(!strcmp("WORD",tmp[2])){	
		symTAB[(*n)++].address = *LOCCTR;
		*LOCCTR += 0x3;
		return 0x3;
	}
	else if(!strcmp("RESW",tmp[2])){
		symTAB[(*n)++].address = *LOCCTR;
		*LOCCTR += strtol(tmp[3],NULL,16) * 3;
		return strtol(tmp[3],NULL,16) * 3;
	}
	else if(!strcmp("RESB",tmp[2])){
		symTAB[(*n)++].address = *LOCCTR;
		*LOCCTR += strtol(tmp[3],NULL,10);
		return strtol(tmp[3],NULL,10);
	}
}
int cal_LOCCTR_B(int *LOCCTR){
	*LOCCTR += 0x3;
	return 0x3;
}
int cal_LOCCTR_C(int *LOCCTR){
	*LOCCTR += 0x3;
	return 0x3;
}

int findOP(char *opname){
	int i;
	for(i=0;i<OPLEN;i++){
		if(!strcmp(OPTAB[i].name,opname)){
			//printf("================test opcode %x\n",OPTAB[i].opcode);
			return OPTAB[i].opcode;
		}
	}
}

int findADD(char *address){
	int i;
	
	for(i=0;i<SYMLEN;i++){
			if(!strcmp(symTAB[i].symbol,address)) return symTAB[i].address;
	}
}
int findADD_B(char *command, char *tmp){
	int result=0;
	int i;
	if(!strcmp("BYTE",command)){
		switch (tmp[0]){
			case 'x':
			case 'X':
				for(i=2;i<strlen(tmp)-1;i++){					
					if('0' <= tmp[i] && tmp[i] <= '9'){ // 숫자일때 
						result = result * 16 + (int)(tmp[i] - '0');
					}
					else{//문자일때 
						result = result * 16+ (int)(tmp[i] - 'A' + 10);
					}
				}
				return result;
			case 'c':
			case 'C':
				for(i=2;i<strlen(tmp)-1;i++){
					result = result << 8;
					result |= tmp[i];
				}
				return result;
		}
	}
	else if(!strcmp("WORD",command)){
		return strtol(tmp,NULL,10);
	}
	else if(!strcmp("RESW",command)){
	}
	else if(!strcmp("RESB",command)){
	}
	else{
	}
}

int cal_OPCODE_A(int n, char tmp[][10], int *opcode){
	if(!strcmp("BYTE",tmp[2])){
		*opcode = findADD_B(tmp[2],tmp[3]);
		return 0;
	}
	else if(!strcmp("WORD",tmp[2])){
		*opcode = findADD_B(tmp[2],tmp[3]);
		return 3;	
	}
	else if(!strcmp("RESW",tmp[2])){
		*opcode = findADD_B(tmp[2],tmp[3]);
		return 2;	
	}
	else if(!strcmp("RESB",tmp[2])){
		*opcode = findADD_B(tmp[2],tmp[3]);
		return 4;	
	}
	else {
		*opcode = findOP(tmp[2]) * 0x10000 + findADD(tmp[3]);
		return 1;
	}
}
void cal_OPCODE_B(int n,char tmp[][10],int *opcode){
	*opcode = findOP(tmp[1]) * 0x10000 + findADD(tmp[2]);
}
void cal_OPCODE_C(int n,char tmp[][10],int *opcode){
	*opcode = findOP(tmp[1]) * 0x10000 + 0;
}

void insertFILE(int oplist_len, int opcode, FILE *w,int flag,int address){	
	int i;
	int len=0;
	
	if(opcodelist_i==-1){
		if(startop == 0){
			opcodelist_i = 0;
			startop = oplist_len + END_LOC;
			fprintf(w,"T.%06X",address); //시작주소 
		}
		return;
	}
	
	if(flag== 0) { // 파일이 끝났을 때 
		len = 0;
		for(i=0;i<opcodelist_i;i++){
			len += opcodelist[i][1];
		}
		fprintf(w,".%02X", len); //길이 
		startop += len;
		
		for(i=0;i<opcodelist_i;i++) {
			if(opcodelist[i][0] >> 3 == 0) fprintf(w,".%02X",opcodelist[i][0]);
			else fprintf(w,".%06X",opcodelist[i][0]);
		}
		return;
	}
	else if(flag == 4){// 10되기전에 종료 
		opcodelist[opcodelist_i][0] = opcode;
		for(i=0;i<opcodelist_i;i++){
			len += opcodelist[i][1];
		}
		
		fprintf(w,".%02X",len); //길이 
		
		for(i=0;i<opcodelist_i;i++) {
			if(opcodelist[i][0] >> 3 == 0) fprintf(w,".%02X",opcodelist[i][0]);
			else fprintf(w,".%06X",opcodelist[i][0]);
		}
		fprintf(w,"\nT.%06X",address);
		startop += len; 
		opcodelist_i=0;
		return;                       
	}
	else{ // 정상 종료 
		if (opcodelist_i == 10 ){
			for(i=0;i<opcodelist_i;i++){
				len+= opcodelist[i][1];
			}
			fprintf(w,".%02X",len); //길이
			startop += len;
			
			for(i=0;i<opcodelist_i;i++){
				if(opcodelist[i][0] >> 3 == 0) fprintf(w,".%02X",opcodelist[i][0]);
				else fprintf(w,".%06X",opcodelist[i][0]);
			}
			fprintf(w,"\nT.%06X",address);
			
			opcodelist_i = 0;
			opcodelist[opcodelist_i++][0] = opcode;
		}
		else{
			opcodelist[opcodelist_i++][0] = opcode;
		}
	}
}

void pass1(){
//pass1으로 symbol 테이블과 operand 테이블을 완성시키고 프로그램 전체 길이를 측정하는 함수 
	FILE *f = fopen("sample.s","r");
	char buf[100]; // 파일에서 한 줄을 받을 배열 
	char *tmp_a; //buf에서 " \t\n"으로 strtok하여 잘라낸 문자열을 저장하는 변수 
	char tmp[4][10]; // tmp[종류를 분류하는 부분][문자열 길이]
	int count=0; // 종류를 분류하는 변수 
	int sy=0; // symbol 테이블의 갯수
	int op=0; // operand 테이블의 갯수
	int i;
	int num;
	int temp;
	int LOCCTR;
	
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
		if(!strcmp(tmp[2],"START")) LOCCTR = END_LOC = strtol(tmp[3],NULL,16);
		else{
			printf("ERROR in FIRST LINE...(line 83)\n");
			exit(1);
		}
	}
	
	else{
		printf("ERROR in FIRST LINE...(line 88)\n");
		exit(1);
	}
	
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
				//temp = LOCCTR;
				strcpy(symTAB[sy].symbol,tmp[1]);
				cal_LOCCTR_A(&sy,tmp,&LOCCTR);
				break;
			case 3:
				//temp = LOCCTR;
				if(strcmp(tmp[1],"END")) {
					cal_LOCCTR_B(&LOCCTR);
				}
				else{
				}
				break;
			case 2:
				if(!strcmp(tmp[1],".")){
				}
				else{
					//temp = LOCCTR;
					cal_LOCCTR_C(&LOCCTR);
				}
				break;
			default:
				break;
		}
	}
	
	fclose (f);
	
	SYMLEN = sy;
	filelen = LOCCTR - END_LOC;
}
void pass2(){
	char buf[100]; // 파일에서 한 줄을 받을 배열 
	char *tmp_a; //buf에서 " \t\n"으로 strtok하여 잘라낸 문자열을 저장하는 변수 
	char tmp[4][10]; // tmp[종류를 분류하는 부분][문자열 길이]
	int count=0; // 종류를 분류하는 변수 
	int sy=0; // symbol 테이블의 갯수
	int op=0; // operand 테이블의 갯수
	int i;
	int temp;
	int loc;
	int opcode;
	int flag;
	int oplist_len=0;
		
	FILE *f = fopen("sample.s","r");
	FILE *w = fopen("assemler.o","w");
	
	if(f == NULL){
		printf("ERROR in make file\n");
		exit(1);
	}
	
	fgets(buf,sizeof(buf),f);
	for(tmp_a = strtok(buf," \t\n");tmp_a != NULL;){
		strcpy(tmp[count++],tmp_a);
		tmp_a = strtok(NULL, " \t\n");
	}
	
	OP_LOC = loc = strtol(tmp[3],NULL,16);
	
	printf("%s\t%X\t%s\t%s\t%s\n",tmp[0],loc,tmp[1],tmp[2],tmp[3]);
	fprintf(w,"H.%s\t.%06X.%06X\n",tmp[1],strtol(tmp[3],NULL,16),filelen); // 맨처음에 넣을 것 
	insertFILE(oplist_len,opcode, w,1,strtol(tmp[3],NULL,16));
	
	while(fgets(buf,sizeof(buf),f) != NULL){
		count = 0;
		opcode = 0;
		for(tmp_a = strtok(buf," \t\n");tmp_a != NULL;){
			strcpy(tmp[count++],tmp_a);
			tmp_a = strtok(NULL, " \t\n");
		}
		
		switch(count){
			case 4:
				temp = loc;
				strcpy(symTAB[sy].symbol,tmp[1]);
				opcodelist[opcodelist_i][1] = cal_LOCCTR_A(&sy,tmp,&loc);
				flag = cal_OPCODE_A(sy,tmp,&opcode);
				if(flag == 1) {
					printf("%s\t%X\t%s\t%s\t%s     \t%06X\n",tmp[0],temp,tmp[1],tmp[2],tmp[3],opcode);
					insertFILE(oplist_len,opcode, w,1,temp);
				}
				else if(flag == 0){
					printf("%s\t%X\t%s\t%s\t%s    \t%02X\n",tmp[0],temp,tmp[1],tmp[2],tmp[3],opcode);
					insertFILE(oplist_len,opcode, w,1,temp);
				}
				else if(flag == 2) {
					printf("%s\t%X\t%s\t%s\t%s    \t%\n",tmp[0],temp,tmp[1],tmp[2],tmp[3]);	
				}
				else if(flag == 3) {
					printf("%s\t%X\t%s\t%s\t%s       \t%06X\n",tmp[0],temp,tmp[1],tmp[2],tmp[3],opcode);
					insertFILE(oplist_len,opcode, w,2,temp);
				}
				else {
					printf("%s\t%X\t%s\t%s\t%s    \t\n",tmp[0],temp,tmp[1],tmp[2],tmp[3]);
					insertFILE(oplist_len,opcode, w,4,temp);
				}
				break;
			case 3:
				temp = loc;
				if(strcmp(tmp[1],"END")) {
					opcodelist[opcodelist_i][1] = cal_LOCCTR_B(&loc);
					cal_OPCODE_B(sy,tmp,&opcode);
					printf("%s\t%X\t\t%s\t%s    \t%06X\n",tmp[0],temp,tmp[1],tmp[2],opcode);
					insertFILE(oplist_len,opcode, w,1,temp);
				}
				else{
					printf("%s\t\t\t%s\t%s    \t\n",tmp[0],tmp[1],tmp[2]);
				}
				break;
			case 2:
				if(!strcmp(tmp[1],".")){
					printf("%s    \t%s\n",tmp[0],tmp[1]);
				}
				else{
					temp = loc;
					opcodelist[opcodelist_i][1] = cal_LOCCTR_C(&loc);
					cal_OPCODE_C(sy,tmp,&opcode);
					printf("%s\t%X\t\t%s    \t\t%06X\n",tmp[0],temp,tmp[1],opcode);
					insertFILE(oplist_len,opcode, w,1,temp);
				}
				break;
			default:
				break;
		}
	}
	insertFILE(oplist_len,opcode, w,0,temp);
	fprintf(w,"\nE.%06X\n",END_LOC);
	fclose (f);
	fclose (w);
}

main(){
	pass1();
	pass2();
}
