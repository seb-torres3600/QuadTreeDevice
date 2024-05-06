#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <iostream>
#include <string>
#include <fstream>
#include "structs.hpp"
#include <nlohmann/json.hpp>
#include <unistd.h>
#include "logger.hpp"

class Device{
    public:
        static Device* getInstance();
        void setLocation(float x, float y);
        Point* getLocation();
        std::string getDeviceIdentifier();
        void setConfiguration(std::string config_file);
        float getThreshold();
        std:: vector<float> getSizeOfArea();
        std::shared_ptr<spdlog::logger> logger = Logger::getLogger();
        
    private:
        Device();
        ~Device();
        Point* location;
        std::string device_identifier;
        static Device* device;
        nlohmann:: json config;

};


#endif