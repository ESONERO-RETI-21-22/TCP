/*
 ============================================================================
 Name        : client.c
 Author      : Alberto Ricchiuti, Beatrice Scavo
 Description : TCP program
 ============================================================================
 */

#include "protocol.h"
#include "function.h"



int main(int argc, char *argv[]) {

	setvbuf(stdout, NULL, _IONBF, 0); //output buffer cleaner

	question op={'\0', 0, 0};
	answer res={"\0", 0};
	short flag=1;
	int port=0;
	char ind[16]={"\0"};



	if (argc > 1) { //if parameters are specified they will be checked and possibly used as address

		if(numericalControl(argv[1])==1 && atoi(argv[1])>0 && atoi(argv[1])<65536){
			if(strlen(argv[0])<16 && addressParser(argv[0])==1){
				strcpy(ind, argv[0]);
				port = atoi(argv[1]);
			} else {
				puts("- IP address is not valid!\n\n");
				strcpy(ind, PROTIP);//use default ip and port number
				port = PROTOPORT;
			}
		} else {
			puts("- Port number is not valid!\n\n");
			strcpy(ind, PROTIP); //use default ip and port number
			port = PROTOPORT;
		}

	} else {
		strcpy(ind, PROTIP); //use default ip and port number
		port = PROTOPORT;
	}

#if defined WIN32 //initialize Winsock
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (result != NO_ERROR) {
		errorhandler("- Error at WSAStartup()\n\n");
		system("pause");
		return 0;
	}
#endif

	int my_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //socket creation
	if (my_socket < 0) {
		errorhandler("- Socket creation failed\n\n");
		closesocket(my_socket);
		clearwinsock();
		system("pause");
		return -1;
	}

	struct sockaddr_in sad; //creating server's address
	memset(&sad, 0, sizeof(sad));
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr(ind); //server's ip
	sad.sin_port = htons(port); //server's port

	if (connect(my_socket, (struct sockaddr*) &sad, sizeof(sad)) < 0) { //client connection to server
		errorhandler("- Failed to connect\n\n");
		closesocket(my_socket);	//in error case connection will be closed
		clearwinsock();
		system("pause");
		return -1;
	}

	puts("- ONLINE CALCULATOR -\n\n Calculator for operations between two INTEGERS\nAllowed operations:\n'+'\t'-'\t'*'\t'/'\n");
	puts("Input format required:\toperator number1 number2   [Press Enter]\n\nPress Space Bar for separating the input operation elements.\n"
			"If pressing Enter before completing the input the calculator will wait for the remaining part, \nthen press Enter button again.\n\n");
	puts("Entering as operator '=' and then the other two numbers will close the program and the connection.\n\n\n\nEx:\t*  3  4  [Enter]\n\n\tor\t =  4  1  [Enter]");
	while(flag==1){ //until user's operator input is not '=' connection and operation's request will continue
		puts("\n\n\nInsert the operation below:\n\n\t");
		flag=getInput(&op);
		if(flag==1){

			if ((send(my_socket, &op, sizeof(op), 0)) <= 0) { //sending operation to the server
				errorhandler("\n- send() sent a different number of bytes than expected\n\n");
				closesocket(my_socket);
				clearwinsock();
				system("pause");
				return -1;
			}

			if ((recv(my_socket, &res, sizeof(res), 0)) <= 0) { //receiving server'response for the user
				errorhandler("\n- recv() failed or connection closed prematurely\n\n");
				closesocket(my_socket);
				clearwinsock();
				system("pause");
				return -1;
			}

			res.failedOp=ntohl(res.failedOp); //printing calculation outcome
			if(res.failedOp==0){
				printf("\nThe result is:\t%s\n\n", res.result);
			} else {
				puts("\nOperation not allowed!\n\n");
			}

		} else {
			if ((send(my_socket, &op, sizeof(op), 0)) <= 0) { //sending operator '=' for connection end
				errorhandler("\n- send() sent a different number of bytes than expected\n\n");
				closesocket(my_socket);
				clearwinsock();
				system("pause");
				return -1;
			}
		}
		memset(&op,0,sizeof(op)); //cleaning the values of the structs used for trasmission
		memset(&res,0,sizeof(res));
	}



	puts("\nTerminating process\n\n");	//process ends
	closesocket(my_socket);
	clearwinsock();
	system("pause");
	return 0;
}
