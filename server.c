#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    
WSADATA wsa;
SOCKET server_fd, client_socket;
struct sockaddr_in server_addr, client_addr;
int addrlen = sizeof(client_addr);
char buffer[BUFFER_SIZE] = {0};

//Initializing Winsock.
if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
    printf("WSAStartup failed!\n");
    return 1;
}

//Creating a socket.
if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
    printf("Socket creation failed!\n");
    WSACleanup();
    return 1;
}

//Configuring the server address architecture.
server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = INADDR_ANY;
server_addr.sin_port = htons(PORT);

//Binding the socket.
if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
    printf("Bind failed!\n");
    closesocket(server_fd);
    WSACleanup();
    return 1;
}

//Listening for incoming connections.
if (listen(server_fd, SOMAXCONN) == SOCKET_ERROR) {
    printf("Listen failed!\n");
    closesocket(server_fd);
    WSACleanup();
    return 1;
}
printf("Server listening on port %d...\n", PORT);

//Accepting the connection from the client.
client_socket = accept(server_fd, (struct sockaddr*)&client_addr, &addrlen);
if (client_socket == INVALID_SOCKET) {
    printf("Accept failed!\n");
    closesocket(server_fd);
    WSACleanup();
    return 1;
}
printf("Client connected!\n");

//Receiving the client data and outputting it.
int bytes_read;
while ((bytes_read = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
    printf("Received: %s", buffer);
    send(client_socket, buffer, bytes_read, 0);
    memset(buffer, 0, BUFFER_SIZE);
}

//End of communications - closing sockets and exiting program.
closesocket(client_socket);
closesocket(server_fd);
WSACleanup();
return 0;

}
