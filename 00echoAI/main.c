#include "csapp.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int clientfd;
    char *host, *port, buf[MAXLINE];
    rio_t rio;

    if (argc != 3) {
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
        exit(1);
    }

    host = argv[1];
    port = argv[2];

    clientfd = open_clientfd(host, port);
    if (clientfd < 0) {
        fprintf(stderr, "open_clientfd error\n");
        exit(1);
    }

    // 初始化rio_t结构体
    rio_readinitb(&rio, clientfd);

    printf("Connected to %s:%s\n", host, port);
    printf("Enter text to send (Ctrl+D to exit):\n");

    // 从标准输入读取用户输入并发送到服务器
    while (fgets(buf, MAXLINE, stdin) != NULL) {
        // 发送到服务器
        rio_writen(clientfd, buf, strlen(buf));

        // 从服务器读取响应并打印
        ssize_t n;
        while ((n = rio_readlineb(&rio, buf, MAXLINE)) > 0) {
            printf("Server response: %s", buf);
            if (n < MAXLINE && buf[n-1] == '\n')
                break; // 完整的一行
        }
    }

    close(clientfd);
    printf("Connection closed.\n");
    exit(0);
}
