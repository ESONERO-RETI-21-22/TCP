/*
 ============================================================================
 Name        : function.c
 Author      : Alberto Ricchiuti, Beatrice Scavo
 Description : TCP program
 ============================================================================
 */
#include "function.h"

short  numericalControl(char num[MAX_LENGTH]){ //checks the numbers entered for the operation
	short ctrl=1, i=0;

	if(strlen(num)>(MAX_LENGTH-1) || strlen(num)==0){
		ctrl=0;
		puts("\n- This number is too big!\n\n");
	}

	while(ctrl==1 && i<strlen(num)){
		if(isdigit(num[i]) || (i==0 && (num[i]=='-' || num[i]=='+') && strlen(num)>1)){
			i++;
		} else {
			ctrl=0;
		}
	}

	return ctrl;
}

short analyzeInput(char operator, char num1[MAX_LENGTH], char num2[MAX_LENGTH]){ //checks the operation entered in input
	short ctrl=1;

	if((operator!='+' && operator!='-' && operator!='*' && operator!='/') || numericalControl(num1)==0 || numericalControl(num2)==0){

		if(operator=='='){
			ctrl=-1;
		} else {
			ctrl=0;
		}

	}

	return ctrl;
}

short getInput(question *op){ //takes the operation to be transmitted to the server
	short flag=1, ctrl=1;
	char operator='0';
	char num1[MAX_LENGTH]={"0"}, num2[MAX_LENGTH]={"0"};

	do{
		operator='\0';
		memset(&num1, '\0', sizeof(num1));
		memset(&num2, '\0', sizeof(num2));

		fflush(stdin);
		scanf("%c %s %s", &operator, num1, num2);

		ctrl=analyzeInput(operator, num1, num2); //checks if the operation is valid

		if(ctrl==0){ //if the input is not valid, it will be entered again
			puts("\n- Insertion error\n\n  Try again. . . \n\n");
			puts("\nInsert the operation below:\n\n\t");
		} else if(ctrl==-1){ //if the operator is '=', the connection will end with flag returned
			puts("\n- Closing connection\n\n");
			flag=0;
		}
	}while(ctrl==0);

	if(flag==1){
		op->operator=operator;
		op->number1=htonl(atoi(num1));
		op->number2=htonl(atoi(num2));
	} else {
		op->operator=operator;
	}

	return flag;
}

short addressControl(char partOf[4]){ //checks the validity of an ip address' segment
	short ctrl=1, i=0;
	int add=0;

	while(ctrl==1 && i<strlen(partOf)){
		if(isdigit(partOf[i])){
			i++;
		} else {
			ctrl=0;
		}
	}
	if(ctrl==1){
		if(strlen(partOf)!=0){
			add=atoi(partOf);
			if(add<0 || add>255){
				ctrl=0;
			}
		} else {
			ctrl=0;
		}
	}

	return ctrl;
}

short addressParser(char address[16]){ //checks the structure of ip address

	short ctrl=1, i=0, contP=0, contC=0, flag=1;
	char partOf[4]={"\0"};

	while(ctrl==1 && i<strlen(address)){
		if(address[i]=='.'){
			contP++;
			ctrl=addressControl(partOf);
			printf("\n\n c:%d", ctrl);
			memset(&partOf, '\0', sizeof(partOf));
			contC=0;
		} else {
			contC++;
			if(contC<4){
				partOf[contC-1]=address[i];
			}
		}
		i++;
		if(i==strlen(address)){
			ctrl=addressControl(partOf);
		}
		if(contP>3 || contC>3 || ctrl==0){
			ctrl=0;
			flag=0;
		}
	}
	if(contP!=3){
		flag=0;
	}
	return flag;
}
