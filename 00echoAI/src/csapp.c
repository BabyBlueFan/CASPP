#include "csapp.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifndef LISTENQ
#define LISTENQ 1024  /* 监听队列的最大长度 */
#endif

/*
 * rio_readinitb - Associate a descriptor with a read buffer and reset buffer
 */
void rio_readinitb(rio_t *rp, int fd)
{
    rp->rio_fd = fd;
    rp->rio_cnt = 0;
    rp->rio_bufptr = rp->rio_buf;
}

/*
 * rio_read - This is a wrapper for the Unix read() function that
 *    transfers min(n, rio_cnt) bytes from an internal buffer to a user
 *    buffer, where n is the number of bytes requested by the user and
 *    rio_cnt is the number of unread bytes in the internal buffer. On
 *    entry, rio_read() refills the internal buffer via the Unix read()
 *    function if the internal buffer is empty.
 */
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n)
{
    int cnt;

    while (rp->rio_cnt <= 0) { /* Refill if buf is empty */
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf,
                           sizeof(rp->rio_buf));
        if (rp->rio_cnt < 0) {
            if (errno != EINTR) /* Interrupted by sig handler return */
                return -1;
        }
        else if (rp->rio_cnt == 0)  /* EOF */
            return 0;
        else
            rp->rio_bufptr = rp->rio_buf; /* Reset buffer ptr */
    }

    /* Copy min(n, rp->rio_cnt) bytes from internal buf to user buf */
    cnt = n;
    if (rp->rio_cnt < n)
        cnt = rp->rio_cnt;
    memcpy(usrbuf, rp->rio_bufptr, cnt);
    rp->rio_bufptr += cnt;
    rp->rio_cnt -= cnt;
    return cnt;
}

/*
 * rio_readlineb - Robustly read a text line (buffered)
 */
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
    int n, cnt;
    char c, *buf = usrbuf;

    for (n = 1; n < maxlen; n++) {
        if ((cnt = rio_read(rp, &c, 1)) == 1) {
            *buf++ = c;
            if (c == '\n')
                break;
        } else if (cnt == 0) {
            if (n == 1)
                return 0; /* EOF, no data read */
            else
                break;    /* EOF, some data was read */
        } else
            return -1;    /* Error */
    }
    *buf = 0;
    return n;
}

/*
 * rio_readnb - Robustly read n bytes (buffered)
 */
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;

    while (nleft > 0) {
        if ((nread = rio_read(rp, bufp, nleft)) < 0)
            return -1;          /* Error */
        else if (nread == 0)
            break;              /* EOF */
        nleft -= nread;
        bufp += nread;
    }
    return (n - nleft);         /* Return >= 0 */
}

ssize_t rio_readn(int fd, void *usrbuf, size_t n)
{
    size_t nleft = n;                                   //剩余没有读的字节数
    ssize_t nread;                                      //本次已经读的字节数
    char* bufp = usrbuf;                                //永远指向usrbuf内存区下一个打算写入的字节/还没被写入的第一个字节

    while (nleft > 0)
    {
        if ((nread = read(fd, bufp, nleft)) < 0)
        {
            if (errno == EINTR)                         //是被系统信号打断了， 可以继续
            {
                nread = 0;                              //本次读取字节数为0
            }
            else                                        //否则， 发生系统错误
            {
                return -1;
            }
        }
        else if (nread == 0)                            //表示遇到了文件结束符 EOF 可以直接结束读取了
        {
            break;
        }
        nleft -= nread;
        bufp += nread;
    }
    return (n - nleft);
}

/*
 * rio_writen - Robustly write n bytes (unbuffered)
 */
ssize_t rio_writen(int fd, void *usrbuf, size_t n)
{
    size_t nleft = n;                                       // 剩余没有写入到fd的字节数
    ssize_t nwritten;                                       // 本次调用系统函数write写入到fd的字节数
    char *bufp = usrbuf;                                    // 指向下一次写入到fd 的内存区域

    while (nleft > 0) {
        if ((nwritten = write(fd, bufp, nleft)) <= 0) {
            if (errno == EINTR)  /* Interrupted by sig handler return */
                nwritten = 0;    /* Call write() again */
            else
                return -1;       /* Error */
        }
        nleft -= nwritten;
        bufp += nwritten;
    }
    return n;
}

int open_clientfd(char *hostname, char *port)
{
    int clientfd;
    struct addrinfo hints, *listp, *p;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;                        //Open a conncettion
    hints.ai_flags = AI_NUMERICSERV;
    hints.ai_flags |= AI_ADDRCONFIG;

    if (getaddrinfo(hostname, port, &hints, &listp) != 0) {
        return -1;
    }

    for (p = listp; p; p = p->ai_next)
    {
        if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
        {
            continue;
        }

        if (connect(clientfd, p->ai_addr, p->ai_addrlen) != -1)
        {
            break;
        }
        close(clientfd);
        clientfd = -1;
    }

    freeaddrinfo(listp);
    if (!p)
    {
        return -1;
    }
    else
    {
        return clientfd;
    }
}

int open_listenfd(char* port)
{
    struct addrinfo hints, *listp, *p;
    int listenfd, optval = 1;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;

    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;
    hints.ai_flags |= AI_NUMERICSERV;

    if (getaddrinfo(NULL, port, &hints, &listp) != 0) {
        return -1;
    }

    for (p = listp; p; p = p->ai_next)
    {
        //根据套接字类型申请 文件描述符
        if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
        {
            continue;
        }
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(int));

        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
        {
            break;
        }
        close(listenfd);
    }

    freeaddrinfo(listp);

    if (!p)
    {
        return -1;
    }

    if (listen(listenfd, LISTENQ) < 0)
    {
        close(listenfd);
        return  -1;
    }

    return  listenfd;
}
