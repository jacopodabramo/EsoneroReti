#ifndef PROTOCOL_H_
#define PROTOCOL_H_
// Costants
#define BUFFERSIZE 512
#define PROTOPORT 27015
#define NO_ERROR 0
#include"float.h"

struct Operation {
	char op;
	int number1;
	int number2;
};


// functions
void clearwinsock() {
#if defined WIN32
    WSACleanup();
#endif
}

void errorhandler(char *errorMessage) {
	printf("%s", errorMessage);
}



#endif /* PROTOCOL_H_ */
