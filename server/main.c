#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include "caspp.h"
#include <stdlib.h>


int main(int argc, char* argv[])
{
    char hostname[256], port[256];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    int fd = open_server(argv[1]);
    fprintf(stdout, "fd = %d\n", fd);
    while (1) {
        /*等待并接受客户端client 的连接*/
        int connfd = accept(fd, (struct sockaddr*)&clientaddr, &clientlen);
        if (connfd < 0) {
            perror("accept error!");
            continue;
        }

        getnameinfo((struct sockaddr*)&clientaddr, clientlen, hostname, 256, port, 256, 0);
        printf("Connected to %s:%s\n", hostname, port);

        /*创建子进程 处理客户端的连接*/
        if (0 == fork()) {
            /*进入到子进程中*/
            close(fd);                  //子进程不需要 监听socket
            printf("In child pid = %d\n", getpid());
            //TODO:  处理来自客户端 的内容
            while (1) {
                char usrBuf[1024];
                read(connfd, usrBuf , sizeof(usrBuf));
                fprintf(stdout, "form client:\n %s \n", usrBuf);
            }
            close(connfd);              //处理完毕, 关闭连接
            exit(0);
        }
        
        //父进程
        close(connfd);

    }
    return 0;
}
