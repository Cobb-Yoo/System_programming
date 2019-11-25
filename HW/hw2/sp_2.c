#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void word_f(char *bp,int *a, int *b, int *c){
	char *cp;
	int i,j;
	
	for(cp = strtok(bp, " \t\n"); cp != NULL; ) {
		for(j=0;j<strlen(cp);j++){
			if(',' == cp[j] || '\''== cp[j]){
				*a+=1;
				break;
			}
		}
		
		for(i = 0;i < strlen(cp);i++) {
			
			if('A' <= cp[i] && cp[i] <= 'Z'){
				*b+=1; //알파벳 수.
				break;
			}
			
			if('0' <= cp[i] && cp[i] <= '9'){
				*c+=1;//숫자 단어의 수. 
				break;
			}
		}
		cp = strtok(NULL, " \t\n");
	}
}

int main() {
	 FILE *fp;
	 char buf[80];
	 int a,b,c;
	 
	 a=b=c=0;

	 if ((fp = fopen("sample.s", "r")) == NULL) {
		fprintf(stderr, "file not found...\n");
		exit(1);
	 }

 	while(fgets(buf, sizeof(buf), fp) != NULL) {//단어의 총 수 
		word_f(buf,&a,&b,&c);
 	}

	fclose(fp);
	
	b -= a;

	printf("Number of words = %d\n", a);
	printf("Number of numbers = %d\n", c);
	printf("Number of alphabet = %d\n", b);
}

