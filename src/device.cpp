#include "../include/device.hpp"

Device* Device :: device = NULL;

Device:: Device(){
    // TODO: Buy gps receiver or find library to get accurate location
    // std:: vector<float> default_location = {47.61569, -122.338686}; // Amazon spheres
    std:: vector<float> default_location = {10.0, 8.0}; 
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
    std:: string area_width = "area_width";
    std:: string area_height = "area_height";
    if(config.find(area_width) != config.end() && config.find(area_height) != config.end()){
        float area_width_half = float(config[area_width])/2.0;
        float area_height_half = float(config[area_height])/2.0;
        return {location->x - area_width_half, location->y - area_height_half,
                location->x + area_width_half, location->y + area_height_half};
    }
    logger->error("No size of area value found in config");
}

std:: string Device:: getCurlUrl(){
    std:: string api_host = getApiHost();
    std:: string api_port = getApiPort();
    std:: string api_endpoint = getApiEndpoint();
    std:: string database_table = getDataTable();
    std:: string curl_url = api_host + ":" + api_port + "/" + api_endpoint + "/" + database_table + "/";
    return curl_url;
}

std:: string Device:: getApiHost(){
    std:: string api_host = "api_host";
    if(config.find(api_host) != config.end()){
        return config[api_host];
    }
}

std:: string Device:: getApiPort(){
    std:: string api_port = "api_port";
    if(config.find(api_port) != config.end()){
        return to_string(config[api_port]);
    }
}

std:: string Device:: getApiEndpoint(){
    std:: string api_endpoint = "zones_endpoint";
    if(config.find(api_endpoint) != config.end()){
        return config[api_endpoint];
    }
}

std:: string Device:: getDataTable(){
    std:: string database_table = "database_table";
    if(config.find(database_table) != config.end()){
        return config[database_table];
    }
}

int Device:: getInterval(){
    std:: string interval = "interval";
    if(config.find(interval) != config.end()){
        return config[interval];
    }
}


