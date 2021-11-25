/*
 ============================================================================
 Name        : server.c
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
	int port= PROTOPORT;

#if defined WIN32	//initialize Winsock
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (result != NO_ERROR) {
		errorhandler("\n- Error at WSAStartup()\n\n");
		system("pause");
		return 0;
	}
#endif

	int my_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //creating socket
	if (my_socket < 0) {
		errorhandler("\n- Socket creation failed\n\n");
		clearwinsock();
		system("pause");
		return -1;
	}

	//address assignment
	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad)); //ensures that extra bytes contain 0
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr(PROTIP);
	sad.sin_port = htons(port); /*converts values between the host and
	 	 	 	 	 	 	 	 network byte order. Specifically, htons() converts 16-bit quantities
	 	 	 	 	 	 	 	 from host byte order to network byte order.*/

	if (bind(my_socket, (struct sockaddr*) &sad, sizeof(sad)) < 0) {
		errorhandler("\n- bind() failed\n\n"); //binding socket and address
		closesocket(my_socket);
		clearwinsock();
		system("pause");
		return -1;
	}

	if (listen(my_socket, QLEN) < 0) { //setting socket for listening
		errorhandler("\n- listen() failed\n\n");
		closesocket(my_socket);
		clearwinsock();
		system("pause");
		return -1;
	}

	//accepting a new connection
	struct sockaddr_in cad; // structure for the client address
	int client_socket = 0; // socket descriptor for the client
	int client_len; // the size of the client address
	printf("\n- SERVER-SIDE -\n\n\n- Waiting for a client to connect...\n\n");
	while (1) { //server never ends its process, always waiting for client
		client_len = sizeof(cad); // set the size of the client

		if ((client_socket = accept(my_socket, (struct sockaddr*) &cad,
				&client_len)) < 0) { //acceptation
			errorhandler("\n- accept() failed\n\n");
			closesocket(client_socket); //closing connection in case of error
			clearwinsock();
			system("pause");
			return 0;
		}

		printf("Handling client: %s\n\n", inet_ntoa(cad.sin_addr));

		while(flag==1){ //as long as the client doesn't close the connection, the server will respond to its requests
			if ((recv(client_socket, (question*) &op, sizeof(op), 0)) <= 0) { //server receives the client requests
				errorhandler("\n- recv() failed or connection closed prematurely\n\n");
				closesocket(my_socket);
				clearwinsock();
				system("pause");
				return -1;
			}

			flag=executeOp(&op,&res); //calculating the operation's result

			if(flag==1){
				if ((send(client_socket, &res, sizeof(res), 0)) <= 0) { //sending the response to the client
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

		flag=1; //preparation for the next client

	}


	puts("\n- Terminating Service\n\n"); //terminating process
	closesocket(my_socket);
	clearwinsock();
	system("pause");
	return 0;
}
