// Realproxy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>
#include <vector>
#include <map>
#include <string.h>

#include "httpparse.h"
#include "tcpparse.h"
#include "name.h"

int portvalue;
char* realipvalue;
int realportvalue;
char* fakeipvalue;
int fakeportvalue;

char* typevalue;
char* httpflagvalue;
char* passvalue;

int tcpflagvalue;


char* defaultip = (char*)"127.0.0.1";
int defaultport = 8879;
int defaultpass = 0;

SOCKET realsock;
SOCKET fakesock;

char end[] = { 0x0d,0x0a,0x0d,0x0a };


class CmdParse {

	

	std::map<char*, char*> array;

public:
	int parseinit(int argc, char* argv[]) {
		
		for (size_t i = 0; i < argc; i++)
		{
			if (argv[i][0] == 45)
			{
			
				if (argv[i + 1][0] != 45)
				{
					array.insert(std::pair<char*, char*>(argv[i], argv[i + 1]));
				}
			}
		}
		return 1;
	}
	char* get(char* t) {
		std::map<char*, char*>::iterator it;
		for (it = array.begin(); it != array.end(); it++)
		{
			int f = 0;
			for (size_t i = 0; i < strlen(t); i++)
			{
				if (it->first[i] != t[i]) {
					f++;
				}
			}
			if (f==0)
			{
				return it->second;
			}
		}
		return NULL;
		
	}
};

