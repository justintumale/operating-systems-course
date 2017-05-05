#include <stdio.h>
#include<sys/socket.h>

/*
 * Write a simple C program that creates, initializes, and connects a client socket to a server socket.
 * You should provide a way to specify the connecting server address and port. This can be hardcoded or
 * passed via the command line.
 */

int main() {
    //Create the client socket
    int socket_desc;
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
        //Address Family -AF_INET(this is IP version 4)
        //Type - SOCK_STREAM (this means connection oriented TCP protocol)
        //Protocol - 0 (or IPPROTO_IP, this is IP protocol)
    if (socket_desc == -1) {
        printf("Could not create socket.");
    }
    return 0;
}