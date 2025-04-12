#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/applink.c>

#define SERVER_IP "0.0.0.0" //Change to your IP address.
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = { 0 };
    char message[BUFFER_SIZE];


    // Windows socket API error handling.
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup failed!\n");
        return 1;
    }

    

    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();

    // SSL method/context error handling.
    const SSL_METHOD *method = TLS_client_method();
    SSL_CTX *ctx = SSL_CTX_new(method);
    if (!ctx) {
        ERR_print_errors_fp(stderr);
        return 1;
    }



    // Load local client certificate and key.
    if (SSL_CTX_use_certificate_file(ctx, "client-cert.pem", SSL_FILETYPE_PEM) <= 0 ||
        SSL_CTX_use_PrivateKey_file(ctx, "client-key.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        return 1;
    }

    // Load and verify server cert against CA.
    SSL_CTX_load_verify_locations(ctx, "ca.pem", NULL);
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
    SSL_CTX_set_verify_depth(ctx, 1);

    // Setting the minimum TLS version to 1.2, and ensuring SHA-256 is used for the cipher.
    SSL_CTX_set_min_proto_version(ctx, TLS1_2_VERSION);
    SSL_CTX_set_cipher_list(ctx, "ECDHE-RSA-AES128-SHA256");
    

    // Testing for invalid sockets.
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket creation failed!\n");
        WSACleanup();
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Checking for socket errors.
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Connection failed!\n");
        closesocket(client_fd);
        WSACleanup();
        return 1;
    }

    // Creating an SSL structure.
    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, (int)client_fd);

    // Attempting the TLS handshake, along with error handling.
    if (SSL_connect(ssl) != 1) {
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        closesocket(client_fd);
        WSACleanup();
        return 1;
    }

    // Test case for checking the cipher suite used.
    printf("Cipher used: %s\n", SSL_get_cipher(ssl));

    

    while (1) {
        printf("Enter message (type 'exit' to quit): ");
        fgets(message, BUFFER_SIZE, stdin);
        message[strcspn(message, "\n")] = 0;

        if (strncmp(message, "exit", 4) == 0)
            break;

        if (SSL_write(ssl, message, strlen(message)) <= 0) {
            printf("SSL_write failed!\n");
            break;
        }

        int bytes_received = SSL_read(ssl, buffer, BUFFER_SIZE - 1);

        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            printf("Raw bytes received (%d): ", bytes_received);

            for (int i = 0; i < bytes_received; i++) {
                printf("%02X ", (unsigned char)buffer[i]);
            }
            printf("\n");

            printf("Server echoed: %s\n", buffer);
        } else {
            printf("SSL_read failed or server disconnected.\n");
            break;
        }

        memset(buffer, 0, BUFFER_SIZE);
    }

    // Cleanup.
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    closesocket(client_fd);
    WSACleanup();
    return 0;
}
