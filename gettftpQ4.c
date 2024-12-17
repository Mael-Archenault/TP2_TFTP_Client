#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>

// Reminder of the addrinfo structure 

// struct addrinfo {
//     int ai_flags;
//     int ai_family;
//     int ai_socktype;
//     int ai_protocol;
//     socklen_t ai_addrlen;
//     struct sockaddr *ai_addr;
//     char *ai_canonname;
//     struct addrinfo *ai_next;
// };

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
    if (getnameinfo(res->ai_addr, res->ai_addrlen, hbuf, sizeof(hbuf), sbuf,sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV) == -1){
        printf("IP address : %s\n", hbuf);
    }


    // Creating a socket
    int sock;
    if ((sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Creating the request packet

    char request[512];

    request[0] = 0;
    request[1] = 1; 

    for (int i = 0; i < strlen(filepath); i++) {
        request[2+i] = filepath[i];
    }
    request[strlen(filepath) + 2] = 0;

    char mode[10] = "octet";

    // printf("%d \n\r", strlen(filepath));
    // printf("%d \n\r", strlen(mode));
    
    for (int i = 0; i < strlen(mode); i++) {
        request[strlen(filepath) + 3 + i] = mode[i];
    }
    request[strlen(filepath) + 3 + strlen(mode)] = 0;
    
    // Sending the request packet
    
    int request_length = strlen(filepath)+ strlen(mode)+4;

    if (sendto(sock, request, request_length, 0, res->ai_addr, res->ai_addrlen) == -1) {
        perror("sendto");
        exit(EXIT_FAILURE);
    }
    
    char response[516];
    int response_length = 516;

    printf("\nFile : \n\r");

    // Loop to receive all blocks
    while (response_length >= 516){


        // Extracting the source address
        struct sockaddr_storage src_addr;
        socklen_t src_addr_len = sizeof(src_addr);

        // Receiving the response packet
        if ((response_length = recvfrom(sock, response, 516, 0, (struct sockaddr *)&src_addr, &src_addr_len)) == -1) {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < response_length; i++) {
            printf("%c", response[i]);
        }
        

        char ack[4];
        ack[0] = 0;
        ack[1] = 4;
        ack[2] = response[2];
        ack[3] = response[3];

        if (sendto(sock, ack, 4, 0, (struct sockaddr *)&src_addr, res->ai_addrlen) == -1) {
            perror("sendto");
            exit(EXIT_FAILURE);
        }

    }
    printf("\n\r");
    close(sock);
    exit(EXIT_SUCCESS);

}
