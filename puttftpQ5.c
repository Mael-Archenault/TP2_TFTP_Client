#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>

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

BUFF_SIZE = 512;

int main(int argc, char *argv[]) {
    char *host = argv[1];
    char *filepath = argv[2];
    char *port = argv[3];



    int fd;
    if ((fd = open(filepath, O_RDONLY, S_IRUSR)) == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    printf("\nFile opened successfully\n\r");
    char data[1024];
    int data_length;
    
    if ((data_length = read(fd, &data,1024))==-1){
        perror("read");
        exit(EXIT_FAILURE);
    }
    
   

    printf("Content : %s\n\n\r", data);

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
    request[1] = 2; 

    for (int i = 0; i < strlen(filepath); i++) {
        request[2+i] = filepath[i];
    }
    request[strlen(filepath) + 2] = 0;

    char mode[10] = "octet";
    
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

    struct sockaddr_storage src_addr;
    socklen_t src_addr_len = sizeof(src_addr);
    char ack[4];

    if ((recvfrom(sock, ack, 4, 0, (struct sockaddr *)&src_addr, &src_addr_len)) == -1) {
        perror("recvfrom");
        exit(EXIT_FAILURE);
    }

    
    if (ack[0] != 0 && ack[1] != 4) {
        printf("Error : Server did not acknowledge the request\n");
        exit(EXIT_FAILURE);
    }
    // // Loop to send all blocks
    for (int block_number = 1; block_number <= data_length / BUFF_SIZE + 1; block_number++) {
        int data_packet_successfully_sent = 0;
        printf("Block number : %d\n", block_number);
        while (!data_packet_successfully_sent) {

            

            char dataFrame[516];
            dataFrame[0] = 0;
            dataFrame[1] = 3;
            dataFrame[2] = 0;
            dataFrame[3] = block_number;
            for (int i=0; i<BUFF_SIZE; i++){
                dataFrame[4+i] = data[BUFF_SIZE*(block_number-1)+ i];
            }

            if (sendto(sock, dataFrame, data_length + 4, 0, (struct sockaddr *)&src_addr, res->ai_addrlen) == -1) {
                perror("sendto");
                exit(EXIT_FAILURE);
            }
            
            
            if ((recvfrom(sock, ack, 4, 0, (struct sockaddr *)&src_addr, &src_addr_len)) == -1) {
                perror("recvfrom");
                exit(EXIT_FAILURE);
            }

            if (ack[0] == 0 && ack[1]== 4 && ack[2] == 0 && ack[3] == block_number) {
                data_packet_successfully_sent = 1;
                printf("Block %d sent successfully\n", block_number);
            }

        }


    }

    printf("\n\r");
    close(sock);
    exit(EXIT_SUCCESS);

}