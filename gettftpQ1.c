#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    char *host = argv[1];
    char *filepath = argv[2];

    printf("Server : %s\nFilepath : %s\n", host, filepath);
}