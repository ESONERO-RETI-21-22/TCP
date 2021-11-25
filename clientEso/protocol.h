/*
 ============================================================================
 Name        : protocol.h
 Author      : Alberto Ricchiuti, Beatrice Scavo
 Description : TCP program
 ============================================================================
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#if defined WIN32
#include <winsock.h>
#else
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#define closesocket close
#endif

#include <stdio.h>
#include <stdlib.h>
#define NO_ERROR 0
#define BUFFERSIZE 512
#define PROTOPORT 27015 //default port number
#define MAX_LENGTH 8 //max length of operands
#define PROTIP "127.0.0.1" //default ip

typedef struct{ //datatype for operation trasmission
	char operator;
	int number1;
	int number2;
}question;

typedef struct{ //datatype for result trasmission
	char result[16];
	int failedOp; //flag for operation error
}answer;


void errorhandler(char *errorMessage);
void clearwinsock();


#endif /* PROTOCOL_H_ */
