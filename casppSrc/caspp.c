#include "caspp.h"
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>

int open_client(const char* ip, const char* port)
{
    int fd = 0;
    struct addrinfo hint, *addrList, *addrptr;
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    int ret = getaddrinfo(ip, port, &hint, &addrList);
    if (ret != 0)
    {
        fprintf(stderr, "getaddinfo 错误!\n");
        return -1;
    }
    for (addrptr = addrList; addrptr != NULL; addrptr = addrptr->ai_next)
    {
        fd = socket(addrptr->ai_family, addrptr->ai_socktype, addrptr->ai_protocol);
        if (fd < 0) {
            continue;
        }
        if (connect(fd, addrptr->ai_addr, addrptr->ai_addrlen) == 0) {
            //l连接成功
            break;
        }
        close(fd);
        fd = -1;
    }
    freeaddrinfo(addrList);
    return fd;
}

int open_server(const char* arg_port)
{
    int fd = 0;
    struct addrinfo hint;
    struct addrinfo* plist;
    struct addrinfo* ptr;

    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_flags = AI_PASSIVE;

    int res = getaddrinfo(NULL, arg_port, &hint, &plist);
    if (res != 0) {
        fprintf(stderr, "In open_server, getaddinfo Error!\n");
        return -1;
    }
    
    for (ptr = plist; ptr != NULL; ptr = ptr->ai_next) {
        fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (fd < 0) {
            continue;
        }
        
        if (bind(fd, ptr->ai_addr, ptr->ai_addrlen) == 0)
        {
            break;
        }
        close(fd);
    }

    freeaddrinfo(plist);

    if (listen(fd, 1024) != 0)
    {
        fprintf(stderr, "listen Error!\n");
        close(fd);
        return -1;
    }

    return fd;
}
