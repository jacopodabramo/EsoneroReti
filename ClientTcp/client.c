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

char getOperation(char in[]){
	return in[0];
}

int getNumber(char in[],int *pos){
	int i = *pos;
     int count = 0;
		  while(in[i] == ONE ||
				in[i] == TWO ||
				in[i] == THREE ||
				in[i] == FOUR ||
			    in[i] == FIVE ||
				in[i] == SIX ||
				in[i] == SEVEN ||
				in[i] == EIGHT ||
				in[i] == NINE
				){
			  	  	  count++;
			  	  	  i++;
		          }
		  i++;
		  char* str1 = &in[*pos];
		  *pos = i;
		  return  atoi(str1);
}

int check(char in[]){
	if(in[0] == PLUS || in[0] == MINUS || in[0] == MULTIPLY || in[0] == DIVISION){
		if(in[1] == ' '){
			for(int i = 2;i < 30; i++){
				if(isdigit(in[i]) == 0 && isspace(in[i]) == 0 && isalpha(in[i]) != 0)
					return 1; // error
			}
			return 0;
		}
		return 1;
	}
	else if(in[0] == EQUAL){
		for(int i = 1;i < 30; i++){
				if(isdigit(in[i]) != 0 || isalpha(in[i]) != 0)
							return 1;
	    }
	    return 0;
	}
	return 1;
}

int main(int argc, char *argv[]) {

#if defined WIN32
    WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2,2), &wsa_data);
    if (result != NO_ERROR) {
        printf("Error at WSAStartup()\n");
        return 0;
    }
#endif
    int c_socket;
    struct sockaddr_in sad;
    int bytes_rcvd;
    int total_bytes_rcvd = 0;
    int bytes = 0;
    memset(&sad, 0, sizeof(sad));
    struct Operation msg;
    create(msg);


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


    if(connect(c_socket, (struct sockaddr *)&sad, sizeof(sad))<0){
        errorhandler("Failed to connect \n");
        closesocket(c_socket);
        clearwinsock();
        return -1;
    }


    char message[30];
    for(int i = 0;i<30;i++) message[i] = " ";
    printf("Inserire stringa");
    fgets(message,sizeof(message),stdin);

    while(check(message)==1){
    	printf("formato sbagliato, inserire un'altra stringa");
    	memset(message,' ',30);
    	fgets(message,sizeof(message),stdin);
    }
    // string is correct
    while(message[0] != EQUAL){
    	msg.op = getOperation(message);
    	int pos = 2;
    	msg.number1 = getNumber(message,&pos);
    	msg.number2 = getNumber(message,&pos);
    	printf("numero 1 %d",msg.number1);
    	printf("numero 2 %d",msg.number2);

    	bytes = (int)sizeof(struct Operation);
    	if(send(c_socket, (struct Operation*)&msg,bytes, 0) < 0){
               errorhandler("send() sent a different number of bytes");
               closesocket(c_socket);
               clearwinsock();
               return -1;
        }

    	int a; // result
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

    printf("Arrivato %d\n",a);
    printf("Inserire stringa");
    for(int i = 0;i<30;i++) message[i] = " ";
        fgets(message,sizeof(message),stdin);

        while(check(message)==1){
        	printf("formato sbagliato, inserire un'altra stringa");
        	memset(message,' ',30);
        	fgets(message,sizeof(message),stdin);
        }

    }

    msg.op = '=';
    if(send(c_socket, (struct Operation*)&msg,bytes, 0) < 0){
       errorhandler("send() sent a different number of bytes");
       closesocket(c_socket);
       clearwinsock();
       return -1;
    }

    printf("Closing connection ...");
    closesocket(c_socket);
    clearwinsock();
    system("pause");
    return 0;
} // main end
