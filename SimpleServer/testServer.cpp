#include<stdio.h>
#include<stdlib.h>


int main(int argc, char *argv[])
{
	if(argc < 0)
	{
		printf("need two parameter!");
		return 1;
	}

	int sock;
	int connfd;
	struct sockaddr_in sever_address;
	bzero(&sever_address, sizeof(sever_address));
	sever_address.sin_famliy = PF_INET；
	//INADDR_ANY本地任意IP地址
	sever_address.sin_addr.s_addr = htons(INADDR_ANY);
	//主机字节序转网络字节序
	sever_address.sin_port = htons(8888)


}