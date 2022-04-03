#include <stdio.h>

int main(int argc, char* argv[]) {
	int i=0; 
	printf("argc = %d\n",argc); // argc = 파라미터의 수
	do {
		printf(" %s\n",argv[i]);// argv[i] = 파라미터[i]의 내용
		i++;
	} while(i<argc);
	int num1,num2;
	int result=0;
	printf("첫번째 숫자를 입력하시오: ");
	scanf("%d",&num1);
	printf("두번째 숫자를 입력하시오: ");
	scanf("%d",&num2);
	
	result = num1+num2;
	printf("%d + %d = %d\n",num1,num2,result);
	result = num1-num2;
	printf("%d - %d = %d\n",num1,num2,result);
	result = num1*num2;
	printf("%d * %d = %d\n",num1,num2,result);
	result = num1/num2;
	printf("%d / %d = %d\n",num1,num2,result);
	return 1;
	
}
