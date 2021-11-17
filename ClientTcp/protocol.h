#ifndef PROTOCOL_H_
#define PROTOCOL_H_
// Costants
#define BUFFERSIZE 512
#define PROTOPORT 27015
#define NO_ERROR 0
#define ZERO '0'
#define ONE '1'
#define TWO '2'
#define THREE '3'
#define FOUR '4'
#define FIVE '5'
#define SIX '6'
#define SEVEN '7'
#define EIGHT '8'
#define NINE '9'
#define PLUS '+'
#define MINUS '-'
#define MULTIPLY 'x'
#define DIVISION '/'
#define EQUAL '='


struct Operation {
	char op;
	int number1;
	int number2;
};


void create(struct Operation op){
	op.op = ' ';
	op.number1 = 0;
	op.number2 = 0;
}
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
