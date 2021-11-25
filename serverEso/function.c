/*
 ============================================================================
 Name        : function.c
 Author      : Alberto Ricchiuti, Beatrice Scavo
 Description : TCP program
 ============================================================================
 */

#include "protocol.h"

int add(question *op){ //adds two numbers
	return op->number1+op->number2;
}

int sub(question *op){ //subtracts two numbers
	return op->number1-op->number2;
}

int mult(question *op){ //multiplies two numbers
	return op->number1*op->number2;
}

float division(question *op){ //divides two numbers
	float x = (float) op->number1, y = (float) op->number2;

	return x/y;
}

short executeOp(question *op, answer *res){ //recognizes and performs the operation handling the cases of error

	short flag=1;
	float fres=0.0;
	op->number1=ntohl(op->number1);
	op->number2=ntohl(op->number2);

	switch(op->operator){
	case'+':
		fres=add(op);
		break;

	case'-':
		fres=sub(op);
		break;

	case '*':
		fres=mult(op);
		break;

	case '/':
		if(op->number2==0){
			res->failedOp=1;
		} else {
			fres=division(op);
		}
		break;

	case '=':
		flag=0;
		break;
	}

	sprintf(res->result, "%g", fres); //converts float to string
	res->failedOp=htonl(res->failedOp);

	return flag;
}


