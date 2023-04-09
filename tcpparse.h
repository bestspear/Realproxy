#include<windows.h>
#include<stdio.h>
class tcpparse {
public:
	int numbuf[100];
	int numsize;
	int parse(char* data);
private:
	char bufnu[4];
};