/*
 ============================================================================
 Name        : function.h
 Author      : Alberto Ricchiuti, Beatrice Scavo
 Description : TCP program
 ============================================================================
 */
#ifndef FUNCTION_H_
#define FUNCTION_H_

#include "protocol.h"
#include <ctype.h>

short getInput(question *op); //takes the operation to be transmitted to the server
short numericalControl(char num[MAX_LENGTH]); //checks the numbers entered for the operation
short analyzeInput(char operator, char num1[MAX_LENGTH], char num2[MAX_LENGTH]); //checks the operation entered in input
short addressParser(char address[16]); //checks the structure of ip address
short addressControl(char partOf[4]); ////checks the validity of an ip address' segment
#endif /* FUNCTION_H_ */
