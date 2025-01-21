#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define PORT 8080
#define SERVER "localhost"
#define CERT_DIR "./certs/"
#define CA_DIR "./ca/"

void handleError(const char *msg) {
    perror(msg);
    ERR_print_errors_fp(stderr);
    exit(1);
}

int main() {
    int sockfd, len;
    struct sockaddr_in dest;
    struct hostent *host;
    SSL_CTX *ctx;
    SSL *ssl;
    const char *message = "Hello, SSL!";

    // Initialize SSL
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(TLS_client_method());

    // Load CA certificate for verification
    char ca_cert_path[256];
    snprintf(ca_cert_path, sizeof(ca_cert_path), "%sca.crt", CA_DIR);
    if (SSL_CTX_load_verify_locations(ctx, ca_cert_path, NULL) <= 0) {
        handleError("Failed to load CA certificate");
    }

    // Load client certificate and private key
    char client_cert_path[256], client_key_path[256];
    snprintf(client_cert_path, sizeof(client_cert_path), "%sclient.crt", CERT_DIR);
    snprintf(client_key_path, sizeof(client_key_path), "%sclient.key", CA_DIR);

    if (SSL_CTX_use_certificate_file(ctx, client_cert_path, SSL_FILETYPE_PEM) <= 0) {
        handleError("Failed to load client certificate");
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, client_key_path, SSL_FILETYPE_PEM) <= 0) {
        handleError("Failed to load client private key");
    }

    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL); // Verify server certificate
    
    host = gethostbyname(SERVER);
    if (host == NULL) handleError("gethostbyname");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) handleError("socket");

    memset(&dest, 0, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(PORT);
    memcpy(&dest.sin_addr, host->h_addr, host->h_length);

    if (connect(sockfd, (struct sockaddr*)&dest, sizeof(dest)) != 0) handleError("connect");

    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sockfd);

    if (SSL_connect(ssl) <= 0) {
        handleError("SSL connect");
    }

    printf("SSL connection using %s\n", SSL_get_cipher(ssl));

    if (SSL_write(ssl, message, strlen(message)) <= 0) {
        handleError("SSL write");
    }

    SSL_free(ssl);
    close(sockfd);
    SSL_CTX_free(ctx);
    return 0;
}