int proxysend(char* ip, int port, char* sendcontent) {
	WORD realsockVersion = MAKEWORD(2, 2);
	WSADATA realdata;
	if (WSAStartup(realsockVersion, &realdata) != 0)
	{
		return 0;
	}

	SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sclient == INVALID_SOCKET)
	{
		printf("invalid socket !\n");
		return 0;
	}


	struct sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(port);
	serAddr.sin_addr.S_un.S_addr = inet_addr(ip);
	if (connect(sclient, (struct sockaddr*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		printf("connect error !\n");
		closesocket(sclient);
		return 0;
	}
	

		send(sclient, sendcontent, strlen(sendcontent), 0);
		char recData[255];
		int ret = recv(sclient, recData, 255, 0);
		if (ret > 0)
		{

			recData[ret] = 0x00;
			//printf(recData);
		}
		
	
	closesocket(sclient);
	WSACleanup();
	return 0;
}


int tcpsend(PVOID per) {
	struct sockaddr_in realsockadd;
	struct sockaddr_in fakesockadd;
	fakesockadd.sin_family = AF_INET;
	fakesockadd.sin_port = htons(fakeportvalue);
	fakesockadd.sin_addr.S_un.S_addr = inet_addr(fakeipvalue);


	realsockadd.sin_family = AF_INET;
	realsockadd.sin_port = htons(realportvalue);
	realsockadd.sin_addr.S_un.S_addr = inet_addr(realipvalue);




	SOCKET fakesocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKET realsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (realsocket == INVALID_SOCKET)
	{
		printf("socket error !\n");
		return 0;
	}
	if (fakesocket == INVALID_SOCKET)
	{
		printf("invalid socket !\n");
		return 0;
	}

	if (connect(realsocket, (struct sockaddr*)&realsockadd, sizeof(realsockadd)) == SOCKET_ERROR)
	{
		printf("connect real sock error !\n");
		closesocket(realsocket);
		closesocket(fakesocket);
		exit(0);
	}
	if (connect(fakesocket, (struct sockaddr*)&fakesockadd, sizeof(fakesockadd)) == SOCKET_ERROR)
	{
		printf("connect fake sock error !\n");
		closesocket(fakesocket);
		closesocket(realsocket);
		exit(0);
	}
	int ff = 0;
	while (true) {
		int kkkk = 0;
		char* revData = (char*)malloc(0x10000);
		memset(revData, 0x00, 0x10000);
		int rett = recv(*(SOCKET*)per, revData, 0x10000, 0);
		if (rett <= 0)
		{
			printf("TIPS:server shut down!\n");
			closesocket(realsocket);
			closesocket(fakesocket);
			closesocket(*(SOCKET*)per);
			return 0;
		}
		if (ff == 0)
		{
			tcpparse* tcpp = new tcpparse();
			int no = 0;
			for (size_t i = 0; i < tcpp->numsize; i++)
			{
				if (revData[i + tcpflagvalue] == tcpp->numbuf[i])
				{
					no++;
				}
			}

			if (no == tcpp->numsize)
			{
				kkkk++;
			}
		}
		

		char* srecData = (char*)malloc(0x10000);
		
	

		if (defaultpass || kkkk) {
			kkkk = 1;
			int retsend = send(realsocket, revData, strlen(revData), 0);
			if (retsend <= 0)
			{
				printf("error:real send error!\n");
				closesocket(realsocket);
				closesocket(fakesocket);
				closesocket(*(SOCKET*)per);
				return 0;
			}
	
			

			memset(srecData, 0x00, 0x10000);
			int ret = recv(realsocket, srecData, 0x10000, 0);
			int retsen = send(*(SOCKET*)per, srecData, ret, 0);
				


			if (retsen <= 0)
			{
				printf("error:send error!\n");
				closesocket(realsocket);
				closesocket(fakesocket);
				closesocket(*(SOCKET*)per);
				break;
			}
			if (ret <= 0)
			{
				printf("error:real recv error!\n");
				closesocket(realsocket);
				closesocket(fakesocket);
				closesocket(*(SOCKET*)per);
				break;
			}


			

		}
		else {
			int retsend = send(fakesocket, revData, strlen(revData), 0);
			if (retsend <= 0)
			{
				printf("error:real send error!\n");
				closesocket(realsocket);
				closesocket(fakesocket);
				closesocket(*(SOCKET*)per);
				return 0;
			}



			memset(srecData, 0x00, 0x10000);
			int ret = recv(fakesocket, srecData, 0x10000, 0);
			int retsen = send(*(SOCKET*)per, srecData, ret, 0);



			if (retsen <= 0)
			{
				printf("error:send error!\n");
				closesocket(realsocket);
				closesocket(fakesocket);
				closesocket(*(SOCKET*)per);
				break;
			}
			if (ret <= 0)
			{
				printf("error:real recv error!\n");
				closesocket(realsocket);
				closesocket(fakesocket);
				closesocket(*(SOCKET*)per);
				break;
			}



		}
		


		ff++;
		free(srecData);
		free(revData);
	}

	closesocket(realsocket);
	closesocket(fakesocket);
	closesocket(*(SOCKET*)per);

	return 0;
}

int tcpserver() {
	//初始化WSA
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	SOCKET myself = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (myself == INVALID_SOCKET)
	{
		printf("self invalid socket !\n");
		return 0;
	}

	//real target
	fakesock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fakesock == INVALID_SOCKET)
	{
		printf("invalid socket !\n");
		return 0;
	}

	realsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (realsock == INVALID_SOCKET)
	{
		printf("socket error !\n");
		return 0;
	}

	//绑定IP和端口
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(portvalue);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(myself, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}








	//开始监听
	if (listen(myself, 100) == SOCKET_ERROR)
	{
		printf("listen error !\n");
		return 0;
	}

	//循环接收数据
	SOCKET myselfClient;
	struct sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	//char revData[255];


	while (1)
	{
		myselfClient = accept(myself, (SOCKADDR*)&remoteAddr, &nAddrlen);

		SOCKET* so = (SOCKET*)malloc(sizeof(SOCKET));
		*so = myselfClient;
		if (myselfClient == INVALID_SOCKET)
		{
			printf("accept error !\n");
			continue;
		}
		printf("connect success!\n");
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&tcpsend, so, 0, NULL);

	}

	closesocket(myselfClient);
	closesocket(fakesock);
	closesocket(realsock);
	WSACleanup();
	return 0;
}

