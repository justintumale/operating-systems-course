/*
 * In C, write a server and client that implement the fictitious "echo protocol". To implement the protocol, the
 * server should accept any string from the client, and then return that string with all letters capitalized
 * (if letters exist).
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

int main() {
    int socket_fd;
    int client_socket_fd;
    int port_number;
    int client_length;

    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    //Create server socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("Error creating server socket");
        return -1;
    }

    //Initialize the socket structure
    bzero((char*)&server_address, sizeof(server_address));
    port_number = 6000;

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = IN_ANY;
    server_address.sin_port = htons(port_number);

    //Bind socket to server address
    int bind = bind(socket_fd, (struct sockaddr*) server_address, sizeof(server_address));
    if (bind < 0) {
        perror("Error binding server socket");
        return -1;
    }

    //Listen for clients
    listen(socket_fd, 5);

    //Create client socket through accepted connection
    client_socket_fd = accept(socket_fd, (struct sockaddr*)&client_address, &client_length);
    if (client_socket_fd < 0) {
        perror("Error establishing connection with client socket");
        return -1;
    }

    //If connection is established, start communication
    char buffer[256];
    bzero(buffer, 256);
    n = read(client_socket_fd, buffer, 255);

    if (n < 0) {
        perror("Error writing to client socket");
        return -1;
    }

    return 0;
}