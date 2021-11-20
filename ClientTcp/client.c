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
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#define BUFFERSIZE 512
#define PROTOPORT 27015

#define NO_ERROR 0

int checkOp(char);
int check(char[], struct Operation*);

int main(int argc, char *argv[]) {

  #if defined WIN32

     WSADATA wsa_data;
     int result = WSAStartup(MAKEWORD(2,2), &wsa_data);
     if (result != NO_ERROR) {
        printf("Error at WSAStartup()\n");
        return 0;
     }

  #endif

     //Variables initalization
     int c_socket;
     struct sockaddr_in sad;
     int bytes_rcvd;
     int total_bytes_rcvd = 0;
     int bytes = 0;
     memset(&sad, 0, sizeof(sad));
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
    sad.sin_addr.s_addr = inet_addr( "127.0.0.1");
    sad.sin_port = htons(27015);

    //server connection
    if(connect(c_socket, (struct sockaddr *)&sad, sizeof(sad))<0){
        errorhandler("Failed to connect \n");
        closesocket(c_socket);
        clearwinsock();
        return -1;
    }


    while(1){
    	do {

    		printf("Inserire una stringa nel formato:[operazione] [operando 1] [operando 2] \n");
    		fgets(message,sizeof(message),stdin);

    	} while (check(message,&msg) == 0);

    	if(checkOp(msg.op)){
    		printf("Operando uno = %d",msg.number1);
    		printf(" Operando due = %d ",msg.number2);
    	}
    	bytes = (int)sizeof(struct Operation);
    	if(send(c_socket, (struct Operation*)&msg, bytes, 0) < 0){
               errorhandler("send() sent a different number of bytes");
               closesocket(c_socket);
               clearwinsock();
               return -1;
        }

    	if(msg.op == EQUAL) break;

    	// result
    	int a;

    	total_bytes_rcvd = 0;
    	while(total_bytes_rcvd < sizeof(int)){
            if((bytes_rcvd = recv(c_socket, (int*)&a, (int) sizeof(int), 0))<=0){
                errorhandler("recv() failed or connection closed prematurely");
                closesocket(c_socket);
                clearwinsock();
                return -1;
            }
            total_bytes_rcvd += bytes_rcvd;
        }

    	printf("Risultato = %d\n",a);
    }

    printf("Closing connection ...");
    closesocket(c_socket);
    clearwinsock();
    return 0;
}


int checkOp(char a){
    if(a == '+' ||  a == 'x' || a == '/' ||  a == '-') return 1;
    return 0;
}

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
