/*
 ============================================================================
 Name        : function.h
 Author      : Alberto Ricchiuti, Beatrice Scavo
 Description : TCP program
 ============================================================================
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_

short executeOp(question *op, answer *res); //recognizes and performs the operation handling the cases of error

int add(question *op); //adds two numbers

int sub(question *op); //subtracts two numbers

int mult(question *op); //multiplies two numbers

float division(question *op); //divides two numbers

#endif /* FUNCTION_H_ */
