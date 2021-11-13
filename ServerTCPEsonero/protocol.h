#ifndef PROTOCOL_H_
#define PROTOCOL_H_
// Costants
#define BUFFERSIZE 512
#define PROTOPORT 27015
#define NO_ERROR 0


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

int divis(int a,int b){
	return a/b;
}

int add(int a,int b) {
	return a+b;
}

int mul(int a,int b) {
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
		return mul(op.number1,op.number2);
	}
	if(op.op == '/') {
		return divis(op.number1,op.number2);
	}
	return 0;
}


#endif /* PROTOCOL_H_ */
