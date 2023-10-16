#include "include.h"

struct MemoryStruct {
    char *memory;
    size_t size;
};

// Callback function to write received data into a buffer.
size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL) {
        printf("Not enough memory (realloc returned NULL)\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0; // Null-terminate the string

    return realsize;
}

cJSON *GetRequest(const char *url, const char *credentials) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        struct MemoryStruct chunk;
        chunk.memory = (char *)malloc(1); // Start with an empty string
        chunk.size = 0;

        curl_easy_setopt(curl, CURLOPT_USERPWD, credentials);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // Disable peer verification
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // Disable host verification

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "Request failed: %s\n", curl_easy_strerror(res));
            free(chunk.memory);
            curl_easy_cleanup(curl);
            return NULL; // Return NULL on error
        }

        cJSON *json = cJSON_Parse(chunk.memory);
        free(chunk.memory); // Free the memory allocated for chunk.memory

        if (json == NULL) {
            fprintf(stderr, "Failed to parse JSON\n");
            curl_easy_cleanup(curl);
            return NULL; // Return NULL on JSON parsing error
        }

        curl_easy_cleanup(curl);
        return json; // Return the parsed JSON object
    }

    return NULL; // Return NULL if curl initialization failed
}