int RealorFakeSend(PVOID per) {
	struct sockaddr_in realsockadd;
	struct sockaddr_in fakesockadd;
	fakesockadd.sin_family = AF_INET;
	fakesockadd.sin_port = htons(fakeportvalue);
	fakesockadd.sin_addr.S_un.S_addr = inet_addr(fakeipvalue);
	/*if (connect(fakesock, (struct sockaddr*)&fakesockadd, sizeof(fakesockadd)) == SOCKET_ERROR)
	{
		printf("fake socket connect error !\n");
		closesocket(fakesock);
		closesocket(myself);
		closesocket(realsock);
		return 0;
	}*/

	//real sock init

	realsockadd.sin_family = AF_INET;
	realsockadd.sin_port = htons(realportvalue);
	realsockadd.sin_addr.S_un.S_addr = inet_addr(realipvalue);
	/*if (connect(realsock, (struct sockaddr*)&realsockadd, sizeof(realsockadd)) == SOCKET_ERROR)
	{
		printf("connect real sock error !\n");
		closesocket(fakesock);
		closesocket(myself);
		closesocket(realsock);
		return 0;
	}*/



	SOCKET fakesocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKET realsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (realsocket == INVALID_SOCKET)
	{
		printf("socket error !\n");
		return 0;
	}
	if (fakesocket == INVALID_SOCKET)
	{
		printf("invalid socket !\n");
		return 0;
	}

	if (connect(realsocket, (struct sockaddr*)&realsockadd, sizeof(realsockadd)) == SOCKET_ERROR)
	{
		printf("error:connect real sock error !\n");
		closesocket(realsocket);
		closesocket(fakesocket);
		exit(0);
	}
	if (connect(fakesocket, (struct sockaddr*)&fakesockadd, sizeof(fakesockadd)) == SOCKET_ERROR)
	{
		printf("error:connect fake sock error !\n");
		closesocket(fakesocket);
		closesocket(realsocket);
		exit(0);
	}

	int flag = 0;
	while (true) {

		flag++;
		
		char* revData = (char*)malloc(0x10000);
		memset(revData, 0x00, 0x10000);
		int rett = recv(*(SOCKET*)per, revData, 0x10000, 0);
		if (rett <= 0)
		{
			printf("TIPS:server shut down!\n");
			closesocket(realsocket);
			closesocket(fakesocket);
			closesocket(*(SOCKET*)per);
			return 0;
		}



		char* srecData = (char*)malloc(0x2000);
		httpparse* httpp = new httpparse();
		httpparse* httpserver = new httpparse();
		httpserver->parse(revData);
		char* buff = (char*)malloc(200);
		memset(buff, 0, 100);
		httpserver->getflagvalue(httpflagvalue,buff);
		int fps = 0;
		for (size_t i = 0; i < strlen(passvalue); i++)
		{
			if (passvalue[i]==buff[i])
			{
				fps++;
			}
		}
		if ((fps==strlen(passvalue))||defaultpass) {
			int retsend = send(realsocket, revData, strlen(revData), 0);
			if (retsend <= 0)
			{
				printf("error:real send error!\n");
				closesocket(realsocket);
				closesocket(fakesocket);
				closesocket(*(SOCKET*)per);
				return 0;
			}
			int fleng = -1;
			int tongji = 0;
			while (1) {

				
				memset(srecData, 0x00, 0x2000);
				int ret = recv(realsocket, srecData, 0x2000, 0);
				tongji = tongji + ret;
				int retsend = send(*(SOCKET*)per, srecData, ret, 0);
				if (srecData[0] == 0x48 && srecData[1] == 0x54 && srecData[2] == 0x54 && srecData[3] == 0x50) {
					if (httpp->parse(srecData)) {
						fleng = httpp->getlength();
					}
				}

				
				if (retsend <= 0)
				{
					printf("error:send error!\n");
					closesocket(realsocket);
					closesocket(fakesocket);
					closesocket(*(SOCKET*)per);
					break;
				}
				if (ret <= 0)
				{
					printf("error:real recv error!\n");
					closesocket(realsocket);
					closesocket(fakesocket);
					closesocket(*(SOCKET*)per);
					break;
				}
				
				if (fleng != -1)
				{
					
					if (tongji == httpp->httpheaderlength + fleng + 4)
					{
		
						break;
					}
				}
				
			}

		}
		else {
			int retsend = send(fakesocket, revData, strlen(revData), 0);
			if (retsend <= 0)
			{
				printf("error:real send error!\n");
				closesocket(realsocket);
				closesocket(fakesocket);
				closesocket(*(SOCKET*)per);
				return 0;
			}
			int fleng = -1;
			int tongji = 0;
			while (1) {


				memset(srecData, 0x00, 0x2000);
				int ret = recv(fakesocket, srecData, 0x2000, 0);
				tongji = tongji + ret;
				int retsend = send(*(SOCKET*)per, srecData, ret, 0);
				if (srecData[0] == 0x48 && srecData[1] == 0x54 && srecData[2] == 0x54 && srecData[3] == 0x50) {
					if (httpp->parse(srecData)) {
						fleng = httpp->getlength();
					}
				}

				if (retsend <= 0)
				{
					printf("error:send error!\n");
					closesocket(realsocket);
					closesocket(fakesocket);
					closesocket(*(SOCKET*)per);
					break;
				}
				if (ret <= 0)
				{
					printf("error:real recv error!\n");
					closesocket(realsocket);
					closesocket(fakesocket);
					closesocket(*(SOCKET*)per);
					break;
				}

				if (fleng != -1)
				{

					if (tongji == httpp->httpheaderlength + fleng + 4)
					{
						break;
					}
				}

			}
		}

		
		free(srecData);
		free(revData);
	}

	closesocket(realsocket);
	closesocket(fakesocket);
	closesocket(*(SOCKET*)per);

	return 0;
}

