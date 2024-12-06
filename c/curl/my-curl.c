#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

// Callback function to write the received data to stdout
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t written = fwrite(ptr, size, nmemb, stdout);
    return written;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <URL>\n", argv[0]);
        return 1;
    }

    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        // Set the URL based on command-line argument
        curl_easy_setopt(curl, CURLOPT_URL, argv[1]);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        // Perform the request, res will get the return code
        res = curl_easy_perform(curl);
        // Check for errors
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        // always cleanup
        curl_easy_cleanup(curl);
    }
    return 0;
}