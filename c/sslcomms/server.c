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
#include <time.h>
#include <signal.h>

#define PORT 8080
#define CERT_DIR "./certs/"
#define CA_DIR "./ca/"
#define BUFFER_SIZE 1024

// Global flag to handle termination signals
volatile sig_atomic_t running = 1;

void handleError(const char *msg) {
    perror(msg);
    ERR_print_errors_fp(stderr);
    exit(1);
}

// Signal handler for graceful shutdown
void sig_handler(int signum) {
    running = 0;
}

int add(int a, int b) {
    return a + b;
}

int main() {
    SSL_CTX *ctx;
    SSL *ssl;
    int server_fd, client_fd;
    struct sockaddr_in addr;
    char buf[BUFFER_SIZE] = {0};
    int len;

    // Register signal handler
    signal(SIGINT, sig_handler);

    // Initialize SSL
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(TLS_server_method());

    // Load server certificate and private key
    char server_cert_path[256], server_key_path[256];
    snprintf(server_cert_path, sizeof(server_cert_path), "%sserver.crt", CERT_DIR);
    snprintf(server_key_path, sizeof(server_key_path), "%sserver.key", CA_DIR);

    if (SSL_CTX_use_certificate_file(ctx, server_cert_path, SSL_FILETYPE_PEM) <= 0) {
        handleError("Failed to load server certificate");
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, server_key_path, SSL_FILETYPE_PEM) <= 0) {
        handleError("Failed to load server private key");
    }
    
    // Check if the private key corresponds to the certificate
    if (!SSL_CTX_check_private_key(ctx)) {
        handleError("Private key does not match the certificate public key");
    }

    // Load CA certificate for client verification
    char ca_cert_path[256];
    snprintf(ca_cert_path, sizeof(ca_cert_path), "%sca.crt", CA_DIR);
    if (SSL_CTX_load_verify_locations(ctx, ca_cert_path, NULL) <= 0) {
        handleError("Failed to load CA certificate");
    }
    
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);  // Verify client certificate
    
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) handleError("socket");

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) handleError("bind");
    if (listen(server_fd, 3) < 0) handleError("listen");

    printf("Server listening on port %d\n", PORT);

    while (running) {
        if ((client_fd = accept(server_fd, NULL, NULL)) < 0) {
            if (errno == EINTR) {  // Handle interruption
                continue;
            }
            handleError("accept");
        }

        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client_fd);

        if (SSL_accept(ssl) <= 0) {
            handleError("SSL accept");
        }

        printf("SSL connection using %s\n", SSL_get_cipher(ssl));

        // Continuous conversation until 'exit' is received or signal interrupts
        while (running) {
            len = SSL_read(ssl, buf, sizeof(buf) - 1);
            if (len > 0) {
                buf[len] = '\0';
                printf("Client: %s\n", buf);

                if (strcmp(buf, "exit\n") == 0 || strcmp(buf, "exit") == 0) {
                    break;
                }

                // Parse the question and respond
                int a, b;
                if (sscanf(buf, "What is %d plus %d?", &a, &b) == 2) {
                    int result = add(a, b);
                    char response[BUFFER_SIZE];
                    snprintf(response, sizeof(response), "The result is: %d\n", result);
                    SSL_write(ssl, response, strlen(response));
                } else {
                    SSL_write(ssl, "I didn't understand that. Please ask about addition.\n", 52);
                }
            } else if (len == 0) {  // Client closed connection
                printf("Client disconnected\n");
                break;
            } else if (len < 0) {
                if (errno == EINTR) {  // Handle interruption
                    continue;
                }
                handleError("SSL read");
            }
        }

        SSL_free(ssl);
        close(client_fd);
    }

    printf("Server shutting down\n");
    close(server_fd);
    SSL_CTX_free(ctx);
    return 0;
}
