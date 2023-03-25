// Để tạo một HTTP request với body là một JSON và in ra kết quả trả về trên màn hình trong C, chúng ta có thể sử dụng libcurl kết hợp với json-c (một thư viện để xử lý JSON trong C).

// Dưới đây là ví dụ về cách tạo một HTTP POST request với body là một JSON và in ra kết quả trả về trên màn hình:

#include <stdio.h>
#include <curl/curl.h>
#include <json-c/json.h>
struct MemoryStruct {
  char *memory;
  size_t size;
};
 
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(!ptr) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}
  

int main(void)
{
    CURL *curl;
    CURLcode res;

    // JSON data to be sent in the request body

    struct MemoryStruct chunk;
 
    chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
    chunk.size = 0;
    // Initialize libcurl
    curl = curl_easy_init();
    curl_global_init(CURL_GLOBAL_ALL);
    if(curl) {
        // Set the URL for the request
        curl_easy_setopt(curl, CURLOPT_URL, "http://nghuyhoang2509.click/login");

        // Set the HTTP headers
        struct curl_slist *headers = NULL;
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set the request method to POST
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        // Set the request body
        curl_mime *mime;
        curl_mimepart *part;
        mime = curl_mime_init(curl);
        part = curl_mime_addpart(mime);
        curl_mime_name(part, "data");
        curl_mime_data(part, "{\"mail\":\"huytest@gmail.com\", \"password\":\"okokokok1\"}\n", CURL_ZERO_TERMINATED);
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);



        // Set up the write callback function to handle the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        // Perform the request
        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        else{
            printf("test: %s",chunk.memory);
        }

        // Clean up
        curl_mime_free(mime);
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    curl_global_cleanup();
    return 0;
}