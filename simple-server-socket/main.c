#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

/*
 * Write a simple C program that creates and initializes a server socket. Once initialized, the server should accept a
 * client connection, close the connection, and then exit. You should provide a way to specify the server's listening
 * port. This can be hardcoded or passed via the command line.
 */

int main() {
    int socket_fd;
    int newsocket_fd;
    int port_number;
    int client_length;

    char buffer[256];

    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    int n;

    //1. Create the socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        printf("Error opening the socket.");
        return -1;
    }

    //2. Initialize socket structure
    bzero((char*) &server_address, sizeof(server_address);
    port_number = 6000;

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port_number);

    return 0;
}