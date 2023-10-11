#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define N 128

#define ERRLOG(errmsg) do{\
		printf("%s--%s(%d):", __FILE__, __func__, __LINE__);\
		perror(errmsg);\
		exit(-1);\
	}while(0)

int main(int argc, const char *argv[])
{
    //检测命令行参数个数
    if(3 != argc){
        printf("Usage : %s <IP> <PORT>\n", argv[0]);
        exit(-1);
    }

    //1.创建套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sockfd)
        ERRLOG("socket error");


    //2.填充服务器网络信息结构体
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    //端口号  填 8888 9999 6789 ...都可以
    server_addr.sin_port = htons(atoi(argv[2]));
    //ip地址 要么是当前Ubuntu主机的IP地址 或者
    //如果本地测试的化  使用  127.0.0.1 也可以
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    //结构体长度
    socklen_t server_addr_len = sizeof(server_addr);

    //与服务器建立连接
    if(-1 == connect(sockfd, (struct sockaddr *)&server_addr, server_addr_len))
        ERRLOG("connect error");

    printf("---连接服务器成功---\n");
    char buff[128] = {0};
//    fgets(buff,N,stdin);

    while(1){
        scanf("%s", buff);
//        printf("%d个字符\n", strlen(buff));
        int ret = 0;
        if(0 > (ret = send(sockfd, buff,sizeof(buff), 0)))
            ERRLOG("send error");

        if(0 == strcmp(buff, "quit"))
            break;

        if(0 > (ret = recv(sockfd, buff,sizeof(buff), 0)))
            ERRLOG("recv error");

        printf("收到服务器回复:[%s]\n", buff);
    }
    //关闭套接字
    close(sockfd);

    return 0;
}