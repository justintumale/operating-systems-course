#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
    bzero((char*) &server_address, sizeof(server_address));
    port_number = 6000;

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port_number);

    //3. Bind the host address using the bind call
    if (bind(socket_fd, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
        perror("Error binding the socket to the host address");
        return -1;
    }

    //4. Program socket to start listening for clients
        //have this thread sleep until a client attempts a connection
    listen(socket_fd, 5);

        //accept connection from client
    client_length = sizeof(client_address);
    newsocket_fd = accept(socket_fd, (struct sockaddrr*)&client_address, &client_length);

    if (newsocket_fd < 0) {
        perror("Error accepting client socket connection");
        return -1;
    }

    //5. If connection is established, start communicating
    bzero(buffer, 256);
    n = read(newsocket_fd, buffer, 255);

    if (n<0) {
        perror("Error writing to socket");
        return -1;
    }

    return 0;
}