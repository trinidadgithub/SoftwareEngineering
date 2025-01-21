#!/bin/bash

# Directory paths
CA_DIR="ca"
CERTS_DIR="certs"

# Function to log messages
log() {
    local message="$1"
    echo "[$(date +'%Y-%m-%d %H:%M:%S')] $message" >> "${CA_DIR}/generate_certs.log"
}

# Function to check if command was successful
check_success() {
    local command="$1"
    if [ $? -ne 0 ]; then
        log "Error executing: $command"
        echo "An error occurred while executing: $command. Check ${CA_DIR}/generate_certs.log for details."
        exit 1
    else
        log "Successfully executed: $command"
    fi
}

# Function to check if a file exists and prompt to overwrite
check_file() {
    local file="$1"
    local dir="$2"
    if [ -f "$dir/$file" ]; then
        read -p "The file $dir/$file already exists. Do you want to overwrite it? (y/n): " answer
        case ${answer:0:1} in
            y|Y ) return 0;;
            * ) return 1;;
        esac
    fi
    return 0
}

# Function to generate certificate
generate_cert() {
    local type="$1"
    local subj="$2"
    local key="$3"
    local csr="$4"
    local crt="$5"
    local cert_dir="$6"  # Directory for the certificate (ca or certs)
    local ca_dir="$CA_DIR"  # CA directory for keys and CSRs

    # Check if each file exists before proceeding
    if ! check_file "$key" "$ca_dir" || ! check_file "$csr" "$ca_dir" || ! check_file "$crt" "$cert_dir"; then
        log "Skipping $type certificate generation due to user choice or existing files."
        return
    fi

    log "Generating $type certificate"
    openssl genrsa -out "$ca_dir/$key" 4096
    check_success "openssl genrsa -out $ca_dir/$key 4096"

    openssl req -new -key "$ca_dir/$key" -out "$ca_dir/$csr" -subj "$subj"
    check_success "openssl req -new -key $ca_dir/$key -out $ca_dir/$csr -subj \"$subj\""

    if [ "$type" != "CA" ]; then
        openssl x509 -req -in "$ca_dir/$csr" -CA "$ca_dir/ca.crt" -CAkey "$ca_dir/ca.key" -CAcreateserial -out "$cert_dir/$crt" -days 365
        check_success "openssl x509 -req -in $ca_dir/$csr -CA $ca_dir/ca.crt -CAkey $ca_dir/ca.key -CAcreateserial -out $cert_dir/$crt -days 365"
    else
        openssl req -x509 -new -key "$ca_dir/$key" -out "$ca_dir/$crt" -days 365 -nodes -subj "$subj"
        check_success "openssl req -x509 -new -key $ca_dir/$key -out $ca_dir/$crt -days 365 -nodes -subj \"$subj\""
    fi
}

# Main execution
log "Starting certificate generation process"

# Create directories if they don't exist
mkdir -p "$CA_DIR" "$CERTS_DIR"
check_success "mkdir -p $CA_DIR $CERTS_DIR"

# Check if OpenSSL is installed
if ! command -v openssl &> /dev/null; then
    log "OpenSSL could not be found. Please install OpenSSL."
    echo "OpenSSL is required but not found. Please install it."
    exit 1
fi

# Generate CA Certificate
generate_cert "CA" "/C=US/ST=State/L=Locality/O=Organization/CN=Test CA" "ca.key" "ca.csr" "ca.crt" "$CA_DIR"

# Generate Server Certificate
generate_cert "Server" "/C=US/ST=State/L=Locality/O=Organization/CN=localhost" "server.key" "server.csr" "server.crt" "$CERTS_DIR"

# Generate Client Certificate
generate_cert "Client" "/C=US/ST=State/L=Locality/O=Organization/CN=Client" "client.key" "client.csr" "client.crt" "$CERTS_DIR"

log "All certificates generated successfully"
echo "Certificate generation completed. Logs available in ${CA_DIR}/generate_certs.log"
