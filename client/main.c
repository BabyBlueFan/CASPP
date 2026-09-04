#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

int open_client(const char* ip, const char* port);

int main(int argc, char* argv[])
{
    char* ip = argv[1];
    char* port = argv[2];
    int fd = open_client("127.0.0.1", "8888");
    fprintf(stdout, "fd = %d\n", fd);
    return 0;
}

int open_client(const char* ip, const char* port)
{
    int fd = 0;
    struct addrinfo hint, *addrList, *addrptr;
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    getaddrinfo(ip, port, &hint, &addrList);
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
