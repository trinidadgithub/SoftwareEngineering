#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define PORT 8080

void handleError(const char *msg) {
    perror(msg);
    ERR_print_errors_fp(stderr);
    exit(1);
}

int main() {
    SSL_CTX *ctx;
    SSL *ssl;
    int server_fd, client_fd;
    struct sockaddr_in addr;
    char buf[1024] = {0};
    int len;

    // Initialize SSL
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(TLS_server_method());

    // Load server certificate and private key
    if (SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM) <= 0) {
        handleError("Failed to load server certificate");
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM) <= 0) {
        handleError("Failed to load server private key");
    }
    
    // Check if the private key corresponds to the certificate
    if (!SSL_CTX_check_private_key(ctx)) {
        handleError("Private key does not match the certificate public key");
    }

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) handleError("socket");

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) handleError("bind");
    if (listen(server_fd, 3) < 0) handleError("listen");

    printf("Server listening on port %d\n", PORT);

    if ((client_fd = accept(server_fd, NULL, NULL)) < 0) handleError("accept");

    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, client_fd);

    if (SSL_accept(ssl) <= 0) {
        handleError("SSL accept");
    }

    printf("SSL connection using %s\n", SSL_get_cipher(ssl));

    // Here you can add code to verify the client's certificate if needed
    // X509 *client_cert = SSL_get_peer_certificate(ssl);
    // if (client_cert != NULL) {
    //     // Verify certificate here
    //     X509_free(client_cert);
    // }

    len = SSL_read(ssl, buf, sizeof(buf) - 1);
    if (len > 0) {
        buf[len] = '\0';
        printf("Received: %s\n", buf);
    }

    SSL_free(ssl);
    close(client_fd);
    close(server_fd);
    SSL_CTX_free(ctx);
    return 0;
}
