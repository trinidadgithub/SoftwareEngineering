#!/bin/bash

# Directory paths
CA_DIR="ca"
CERTS_DIR="certs"
LOGS_DIR="logs"

# Feature flag for overwriting files
OVERWRITE_ALL=false

# Function to display script usage, options, and requirements
display_help() {
    echo "Usage: $0 [OPTIONS]"
    echo "Script to generate SSL certificates with customizable options."
    echo
    echo "Options:"
    echo "  -o    Overwrite all existing files without prompting."
    echo "  -h    Display this help message."
    echo
    echo "Requirements:"
    echo "  - OpenSSL: Must be installed on your system. This script uses:"
    echo "    - openssl genrsa for key generation"
    echo "    - openssl req for certificate signing requests"
    echo "    - openssl x509 for signing certificates"
    echo "  - Bash: Needs to be run in a Bash-compatible environment."
    echo
    echo "Description:"
    echo "  This script generates SSL certificates including a CA, server, and client certificates."
    echo "  By default, it prompts for permission to overwrite existing files unless -o is specified."
    echo "  Certificates are placed in 'ca' for CA files and 'certs' for server and client certificates."
    echo "  Logs are written to the 'logs' directory."
}

# Function to parse command line options
parse_options() {
    while getopts ":oh" opt; do
        case $opt in
            o)
                OVERWRITE_ALL=true
                ;;
            h)
                display_help
                exit 0
                ;;
            \?)
                echo "Invalid option: -$OPTARG" >&2
                echo "Use -h for help." >&2
                exit 1
                ;;
        esac
    done
}

# Function to log messages
log() {
    local message="$1"
    mkdir -p "$LOGS_DIR"
    echo "[$(date +'%Y-%m-%d %H:%M:%S')] $message" >> "${LOGS_DIR}/generate_certs.log"
}

# Function to check if command was successful
check_success() {
    local command="$1"
    if [ $? -ne 0 ]; then
        log "Error executing: $command"
        echo "An error occurred while executing: $command. Check ${LOGS_DIR}/generate_certs.log for details."
        exit 1
    else
        log "Successfully executed: $command"
    fi
}

# Function to check if a file exists and prompt to overwrite (unless forced overwrite)
check_file() {
    local file="$1"
    local dir="$2"
    if [ -f "$dir/$file" ] && ! $OVERWRITE_ALL; then
        read -p "The file $dir/$file already exists. Do you want to overwrite it? (y/n): " answer
        case ${answer:0:1} in
            y|Y ) return 0;;
            * ) return 1;;
        esac
    elif [ -f "$dir/$file" ] && $OVERWRITE_ALL; then
        log "Overwriting existing file: $dir/$file"
        return 0
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
parse_options "$@"

log "Starting certificate generation process"

# Create directories if they don't exist
mkdir -p "$CA_DIR" "$CERTS_DIR" "$LOGS_DIR"
check_success "mkdir -p $CA_DIR $CERTS_DIR $LOGS_DIR"

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
echo "Certificate generation completed. Logs available in ${LOGS_DIR}/generate_certs.log"


