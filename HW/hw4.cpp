#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getNumber(char line[]){
	char *tmp;
	int total=0;
	int sum =0;
	int i; // index
	
	for(tmp=strtok(line," \t\n");tmp!=NULL;){
		switch (tmp[0])	{
			case 'X':
				for(i=2;i<strlen(tmp)-1;i++){					
					if('0' <= tmp[i] && tmp[i] <= '9'){ // 숫자일때 
						sum = sum * 16 + (int)(tmp[i] - '0');
					}
					else{//문자일때 
						sum = sum * 16 + (int)(tmp[i] - 'A' + 10);
					}
				}
				total += sum;
				printf("%s = %d\n",tmp,sum);
				sum = 0;
				break;
				
			case 'C':
				for(i=2;i<strlen(tmp)-1;i++){					
					sum = sum * 16 + (int)(tmp[i]);
				}
				total += sum;
				printf("%s = %d\n",tmp,sum);
				sum = 0;
				break;
				
			default :
				for(i=0;i<strlen(tmp);i++){
					sum = sum * 10 + (int)(tmp[i]-'0');
				}
				total += sum;
				printf("%s = %d\n",tmp,sum);
				sum = 0;
				break;
		}
		tmp=strtok(NULL," \t\n");
	}
	return total;
}

main(){
	FILE* f= fopen("numb.s","r");
	char line[80];
	int sum = 0;
	
	while(fgets(line,80,f)!= NULL){
		sum += getNumber(line);
	}
	
	fclose(f);
	printf("sum of number is %d\n",sum);
}
