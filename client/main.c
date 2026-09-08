#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include "caspp.h"


int main(int argc, char* argv[])
{
    char* ip = argv[1];
    char* port = argv[2];
    int fd = open_client("localhost", "8888");
    fprintf(stdout, "fd = %d\n", fd);
    while (1) {
        char usrBuf[1024];
        fgets(usrBuf, 1023, stdin);
        write(fd, usrBuf, sizeof(usrBuf));
    }
    return 0;
}
