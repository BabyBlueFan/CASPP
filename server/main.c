#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include "caspp.h"


int main(int argc, char* argv[])
{
    char* port = argv[1];
    int fd = open_server("8888");
    fprintf(stdout, "fd = %d\n", fd);
    while (1) {

    }
    return 0;
}
