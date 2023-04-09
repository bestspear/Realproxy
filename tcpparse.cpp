#include "tcpparse.h"


int tcpparse::parse(char* data) {
	int k = 0;
	int b = 0;
	memset(bufnu, 0x0, 4);
	for (size_t i = 0; i < strlen(data); i++)
	{

		if (data[i] == 0x20) {
			memset(bufnu, 0x0, 4);
			k = 0;
			numbuf[b] = atoi(bufnu);
			b++;
			if (b >= 100) {
				printf("error:Too many bytes then exit!\n");
				exit(0);
			}
		}
		bufnu[k] = data[i];
		k++;
	}
	numsize = b;
}