int proxyserver() {
	//初始化WSA
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	SOCKET myself = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (myself == INVALID_SOCKET)
	{
		printf("self invalid socket !\n");
		return 0;
	}

	//real target
	fakesock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fakesock == INVALID_SOCKET)
	{
		printf("invalid socket !\n");
		return 0;
	}

	realsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (realsock == INVALID_SOCKET)
	{
		printf("socket error !\n");
		return 0;
	}

	//绑定IP和端口
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(portvalue);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(myself, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}








	//开始监听
	if (listen(myself, 100) == SOCKET_ERROR)
	{
		printf("listen error !\n");
		return 0;
	}

	//循环接收数据
	SOCKET myselfClient;
	struct sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	//char revData[255];


	while (1)
	{
		myselfClient = accept(myself, (SOCKADDR*)&remoteAddr, &nAddrlen);
		
		SOCKET* so = (SOCKET*)malloc(sizeof(SOCKET));
		*so = myselfClient;
		if (myselfClient == INVALID_SOCKET)
		{
			printf("accept error !\n");
			continue;
		}
		printf("connect success!\n");
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&RealorFakeSend, so, 0, NULL);

	}

	closesocket(myselfClient);
	closesocket(fakesock);
	closesocket(realsock);
	WSACleanup();
	return 0;
}

int main(int argc, char* argv[])
{

	printf("%s\n", logo);
	CmdParse* cmd = new CmdParse();
	cmd->parseinit(argc,argv);


	char portkey[] = "-port";
	char realipkey[] = "-rip";
	char fakeipkey[] = "-fip";
	char realportkey[] = "-rport";
	char fakeportkey[] = "-fport";

	char typekey[] = "-type";
	char httpflagkey[] = "-hflag";
	char passkey[] = "-pass";

	char tcpflagkey[] = "-tflag";

	realipvalue = cmd->get(realipkey);

	if (cmd->get(portkey) != NULL)
	{
		portvalue = atoi(cmd->get(portkey));
	}

	if (cmd->get(realportkey) != NULL)
	{
		realportvalue = atoi(cmd->get(realportkey));
	}
	fakeipvalue = cmd->get(fakeipkey);


	if (cmd->get(fakeportkey) != NULL)
	{
		fakeportvalue = atoi(cmd->get(fakeportkey));
	}

	typevalue = cmd->get(typekey);
	httpflagvalue = cmd->get(httpflagkey);
	passvalue = cmd->get(passkey);
	if (cmd->get(tcpflagkey) != NULL)
	{
		tcpflagvalue = atoi(cmd->get(tcpflagkey));
	}
	

	if (typevalue == NULL)
	{
		printf("error:no type please try \"-type http\" or \"-type tcp\"\n");
		return 0;
	}
	char tcpk[] = "tcp";
	char httpk[] = "http";
	int ft = 0;
	int fh = 0;
	for (size_t i = 0; i < strlen(typevalue); i++)
	{
		if (typevalue[i] == httpk[i]) {
			fh++;
		}
	}
	for (size_t i = 0; i < strlen(typevalue); i++)
	{
		if (typevalue[i] == tcpk[i]) {
			ft++;
		}
	}


	if (portvalue==NULL)
	{
		printf("warning:NULL start port now setting default 8879 port\n");
		portvalue = defaultport;
	}
	if(realipvalue==NULL)
	{
		printf("warning:NULL real target ip address now setting localhost\n");
		realipvalue = defaultip;
	}
	if (realportvalue==NULL)
	{
		printf("error:NULL real target port\n");
		return 0;
	}
	if (fakeportvalue==NULL)
	{
		printf("error:NULL fake target port\n");
		return 0;
	}
	if (fakeipvalue==NULL) 
	{
		printf("warning:NULL fake target ip address now setting localhost\n");
		fakeipvalue = defaultip;
	}
	if (fh == strlen(httpk))
	{
		printf("start type is http!\n");


		if (httpflagvalue == NULL)
		{
			printf("warning:NULL pass disposition default pass by real passage\n");
			defaultpass = 1;
		}


		proxyserver();
	}

	if (ft == strlen(tcpk))
	{
		printf("start type is tcp!");
		if (tcpflagvalue==NULL)
		{
			printf("error:NULL tcp bit flag");
			exit(0);
		}
		tcpserver();
	}


	if (fh != strlen(httpk) && ft != strlen(tcpk))
	{
		printf("error: error type please restart!\n");
		return 0;
	}


	
}