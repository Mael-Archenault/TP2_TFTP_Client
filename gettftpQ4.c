#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char *host = argv[1];
    char *filepath = argv[2];
    char *port = argv[3];

    printf("Server : %s\nFilepath : %s\n", host, filepath);
    
    // Setting the filter of addresses
    struct addrinfo hints = {0};
    hints.ai_family = AF_INET;
    hints.ai_protocol = IPPROTO_UDP;
    hints.ai_socktype = SOCK_DGRAM;

    // Getting the adresses
    struct addrinfo *res;
    if (getaddrinfo(host, port, &hints, &res)==-1){
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

    // Connecting to the server
    if (connect(sock, res->ai_addr, res->ai_addrlen) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server...\n");

    // Creating the request packet

    char request[512];

    request[0] = 0;
    request[1] = 1; 

    for (int i = 0; i < strlen(filepath); i++) {
        request[2+i] = filepath[i];
    }
    request[strlen(filepath) + 2] = 0;

    char mode[5] = "octet";
    
    for (int i = 0; i < strlen(mode); i++) {
        request[strlen(filepath) + 3 + i] = mode[i];
    }
    request[strlen(filepath) + 3 + strlen(mode)] = 0;
    
    // Sending the request packet
    
    int request_length = strlen(filepath) + 3 + strlen(mode) + 1;
    if (sendto(sock, request, request_length, 0, res->ai_addr, res->ai_addrlen) == -1) {
        perror("sendto");
        exit(EXIT_FAILURE);
    }
    // if (recvfrom(sock, request, strlen(request), 

}