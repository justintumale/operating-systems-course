/*
 * In C, write a server and client that implement the fictitious "echo protocol". To implement the protocol, the
 * server should accept any string from the client, and then return that string with all letters capitalized
 * (if letters exist).
 */

#include <stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>

int main() {
    //Create client socket
    printf("Creating client socket\n");
    int client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket_fd < 0) {
        perror("Error creating client socket\n");
        return -1;
    }

    //Initialize socketaddr_in data structure
    printf("Initializing socketaddr in data structure\n");
    struct sockaddr_in server_socket;
    server_socket.sin_family = AF_INET;
    server_socket.sin_port = htons(6000);
    server_socket.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Connecting to server...\n");
    int connected = connect(client_socket_fd, (struct sockaddr*) &server_socket, sizeof(server_socket));
    if (connected < 0) {
        perror("Error connecting to server socket");
        return -1;
    }
    printf("Connected to server.");

    /*
    char buffer[1024];
    strcpy(buffer, "Hello World\n");
    send(client_socket_fd, buffer, 13, 0);
    */

    return 0;
}