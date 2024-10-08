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
create_file "docker-compose.yml"

# Create modules/eks-cluster directory
create_directory "docs"
create_directory "backend"
create_directory "frontend/src"
create_directory "database"
create_directory ".github/workflows"

# Create eks-cluster module files
create_file "docs/documentation.md"
create_file "backend/Dockerfile"
create_file "backend/server.js"
create_file "backend/package.json"
create_file "frontend/Dockerfile"
create_file "frontend/package.json"
create_file "frontend/src/App.js"
create_file "frontend/src/index.js"
create_file "database/Dockerfile"
create_file "database/init.sql"
create_file ".github/workflows/ci-cd.yml"

# Completion message
echo "Creation of fullstack-web-app directory structure complete."

