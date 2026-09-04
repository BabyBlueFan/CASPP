#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include "caspp.h"


int main(int argc, char* argv[])
{
    char* ip = argv[1];
    char* port = argv[2];
    int fd = open_client("127.0.0.1", "8888");
    fprintf(stdout, "fd = %d\n", fd);
    return 0;
}
