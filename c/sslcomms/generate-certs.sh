#!/bin/bash

# Function to log messages
log() {
    echo "[$(date +'%Y-%m-%d %H:%M:%S')] $1" >> generate_certs.log
}

# Function to check if command was successful
check_success() {
    if [ $? -ne 0 ]; then
        log "Error executing: $1"
        echo "An error occurred while executing: $1. Check generate_certs.log for details."
        exit 1
    else
        log "Successfully executed: $1"
    fi
}

# Function to check if a file exists and prompt to overwrite
check_file() {
    local file=$1
    if [ -f "$file" ]; then
        read -p "The file $file already exists. Do you want to overwrite it? (y/n): " answer
        case ${answer:0:1} in
            y|Y ) return 0;;
            * ) return 1;;
        esac
    fi
    return 0
}

# Function to generate certificate
generate_cert() {
    local type=$1
    local subj=$2
    local key=$3
    local csr=$4
    local crt=$5

    # Check if each file exists before proceeding
    if ! check_file "$key" || ! check_file "$csr" || ! check_file "$crt"; then
        log "Skipping $type certificate generation due to user choice or existing files."
        return
    fi

    log "Generating $type certificate"
    openssl genrsa -out $key 4096
    check_success "openssl genrsa -out $key 4096"

    openssl req -new -key $key -out $csr -subj "$subj"
    check_success "openssl req -new -key $key -out $csr -subj \"$subj\""

    if [ "$type" != "CA" ]; then
        openssl x509 -req -in $csr -CA ca.crt -CAkey ca.key -CAcreateserial -out $crt -days 365
        check_success "openssl x509 -req -in $csr -CA ca.crt -CAkey ca.key -CAcreateserial -out $crt -days 365"
    else
        openssl req -x509 -new -key $key -out $crt -days 365 -nodes -subj "$subj"
        check_success "openssl req -x509 -new -key $key -out $crt -days 365 -nodes -subj \"$subj\""
    fi
}

# Main execution
log "Starting certificate generation process"

# Check if OpenSSL is installed
if ! command -v openssl &> /dev/null; then
    log "OpenSSL could not be found. Please install OpenSSL."
    echo "OpenSSL is required but not found. Please install it."
    exit 1
fi

# Generate CA Certificate
generate_cert "CA" "/C=US/ST=State/L=Locality/O=Organization/CN=Test CA" "ca.key" "ca.csr" "ca.crt"

# Generate Server Certificate
generate_cert "Server" "/C=US/ST=State/L=Locality/O=Organization/CN=localhost" "server.key" "server.csr" "server.crt"

# Generate Client Certificate
generate_cert "Client" "/C=US/ST=State/L=Locality/O=Organization/CN=Client" "client.key" "client.csr" "client.crt"

log "All certificates generated successfully"
echo "Certificate generation completed. Logs available in generate_certs.log"
