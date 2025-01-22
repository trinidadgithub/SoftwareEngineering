## SSL Certificate Generation

Key Features:

- Error Handling: The check_success function checks if each openssl command was successful. If not, it logs the error and exits the script.
- Logging: Each significant action is logged to a file named generate_certs.log for tracking purposes.
- Functions:
  
        log: Logs messages with a timestamp to a log file.
        check_success: Checks the exit status of the last command.
        generate_cert: A reusable function to generate certificates, reducing code duplication.
- Checks: 
        Checks if OpenSSL is installed before attempting to use it.
- Modularity: By using functions, the script becomes more modular and easier to maintain or extend.


Usage:

- Save this script to a file, for example generate_certs.sh.
- Make it executable with chmod +x generate_certs.sh.
- Run it with `./generate_certs.sh.`


This script will provide feedback on what's happening during the certificate generation process, and it will fail gracefully if something goes wrong, informing the user of the specific step where the failure occurred.

### Adding check file feature

Key Enhancements:

- File Existence Check: The check_file function checks if a file exists and prompts the user for permission to overwrite it. 
- Conditional Certificate Generation: The generate_cert function now checks if each certificate file already exists before proceeding with generation. If any file exists and the user opts not to overwrite, the certificate generation for that type is skipped.
- User Interaction: The script now interacts with the user to confirm overwriting existing files, providing an extra layer of caution and control.


The control flow in the script ensures that you can safely use the script without accidentally overwriting important existing certificates, giving you the control to decide whether to regenerate certificates or not.

### Separating directory structure
Key Enhancements:

- Directory Variables: CA_DIR and CERTS_DIR are now variables at the top of the script, making it easy to change the storage locations for certificates.
- Directory Creation: The script now checks to ensure the directories exist, creating them if they do not.
- File Paths: All file paths in the functions are now constructed using these variables, ensuring that the CA's keys, CSRs, and CRT are stored in the ca directory, while the server and client certificates are placed in the certs directory.


This script is now more flexible for different directory structures and includes better error handling and logging.

---

### SSL Communication Prototype  

This repository also contains prototype implementations of a simple SSL/TLS client and server in C, designed to demonstrate basic secure communication. These programs are intended as a starting point for more sophisticated algorithms and protocols in SSL communication.

**Overview**

    Server (server.c): Listens for incoming SSL connections, processes basic addition questions, and responds accordingly.
    Client (client.c): Initiates an SSL connection to the server, sends addition problems, and prints the server's responses.


**Requirements**

    Operating System: Ubuntu (or similar Linux distribution)
    Compiler: gcc (GNU Compiler Collection)
    SSL/TLS Libraries: openssl
    Network Tools:
        tcpdump for capturing network traffic
        Wireshark for analyzing and decrypting SSL/TLS traffic


**To install these on Ubuntu:**

```bash

sudo apt-get update
sudo apt-get install gcc libssl-dev tcpdump wireshark
```

### Setup

**Certificates**
Before running, ensure you have generated the necessary SSL certificates (see previous section:

```bash
    ca/ca.crt: CA certificate
    ca/ca.key: CA private key
    certs/server.crt: Server certificate
    ca/server.key: Server private key (needs to be in the CA directory for this setup)
    certs/client.crt: Client certificate
    ca/client.key: Client private key
```

Use the provided certificate generation script or similar to create these files.

**Building**
Compile both server and client:

```bash

gcc server.c -o server -lssl -lcrypto
gcc client.c -o client -lssl -lcrypto
```

Running

1. Start the Server:

```bash

./server
```
2. Start the Client in another terminal:

```bash

./client
```

**Analyzing Traffic**
To analyze SSL/TLS traffic:

1. Capture Traffic with tcpdump:

```bash

sudo tcpdump -i lo -s 0 -w capture.pcap port 8080
```
2. Decrypt Traffic with Wireshark:

Open Wireshark, go to Edit > Preferences > Protocols > TLS, and set the "(Pre)-Master-Secret log filename" to the path where session keys were logged (`~/ssl_keys.log`).


**Notes**

Encryption: Ensure `SSLKEYLOGFILE` is set for both client and server to log session keys for decryption in Wireshark:

```bash

export SSLKEYLOGFILE=~/ssl_keys.log
```

Security: This is a prototype for educational purposes. In production, use more secure configurations and never log keys in a real environment.


### Future Enhancements

**These prototypes are meant to be expanded:**

- Improved Error Handling: More robust error management.
- Complex Protocols: Implement more sophisticated SSL/TLS features.
- Containerization: These will be packaged into Docker containers for easier deployment and isolation.


**Deployment**

- Containerization: Planned to be containerized with Docker for consistent runtime environments.
- Kubernetes: Ultimately, these services will be deployed to Kubernetes for scalability, high availability, and management.
