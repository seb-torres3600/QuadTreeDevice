#include "../include/device.hpp"

Device* Device :: device = NULL;

Device:: Device(){
    // TODO: Buy gps receiver or find library to get accurate location
    // std:: vector<float> default_location = {47.61569, -122.338686}; // Amazon spheres
    std:: vector<float> default_location = {15.5, 6.0}; // Amazon spheres
    location = new Point(default_location);
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        device_identifier = std::string(hostname);
    } else {
        logger->error("Failed to get hostname");
    }
}

Device:: ~Device(){
    delete location;
    delete device;
}

Device* Device:: getInstance(){
    if(device == NULL){
        device = new Device();
    }
    return device;
}

void Device:: setLocation(float x, float y){
    location->x = x;
    location->y = y;
}

Point* Device:: getLocation(){
    return location;
}

std::string Device:: getDeviceIdentifier(){
    return device_identifier;
}

void Device:: setConfiguration(std:: string config_file){
    std::ifstream f(config_file);
    config = nlohmann::json::parse(f);
    if (f.is_open()){
        f.close();
    }
}

float Device:: getThreshold(){
    std:: string threshold_label = "threshold";
    if(config.find(threshold_label) != config.end()){
        return config["threshold"];
    }
    logger->error("No threshold value found in config");
}

std:: vector<float> Device:: getSizeOfArea(){
    std:: string area_label = "size_of_area";
    if(config.find(area_label) != config.end()){
        float surrounding_area = float(config[area_label])/2.0;
        return {location->x - surrounding_area, location->y - surrounding_area,
                location->x + surrounding_area, location->y + surrounding_area};
    }
    logger->error("No size of area value found in config");
}



