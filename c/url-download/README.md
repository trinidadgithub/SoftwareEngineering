# Simple CURL Downloader

This is a basic implementation of a command-line tool that downloads web content using `libcurl` and saves it to a file. The filename is derived from the URL provided, ensuring a unique name for each download.

## Features

- **Download Web Pages:** Fetches content from a given URL.
- **File Naming:** Automatically generates a filename based on the URL, replacing slashes with hyphens and appending `.html` if needed.
- **User-Agent:** Sets a custom User-Agent string to mimic a common browser.
- **Follow Redirects:** Automatically follows URL redirections.

## Prerequisites

- **libcurl:** You need to have `libcurl` installed on your system to compile and run this program. For most Linux distributions, you can install it using your package manager:
  - Ubuntu/Debian: `sudo apt-get install libcurl4-openssl-dev`
  - Fedora: `sudo dnf install libcurl-devel`

## Usage

To use this program, compile it and then run it with a URL as an argument:

```sh
gcc -o curl_downloader curl_downloader.c -lcurl
./curl_downloader <URL>
```

Example
```sh
./curl_downloader https://example.com/some/path
```

This will download the content from https://example.com/some/path and save it as example.com-some-path.html.

Code Structure

    write_data: Callback function for writing downloaded data to a file.
    get_filename_from_url: Function to generate a filename from the URL, ensuring no conflicts by replacing slashes with hyphens and ensuring the file has an .html extension.
    main: Handles command-line arguments, initializes libcurl, performs the download, and writes the data to a file.


Limitations

    URL Length: There's a predefined maximum length for URLs and filenames to prevent buffer overflows.
    File Type: It assumes or forces an .html file extension, which might not be suitable for all content types.
    Error Handling: Basic error reporting is implemented, but more extensive error checks could be added for robustness.


Future Enhancements

    Implement more curl options like verbose output, authentication, or custom headers.
    Enhance the filename generation to handle different file types or MIME types detected in headers.
    Add command-line options for users to specify output filenames, directories, or different behaviors.


License
This code is provided under the MIT License (LICENSE).

Feel free to contribute by suggesting improvements or adding features through pull requests or issues!

### Sequence Diagram
The libcurl implementation is very simple but highly customized to my needs.

```mermaid
sequenceDiagram
    participant C as C Program
    participant Libcurl as libcurl
    participant SSL as SSL/TLS Layer
    participant DNS as DNS Server
    participant HTTP as HTTP Server (Website)
    
    Note over C,Libcurl: User runs program with URL
    C->>Libcurl: curl_easy_init()
    Libcurl->>Libcurl: Initialize curl session
    
    Note over C,Libcurl: Set options including URL, output file, follow location, etc.
    C->>Libcurl: curl_easy_setopt(CURLOPT_URL, url)
    C->>Libcurl: curl_easy_setopt(CURLOPT_WRITEDATA, file pointer)
    C->>Libcurl: curl_easy_setopt(CURLOPT_WRITEFUNCTION, write_data)
    C->>Libcurl: curl_easy_setopt(CURLOPT_FOLLOWLOCATION, 1L)
    
    Note over Libcurl,DNS: DNS resolution
    Libcurl->>DNS: Resolve hostname to IP
    DNS-->>Libcurl: Return IP address
    
    Note over Libcurl,SSL: SSL/TLS Handshake Begins
    Libcurl->>SSL: Initiate SSL/TLS connection
    SSL->>HTTP: TCP handshake with server
    
    Note over SSL,HTTP: SSL/TLS Handshake
    SSL->>HTTP: Client Hello
    HTTP->>SSL: Server Hello, Certificate
    SSL->>HTTP: Certificate Verification
    SSL->>HTTP: Client Key Exchange, Change Cipher Spec, Encrypted Handshake Message
    
    Note over SSL,HTTP: Secure Channel Established
    SSL->>HTTP: Encrypted HTTP GET request
    
    Note over HTTP,SSL: Server Processes Request
    HTTP->>SSL: Encrypted HTTP Response (302 Redirect)
    
    Note over SSL,Libcurl: Handle Redirection
    SSL->>Libcurl: Decrypts response, identifies 302 redirect
    Libcurl->>Libcurl: Follows redirect by setting new URL
    
    Note over Libcurl,SSL: New SSL connection for redirect URL (if different hostname)
    Libcurl->>SSL: Initiate new SSL/TLS connection if necessary
    SSL->>HTTP: New TCP handshake if needed
    
    Note over SSL,HTTP: Repeat SSL Handshake if new connection
    SSL->>HTTP: Client Hello (if new connection)
    HTTP->>SSL: Server Hello, Certificate (if new connection)
    SSL->>HTTP: Certificate Verification (if new connection)
    SSL->>HTTP: Client Key Exchange, Change Cipher Spec, Encrypted Handshake Message (if new connection)
    
    Note over SSL,HTTP: Secure Channel for Redirected Request
    SSL->>HTTP: Encrypted HTTP GET request for redirected URL
    
    Note over HTTP,SSL: Server Processes Final Request
    HTTP->>SSL: Encrypted HTTP Response (HTML content)
    
    Note over SSL,Libcurl: Decrypt and Process Response
    SSL->>Libcurl: Decrypts and passes HTML content
    
    Note over Libcurl,C: Write Data
    Libcurl->>C: Call write_data function with received data
    C->>Local File: Write data to file
    
    Note over C,Libcurl: Cleanup
    C->>Libcurl: curl_easy_cleanup()
    Libcurl->>Libcurl: Cleanup curl session
    
    Note over C: End of Program Execution
```