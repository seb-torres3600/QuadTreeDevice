#include "../include/requests.hpp"

Request* Request :: requester = NULL;

Request:: Request(){
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(!curl){
        logger->error("Failed to initialize libcurl");
    }
}

Request:: ~Request(){
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    delete requester;
}


Request* Request:: getInstance(){
    if(requester == NULL){
        requester = new Request();
    }
    return requester;
}

std::string Request:: getZones(std:: string url){
    CURLcode res;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    std:: string response_data;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std:: string curl_stderror(curl_easy_strerror(res));
        logger->error("Failed to perform requests: " + curl_stderror);
        return NULL;
    } else {
        // Print the response explicitly
        logger->info("Data received: " + response_data);
        return response_data;
    }
}

size_t Request:: write_callback(char* ptr, size_t size, size_t nmemb, std::string* data) {
    data->append(ptr, size * nmemb);
    return size * nmemb;
}