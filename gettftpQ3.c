#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    char *host = argv[1];
    char *filepath = argv[2];

    printf("Server : %s\nFilepath : %s\n", host, filepath);
    
    // Setting the filter of addresses
    struct addrinfo hints = {0};
    hints.ai_family = AF_INET;
    hints.ai_protocol = IPPROTO_UDP;
    hints.ai_socktype = SOCK_DGRAM;

    // Getting the adresses
    struct addrinfo *res;
    if (getaddrinfo(host, NULL, &hints, &res)==-1){
        perror(gai_strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (res==NULL){
        perror("no address found\n");
        exit(EXIT_SUCCESS);
    
    }

    // Getting the IP address from the adress
    char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
    if (getnameinfo(res->ai_addr, res->ai_addrlen, hbuf, sizeof(hbuf), sbuf,sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV) == 0){
        printf("IP adress : %s\n", hbuf);
    }


    // Creating a socket
    int sock;
    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    // // Connecting to the server
    // if (connect(sock, res->ai_addr, res->ai_addrlen) == -1) {
    //     perror("connect");
    //     exit(EXIT_FAILURE);
    // }

    printf("Connected to server...\n");



}