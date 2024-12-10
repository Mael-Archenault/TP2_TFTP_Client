#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    char *host = argv[1];
    char *filepath = argv[2];

    printf("Server : %s\nFilepath : %s", host, filepath);
    
    struct addrinfo hints = {0};
    struct addrinfo *res;
    if (getaddrinfo(host, NULL, &hints, &res)==-1){
        perror(gai_strerror(errno));
        exit(EXIT_FAILURE);
    }

    while (res->ai_next!= NULL){
        printf("Family: %d\n", res->ai_family);
        printf("Type: %d\n", res->ai_socktype);
        printf("Protocol: %d\n", res->ai_protocol);
        printf("\n");
        res = res->ai_next;
    }

}