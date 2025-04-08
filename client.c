#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define SERVER_IP #Your IP address
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
     WSADATA wsa;
     SOCKET client_fd;
     struct sockaddr_in server_addr;
     char buffer[BUFFER_SIZE] = { 0 };
     char message[BUFFER_SIZE];

     // Initializing Winsock.
     if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
          printf("WSAStartup failed!\n");
          return 1;
     }

     // Creating the socket.
     if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
          printf("Socket creation failed!\n");
          WSACleanup();
          return 1;
     }

     // Configuring the server address architecture.
     server_addr.sin_family = AF_INET;
     server_addr.sin_port = htons(PORT);
     server_addr.sin_addr.s_addr = inet_addr(SERVER_IP); // Using inet_addr() for Windows compatibility

     // Connect to the server.
     if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
          printf("Connection failed!\n");
          closesocket(client_fd);
          WSACleanup();
          return 1;
     }
     printf("Connected to server at %s:%d\n", SERVER_IP, PORT);

     // Send and receive data.
     while (1) {
          printf("Enter message (type 'exit' to quit): ");
          fgets(message, BUFFER_SIZE, stdin);

          // Remove newline character if present
          message[strcspn(message, "\n")] = 0;

          if (strncmp(message, "exit", 4) == 0) {
               break;
          }

          // Send the message to the server
          if (send(client_fd, message, strlen(message), 0) == SOCKET_ERROR) {
               printf("Send failed!\n");
               break;
          }

          // Receive the server's response
          int bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
          if (bytes_received > 0) {
               buffer[bytes_received] = '\0'; // Null terminate the received string
               printf("Server echoed: %s\n", buffer);
          }
          else if (bytes_received == 0) {
               printf("Server closed the connection.\n");
               break;
          }
          else {
               printf("Recv failed!\n");
               break;
          }

          memset(buffer, 0, BUFFER_SIZE); // Clear the buffer for next message
     }

     // End of communications - closing sockets and exiting program.
     closesocket(client_fd);
     WSACleanup();
     return 0;
}
