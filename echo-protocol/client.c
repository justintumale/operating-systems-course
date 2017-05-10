/*
 * In C, write a server and client that implement the fictitious "echo protocol". To implement the protocol, the
 * server should accept any string from the client, and then return that string with all letters capitalized
 * (if letters exist).
 */

#include <stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main() {
    //Create client socket
    int client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket_fd < 0) {
        perror("Error creating client socket");
        return -1;
    }

    //Initialize socketaddr_in data structure
    struct sockaddr_in server_socket;
    server_socket.sin_family = AF_INET;
    server_socket.sin_port = htons(80);
    server_socket.sin_addr.s_addr = inet("XX.XX.XX.XX");

    int connect = connect(client_socket_fd, (struct sockaddr*) &server_socket, sizeof(server_socket));
    if (connect < 0) {
        perror("Error connecting to server socket");
        return -1;
    }


    return 0;
}