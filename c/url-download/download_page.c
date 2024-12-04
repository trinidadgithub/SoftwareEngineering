#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define MAX_URL_LENGTH 1000
#define MAX_FILENAME_LENGTH 1000 // Increased size to accommodate full path

// Callback function to write the received data
size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

// Function to create a valid filename from URL including path
void get_filename_from_url(const char *url, char *filename, size_t filename_size) {
    const char *protocol = strstr(url, "://");
    const char *domain_start = protocol ? protocol + 3 : url;
    const char *path_start = strchr(domain_start, '/');

    if (path_start == NULL) {
        // If no path, just use domain name
        strncpy(filename, domain_start, filename_size - 1);
    } else {
        // Copy domain and path part, replacing slashes with hyphens in path
        size_t domain_length = path_start - domain_start;
        if (domain_length > filename_size - 1) domain_length = filename_size - 1;
        strncpy(filename, domain_start, domain_length);
        filename[domain_length] = '\0';

        const char *path_part = path_start + 1; // Skip first '/'
        size_t path_length = 0;
        int last_char_was_slash = 0; // Flag to check if the last character added was a hyphen
        
        while (*path_part && path_length < filename_size - domain_length - 2) { // -2 for .html and null terminator
            if (*path_part == '/') {
                if (!last_char_was_slash && path_length + 1 < filename_size - domain_length - 2) { // Only add hyphen if it's not already added
                    strncat(filename, "-", 2);
                    path_length++;
                    last_char_was_slash = 1;
                }
            } else {
                char char_to_add[2] = {*path_part, '\0'};
                strncat(filename, char_to_add, 2);
                path_length++;
                last_char_was_slash = 0;
            }
            path_part++;
        }
        // Remove trailing hyphen if it exists
        size_t len = strlen(filename);
        if (len > 0 && filename[len - 1] == '-') {
            filename[len - 1] = '\0';
        }
    }

    // Ensure filename ends with .html if it doesn't already
    if (strstr(filename, ".html") == NULL && strstr(filename, ".htm") == NULL) {
        strncat(filename, ".html", filename_size - strlen(filename) - 1);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <URL>\n", argv[0]);
        return 1;
    }

    CURL *curl;
    FILE *fp;
    CURLcode res;
    char url[MAX_URL_LENGTH];
    char filename[MAX_FILENAME_LENGTH];

    // Copy the URL from command line argument
    strncpy(url, argv[1], MAX_URL_LENGTH - 1);
    url[MAX_URL_LENGTH - 1] = '\0'; // Ensure null-termination

    get_filename_from_url(url, filename, MAX_FILENAME_LENGTH);

    curl = curl_easy_init();
    if(curl) {
        fp = fopen(filename, "wb");
        if(fp == NULL) {
            fprintf(stderr, "Failed to open file %s\n", filename);
            return 1;
        }

        // Set URL
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Pass our file pointer to the callback function
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        // Set the write function to our callback
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

        // Perform the request, res will get the return code
        res = curl_easy_perform(curl);

        // Always cleanup
        curl_easy_cleanup(curl);

        fclose(fp);

        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        else
            printf("Webpage saved as %s\n", filename);
    }

    return 0;
}