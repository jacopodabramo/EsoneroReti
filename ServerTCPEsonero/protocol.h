#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#define BUFFERSIZE 512
#define PROTOPORT 27015
#define NO_ERROR 0

void clearwinsock() {
#if defined WIN32
    WSACleanup();
#endif
}

void errorhandler(char *errorMessage) {
	printf("%s", errorMessage);
}

struct Operation {
	char op;
	int number1;
	int number2;
};

#endif /* PROTOCOL_H_ */
