#include "csapp.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>

// 函数声明
void echo(int connfd);
void sigchld_handler(int sig);

int main(int argc, char* argv[])
{
    // 有一个监听文件描述符
    // 有一个连接文件描述符
    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    // 检查命令行参数
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    // 创建监听socket
    listenfd = open_listenfd(argv[1]);
    if (listenfd < 0) {
        fprintf(stderr, "open_listenfd error\n");
        exit(1);
    }

    printf("Echo Server started on port %s\n", argv[1]);
    printf("Waiting for connections...\n");

    //TODO: 不明白
    // 设置 SIGCHLD 信号处理，避免僵尸进程
    signal(SIGCHLD, sigchld_handler);

    // 主循环：接受客户端连接
    while (1) {
        clientlen = sizeof(clientaddr);

        // 等待并接受客户端连接
        connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
        if (connfd < 0) {
            if (errno == EINTR) {
                // 被信号中断，继续循环
                continue;
            } else {
                perror("accept error");
                continue;
            }
        }

        // 获取客户端信息
        getnameinfo((struct sockaddr *)&clientaddr, clientlen,
                   hostname, MAXLINE, port, MAXLINE, 0);
        printf("Connected to %s:%s\n", hostname, port);

        // 创建子进程处理客户端连接
        if (fork() == 0) {
            // 子进程
            close(listenfd);  // 子进程不需要监听socket
            echo(connfd);     // 处理echo服务
            close(connfd);    // 处理完毕，关闭连接
            printf("Connection with %s:%s closed\n", hostname, port);
            exit(0);          // 子进程退出
        }

        // 父进程
        close(connfd);  // 父进程不需要连接socket
    }

    // 永远不会执行到这里
    close(listenfd);
    return 0;
}

// Echo 函数：接收客户端数据并回传
void echo(int connfd)
{
    size_t n;
    char buf[MAXLINE];
    rio_t rio;

    // 初始化rio缓冲区
    rio_readinitb(&rio, connfd);

    // 循环读取客户端数据并回传
    while ((n = rio_readlineb(&rio, buf, MAXLINE)) != 0) {
        printf("Received %zu bytes: %s", n, buf);

        // 将数据回传给客户端
        rio_writen(connfd, buf, n);

        if (strstr(buf, "quit") != NULL) {
            printf("Client requested to quit\n");
            break;
        }
    }
}

// SIGCHLD 信号处理函数：避免僵尸进程
void sigchld_handler(int sig)
{
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
    return;
}
