#!/bin/bash

# Function to create files and handle errors
create_file() {
    local file=$1
    if [[ -f $file ]]; then
        echo "File '$file' already exists. Skipping."
    else
        touch "$file"
        if [[ $? -eq 0 ]]; then
            echo "File '$file' created successfully."
        else
            echo "Error creating file '$file'." >&2
            exit 1
        fi
    fi
}

# Function to create directories and handle errors
create_directory() {
    local dir=$1
    if [[ -d $dir ]]; then
        echo "Directory '$dir' already exists. Skipping."
    else
        mkdir -p "$dir"
        if [[ $? -eq 0 ]]; then
            echo "Directory '$dir' created successfully."
        else
            echo "Error creating directory '$dir'." >&2
            exit 1
        fi
    fi
}

# Create root-level files
create_file "README.md"
create_file "LICENSE"

# Create modules/eks-cluster directory
create_directory "docs"
create_directory "terraform/modules/iot_core"
create_directory "terraform/modules/iam"
create_directory "terraform/data"
create_directory "python/certificates"
create_directory "logs"

# Create eks-cluster module files
create_file "docs/documentation.md"
create_file "terraform/main.tf"
create_file "terraform/variables.tf"
create_file "terraform/outputs.tf"
create_file "terraform/provider.tf"
create_file "python/certificates/AmazonRootCA1.pem"
create_file "python/certificates/device-certificate.pem.crt"
create_file "python/certificates/device-private.pem.crt"
create_file "logs/mqtt_client.log"

# Completion message
echo "Creation of fullstack-web-app directory structure complete."

