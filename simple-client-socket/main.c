#include <stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>

/*
 * Write a simple C program that creates, initializes, and connects a client socket to a server socket.
 * You should provide a way to specify the connecting server address and port. This can be hardcoded or
 * passed via the command line.
 */

int main() {
    //Create the client socket
    int socket_desc;
    printf("creating socket...");
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
        //Address Family -AF_INET(this is IP version 4)
        //Type - SOCK_STREAM (this means connection oriented TCP protocol)
        //Protocol - 0 (or IPPROTO_IP, this is IP protocol)
    if (socket_desc == -1) {
        printf("Could not create socket.");
    }
    printf("socket created");

    //Connect to a server
        //need ip address and port number to connect to

        //1. Create a sockaddr_in structure
    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr("74.125.235.20");
    server.sin_family = AF_INET;
    server.sin_port = htons(80);

        //2. Connect to remote server
    printf("Connecting...");
    if (connect(socket_desc, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("connect error");
        return 1;
    }

    printf("Connected");

    return 0;
}