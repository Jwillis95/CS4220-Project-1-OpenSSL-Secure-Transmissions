#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/applink.c>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {

    // Initialize OpenSSL.
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();

    const SSL_METHOD *method = TLS_server_method();
    SSL_CTX *ctx = SSL_CTX_new(method);
    if (!ctx) {
        ERR_print_errors_fp(stderr);
        return 1;
    }

    // Load server cert and key.
    if (SSL_CTX_use_certificate_file(ctx, "server-cert.pem", SSL_FILETYPE_PEM) <= 0 ||
        SSL_CTX_use_PrivateKey_file(ctx, "server-key.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        return 1;
    }

    // Load and verify client cert against CA.
    SSL_CTX_load_verify_locations(ctx, "ca.pem", NULL);
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);
    SSL_CTX_set_verify_depth(ctx, 1);

    // Program accepts a minimum of TLS 1.2 and a maximum of TLS 1.3. Also, I selected random ciphers that use SHA256.
    SSL_CTX_set_min_proto_version(ctx, TLS1_2_VERSION); 
    SSL_CTX_set_max_proto_version(ctx, TLS1_3_VERSION); 
    SSL_CTX_set_cipher_list(ctx, "ECDHE-RSA-AES128-SHA256");
    SSL_CTX_set_ciphersuites(ctx, "TLS_AES_128_GCM_SHA256 ");
    

    WSADATA wsa;
    SOCKET server_fd, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int addrlen = sizeof(client_addr);
    char buffer[BUFFER_SIZE] = {0};

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup failed!\n");
        return 1;
    }

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket creation failed!\n");
        WSACleanup();
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed!\n");
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    if (listen(server_fd, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen failed!\n");
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    client_socket = accept(server_fd, (struct sockaddr*)&client_addr, &addrlen);
    if (client_socket == INVALID_SOCKET) {
        printf("Accept failed!\n");
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, (int)client_socket);

    if (SSL_accept(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        closesocket(client_socket);
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    printf("Secure connection established with client.\n");
    printf("Cipher used: %s\n", SSL_get_cipher(ssl));


    int bytes_read;
    while ((bytes_read = SSL_read(ssl, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[bytes_read] = '\0';
        printf("Raw bytes received (%d): ", bytes_read);
        for (int i = 0; i < bytes_read; i++) {
            printf("%02X ", (unsigned char)buffer[i]);
}
        printf("\n");

        printf("Received: %s\n", buffer);
        SSL_write(ssl, buffer, bytes_read);
        memset(buffer, 0, BUFFER_SIZE);
    }

    SSL_shutdown(ssl);
    SSL_free(ssl);
    closesocket(client_socket);
    closesocket(server_fd);
    SSL_CTX_free(ctx);
    WSACleanup();
    return 0;
}
