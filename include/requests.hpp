#ifndef REQ_HPP
#define REQ_HPP

#include <iostream>
#include <string>
#include <curl/curl.h>

#include "../include/logger.hpp"

class Request{
    public:
        static Request* getInstance();
        std:: string getZones(std:: string url);
    private:
        Request();
        ~Request();
        CURL* curl;
        static Request* requester;
        static size_t write_callback(char* ptr, size_t size, size_t nmemb, std::string* data);
        std::shared_ptr<spdlog::logger> logger = Logger::getLogger();
};



#endif