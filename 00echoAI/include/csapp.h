#ifndef __CSAPP_H__
#define __CSAPP_H__

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <errno.h>
#include <netdb.h>

#define MAXLINE 8192
#define RIO_BUFSIZE 8192

/* Rio package - Robust I/O functions */
typedef struct {
    int rio_fd;                /* descriptor for this internal buf */
    int rio_cnt;               /* unread bytes in internal buf */
    char *rio_bufptr;          /* next unread byte in internal buf */
    char rio_buf[RIO_BUFSIZE]; /* internal buffer */
} rio_t;

/* 鲁棒性 I/O函数 RIO */
void rio_readinitb(rio_t *rp, int fd);
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n);

/*
** @brief: 从描述符 @fd 的当前位置最多传送 @n 个字节到内存位置 @usrbuf。
** 在遇到 EOF 时，只能返回一个不足值。
*/
ssize_t rio_readn(int fd, void* usrbuf, size_t n);

/*
** @brief： 从位置 @usrbuf 传送 @n 个字节到描述符 @fd
** 绝不会返回 不足值。
*/
ssize_t rio_writen(int fd, void *usrbuf, size_t n);
/* !@鲁棒性 I/O函数 RIO */

/* Socket functions */
int open_clientfd(char* hostname, char* port);
int open_listenfd(char* port);

#endif // !__CSAPP_H__
