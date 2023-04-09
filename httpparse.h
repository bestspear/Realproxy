#include <stdio.h>
#include <windows.h>
class httpparse {
public:
	char end[4] = { 0x0d,0x0a,0x0d,0x0a };
	char* headerend;
	char* headerbegin;
	int httpheaderlength;
	//Ä¬ÈÏ0x2000×Ö½Ú
	int parse(char* data);
	int getlength();

	int getflagvalue(char* flag, char* outbuf);
};