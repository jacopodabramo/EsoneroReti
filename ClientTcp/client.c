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
#define BUFFERSIZE 512
#define PROTOPORT 27015

#define NO_ERROR 0
void clearwinsock() {
#if defined WIN32
    WSACleanup();
#endif
}


struct Operation {
	char op;
	int number1;
	int number2;
};

void errorhandler(char *errorMessage) {
	printf("%s", errorMessage);
}


char operation(int i){
	if(i == 0) return '+';
	if(i == 1) return '-';
	if (i == 2) return 'x';
	if(i == 3) return '/' ;
	return '+';
}

int main(int argc, char *argv[]) {
    //...
#if defined WIN32
    // Initialize Winsock
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


    c_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(c_socket<0){
        errorhandler("socket creation failed \n");
        closesocket(c_socket);
        clearwinsock();
        return -1;
    }
    struct Operation op;
    sad.sin_family = AF_INET;
    sad.sin_addr.s_addr = inet_addr( "127.0.0.1" );
    sad.sin_port = htons( 27015 );

    if(connect(c_socket, (struct sockaddr *)&sad, sizeof(sad))<0){
        errorhandler("Failed to connect \n");
        closesocket(c_socket);
        clearwinsock();
        return -1;
    }





    for(int i = 0; i < 4; i++){
    op.op = operation(i);
    op.number1 = 10;
    op.number2 = 20;

    bytes = (int)sizeof(struct Operation);
    if(send(c_socket, (struct Operation*)&op,bytes, 0) < 0){
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
    }

    // sendig to close connection
    op.op = '=';
    if(send(c_socket, (struct Operation*)&op,bytes, 0) < 0){
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
