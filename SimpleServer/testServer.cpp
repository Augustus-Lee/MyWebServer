#include<stdio.h>
#include<stdlib.h>

const int PORT = 8888
int main(int argc, char *argv[])
{
	if(argc < 0)
	{
		printf("need two parameter!");
		return 1;
	}

	int listenfd;
	int connfd;
	struct sockaddr_in sever_address;
	bzero(&sever_address, sizeof(sever_address));
	sever_address.sin_famliy = PF_INET；
	//INADDR_ANY本地任意IP地址
	sever_address.sin_addr.s_addr = htonl(INADDR_ANY);
	//主机字节序转网络字节序
	sever_address.sin_port = htons(PORT);

	listenfd = socket(PF_INET, SOCK_STREAM, O);

	int ret = bind(listenfd, (struct * sockaddr *)&sever_address, sizeof(sever_address));
	assert(ret >= 0);

	ret = listen(listenfd, 5);
	assert(ret >= 0);

	while(1)
	{
		struct sockadd_in clien_address;
		socklen_t client_addrLen = sizeof(clien_address);
		//从 listen 监听队列中接受一个连接
		connfd = accept(listenfd, (struct sockadd_in *)&clien_address, &client_addrLen);
		if(connfd < 0)
		{
			printf("errno:%d\n", errno);
		}
		else
		{
			char request[2048];
			recv(connfd, request, 2048, 0);
			request[strlen(request) + 1] = '\0';
			printf("%s\n", request);
			printf("successeful!\n");
			char buf[520]="HTTP/1.1 200 ok\r\nconnection: close\r\n\r\n";//HTTP响应
			int s = send(connfd,buf,strlen(buf),0);//发送响应
			int fd = open("hello.html",O_RDONLY);//消息体
            sendfile(connfd,fd,NULL,2500);//零拷贝发送消息体
            close(fd);
            close(connfd);
            close(listenfd)

		}


	}

	return 0;


}