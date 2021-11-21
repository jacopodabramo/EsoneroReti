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

#include "protocol.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

//Prototypes
int checkOp(char);

int check(char[], struct Operation*);

int check2(char[], struct Operation *);

int main(int argc, char *argv[]) {

  #if defined WIN32

     WSADATA wsa_data;
     int result = WSAStartup(MAKEWORD(2,2), &wsa_data);
     if (result != NO_ERROR) {
        printf("Error at WSAStartup()\n");
        return 0;
     }

  #endif

     //Connection variables initialization
     int c_socket;
     struct sockaddr_in sad;
     int bytes_rcvd;
     int total_bytes_rcvd = 0;
     int bytes = 0;
     memset(&sad, 0, sizeof(sad));

     //message to Server variables initialization
     struct Operation msg;
     char message[30];


     //socket creation
     c_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
     if(c_socket<0){
        errorhandler("socket creation failed \n");
        closesocket(c_socket);
        clearwinsock();
        return -1;
     }

    sad.sin_family = AF_INET;
    if(argc > 1){
    	//command line values
    	sad.sin_addr.s_addr = inet_addr(argv[1]);
    	sad.sin_port = htons(atoi(argv[2]));
    }
    else{
    	//default values
    	sad.sin_addr.s_addr = inet_addr("127.0.0.1");
    	sad.sin_port = htons(PROTOPORT);
    }

    //server connection
    if(connect(c_socket, (struct sockaddr *)&sad, sizeof(sad))<0){
        errorhandler("Failed to connect \n");
        closesocket(c_socket);
        clearwinsock();
        return -1;
    }

    while(1){
    	//getting operation
    	do {
    		printf("Enter a command in a format:[operation] [first operate] [second operate] \n");
    		fgets(message,sizeof(message),stdin);
    	} while (check2(message,&msg) == 0);

    	if(checkOp(msg.op)) {
    		printf("first operator = %d\n", msg.number1);
    		printf("second operator  = %d\n", msg.number2);
    	}

    	//sending data to server
    	bytes = (int)sizeof(struct Operation);
    	if(send(c_socket, (char*)(struct Operation*)&msg, bytes, 0) < 0){
               errorhandler("send() sent a different number of bytes\n");
               closesocket(c_socket);
               clearwinsock();
               return -1;
        }

    	//stop connection
    	if(msg.op == '=') break;

    	// result
    	int a;

    	//receiving data from server
    	total_bytes_rcvd = 0;
    	while(total_bytes_rcvd < sizeof(int)){
            if((bytes_rcvd = recv(c_socket,(char *) (int*)&a, (int) sizeof(int), 0))<=0){
                errorhandler("recv() failed or connection closed prematurely\n");
                closesocket(c_socket);
                clearwinsock();
                return -1;
            }
            total_bytes_rcvd += bytes_rcvd;
        }

    	printf("result = %d\n",a);
    }

    printf("Closing connection ...");
    closesocket(c_socket);
    clearwinsock();
    return 0;
}

/*
 * Parameters:
 *             a: character
 * Return:
 *             integer: 1 if input is an operation,
 *                      0 otherwise
 */
int checkOp(char a){
    if(a == '+' ||  a == 'x' || a == '/' ||  a == '-') return 1;
    return 0;
}

/*
 * Parameters:
 *             in: User's message
 *             op: mapping user's message to operation struct
 * Return:
 *             integer: 1 if user's message is valid
 *                      0 otherwise
 */
int check(char in[], struct Operation *op){
    int i;
    int k = 0;
    for(i = 0; i < strlen(in); i++){
        if(in[i] == ' ') continue;
        else if(in[i] == '='){
            k = 3;
            op->op  = in[i];
            i++;
            break;
        }
        else if(checkOp(in[i]) == 1){
            op->op  = in[i];
            break;
        }
        else return 0; // false
    }

    while(k < 2) {
    // removing space
    for(int j = i + 1;j < strlen(in); j++){
        if(in[j] == ' ') continue;
        else if(isdigit(in[j])){
            i = j;
            break;
        } else return 0; // false
    }

    // checking number
    char num[10];
    int t = 0;
    for(int j = i;j < strlen(in); j++){
            if(isdigit(in[j])) {
                num[t] = in[j];
                t++;
                continue;
            }
            else if(in[j] == ' ' || j == strlen(in) - 1){
                num[t] = '\0';
                i = j;
                if( k == 0) op->number1 = atoi(num);
                else op->number2 = atoi(num);
                break;
            } else return 0;
        }

    if(i > strlen(in)) return 0; // false
    k++;
    }

    // no chararcters after second number
    for(int j = i; j < strlen(in) - 1; j++){
        if(in[j] != ' ') return 0;
    }
    return 1;


}

/*
 * Parameters:
 *             in: User's message
 *             op: mapping user's message to operation struct
 * Return:
 *             integer: 1 if user's message is valid
 *                      0 otherwise
 */
int check2(char in[], struct Operation *op) {
	if(checkOp(in[0]) == 1 && in[1] == ' '){
		op->op = in[0];
	}else if(in[0] == '=' && strlen(in) == 2){
		op->op = in[0];
		return 1;
	}
	else return 0;// false
	int k = 0;
	int i = 2;
	while(k<2){
		int j = 0;
		char num[10];
		while(isdigit(in[i])){
			num[j] = in[i];
			j++;
			i++;
		}
		num[j] = '\0';

		if(in[i] == ' ' || i == strlen(in) - 1){
			if(k == 0){
				op->number1 = atoi(num);
				i++;
			}else op->number2 = atoi(num);
		}else return 0;
		k++;
	}
	if(i == strlen(in) -1) return 1;
	else return 0;
}
