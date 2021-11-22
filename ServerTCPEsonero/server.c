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
#include <string.h>
#include "protocol.h"

//Prototypes
float calculator(struct Operation);

float sub(int,int);

float mult(int,int);

float add(int,int);

float division(int,int);

int main(int argc,char *argv[]) {

  #if defined WIN32

    WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2,2), &wsa_data);
    if (result != NO_ERROR) {
        printf("Error at WSAStartup()\n");
        return 0;
    }

  #endif

    //Connection variables initialization
    int my_socket;
    struct sockaddr_in sad;
    int qlen = 5; // max number of clients
    struct sockaddr_in cad;
    int client_socket;
    int client_len;
    int bytes_rcvd;
    int total_bytes_rcvd = 0;

    //message to client variable initialization
    struct Operation op;

    //socket creation
    my_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(my_socket< 0) {
        errorhandler("socket creation failed \n");
        return -1;
    }

    sad.sin_family = AF_INET;
   	if (argc > 1) {
    	sad.sin_addr.s_addr = inet_addr( argv[1]);
    	sad.sin_port = htons(atoi(argv[2]));
    }

   	else {
    	sad.sin_addr.s_addr = inet_addr( "127.0.0.1" );
    	sad.sin_port = htons( PROTOPORT );
    }

   	//binding
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

    	while(1){
    		total_bytes_rcvd = 0;


    		//receiving data from client
    		if((bytes_rcvd = recv(client_socket,(char*)(struct Operation*)&op, sizeof(struct Operation), 0)) <= 0){
    			errorhandler("recv() failed or connection closed prematurely");
    			closesocket(client_socket);
    			clearwinsock();
    			return -1;
    		}

    		total_bytes_rcvd += bytes_rcvd;

    		if(op.op == '=') {
    			printf("\n");
    			break;
    		} else {

    		    // Calculating the result
    		    float result = calculator(op);

    		     // Sending result
    		     if(send(client_socket,(char*)(float*)&result, sizeof(float), 0) < 0){
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
}

/*
 * Parameters:
 *           a,b: operators
 * Return:
 *           float: result of division
 */
float division(int a,int b){
	if(b == 0) return FLT_MAX  ;
	float toTfloata = (float)a;
	float toTfloatb = (float)b;
	return toTfloata/toTfloatb;
}

/*
 * Parameters:
 *           a,b: operators
 * Return:
 *           float: result of addition
 */
float add(int a,int b) {
	return a+b;
}

/*
 * Parameters:
 *           a,b: operators
 * Return:
 *           float: result of multiplication
 */
float mult(int a,int b) {
	return a*b;
}

/*
 * Parameters:
 *           a,b: operators
 * Return:
 *           float: result of subtraction
 */
float sub(int a,int b) {
	return a-b;
}

/*
 * Parameters:
 *           op: struct
 * Return:
 *           float: parsing operation and getting result
 */
float calculator(struct Operation op){
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

