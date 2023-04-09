#include "httpparse.h"

int httpparse::parse(char* data) {

	for (size_t i = 0; i < 0x2000; i++)
	{
		if (data[i] == 0x0d) {
			if (data[i + 1] == 0x0a && data[i + 2] == 0x0d && data[i + 3] == 0x0a) {
				headerbegin = data;
				headerend = &data[i];
				httpheaderlength = headerend - headerbegin;
				return 1;
			}
		}
	}
	return 0;


}
int httpparse::getlength() {
	char lenflag[] = "Content-Length:";
	//printf("out::%d\n", strlen(lenflag));
	for (size_t i = 0; i < (headerend - headerbegin); i++)
	{
		int ok = 0;
		for (int o = 0; o < strlen(lenflag); o++) {
			if (lenflag[o] != headerbegin[i + o])
			{
				break;
			}
			ok++;
		}
		if (ok == strlen(lenflag))
		{
			char numbuf[10];
			memset(numbuf, 0, 10);
			for (size_t iu = 0; iu < 10; iu++)
			{

				if (headerbegin[i + strlen(lenflag) + iu] == 0x0d)
				{
					break;
				}
				numbuf[iu] = headerbegin[i + strlen(lenflag) + iu];
			}

			return atoi(numbuf);

		}
	}
	return -1;

}
int httpparse::getflagvalue(char* flag, char* outbuf) {

	for (size_t i = 0; i < (headerend - headerbegin); i++)
	{
		int ok = 0;
		for (int o = 0; o < strlen(flag); o++) {
			if (flag[o] != headerbegin[i + o])
			{
				break;
			}
			ok++;
		}
		if (ok == strlen(flag))
		{
			char numbuf[200];
			memset(numbuf, 0, 200);
			for (size_t iu = 0; iu < 200; iu++)
			{

				if (headerbegin[i + strlen(flag) + iu] == 0x0d)
				{
					break;
				}
				outbuf[iu] = headerbegin[i + strlen(flag) + iu];
			}

			return 1;

		}
	}
	return -1;

}
