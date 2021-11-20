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
#include<string.h>
#include "protocol.h"

int calculator(struct Operation);
int sub(int,int);
int mult(int,int);
int add(int,int);
int division(int,int);

int main(int argc,char *argv[]) {

#if defined WIN32
    WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2,2), &wsa_data);
    if (result != NO_ERROR) {
        printf("Error at WSAStartup()\n");
        return 0;
    }
#endif
    int my_socket;
    struct sockaddr_in sad;
    int qlen = 5; // max number of clients
    struct sockaddr_in cad;
    int client_socket;
    int client_len;
    int bytes_rcvd;
    int total_bytes_rcvd = 0;


    my_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(my_socket< 0) {
        errorhandler("socket creation failed \n");
        return -1;
    }



    sad.sin_family = AF_INET;
    // Default value
    sad.sin_addr.s_addr = inet_addr( "127.0.0.1" );
   	sad.sin_port = htons( PROTOPORT );

   	// user value in the comand line

   	if (argc > 1) {
    	sad.sin_addr.s_addr = inet_addr( argv[1]);
    	sad.sin_port = htons((int)argv[2]);
    }


    if(bind( my_socket , (struct sockaddr *) &sad, sizeof (sad))<0){
        errorhandler(("bind() failed \n"));
        closesocket(my_socket);
        return -1;
    }

    if(listen(my_socket, qlen) < 0){
        errorhandler("listen() failed \n");
        closesocket(my_socket);
        return -1;
    }

    while(1){
    	printf("Waiting for a client to connect...");
    	client_len = sizeof(cad);
    	if((client_socket=accept(my_socket, (struct sockaddr *) &cad, &client_len))<0){
          errorhandler("accept() failed \n");
          closesocket(my_socket);
          clearwinsock();
          return -1;
        }

    	printf("Connection established with %s:%d", inet_ntoa(cad.sin_addr),sad.sin_port);

    	// sending and receving data
    	while(1){
    	// Receving operartion
    		total_bytes_rcvd = 0;
    		struct Operation op;
    		if((bytes_rcvd = recv(client_socket,(struct Operation*)&op, sizeof(struct Operation), 0)) <= 0){
    			errorhandler("recv() failed or connection closed prematurely");
    			closesocket(client_socket);
    			clearwinsock();
    			return -1;
    		}

    		total_bytes_rcvd += bytes_rcvd;

    		// Closing socket if the clent sends "="
    		if(op.op == '=') {
    			printf("\n");
    			break;
    		} else {

    			// Calculating the result
    			int result = calculator(op);

    			// Sending result
    			if(send(client_socket, (int*)&result, sizeof(int), 0) < 0){
    				errorhandler("send() sent a different number of bytes");
    				closesocket(client_socket);
    				clearwinsock();
    				return -1;
    			}
    		}
    	}
   }



    closesocket(my_socket);
    clearwinsock();
    system("pause");
    return 0;
} // main end

int division(int a,int b){
	return a/b;
}

int add(int a,int b) {
	return a+b;
}

int mult(int a,int b) {
	return a*b;
}

int sub(int a,int b) {
	return a-b;
}

int calculator(struct Operation op){
	if(op.op == '+') {
		return add(op.number1,op.number2);
	}
	if(op.op == '-') {
		return sub(op.number1,op.number2);
	}
	if(op.op == 'x') {
		return mult(op.number1,op.number2);
	}
	if(op.op == '/') {
		return division(op.number1,op.number2);
	}
	return 0;
}

