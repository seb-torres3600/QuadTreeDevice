#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <format>

#include <unistd.h>
#include <getopt.h>

#include "../include/data.hpp"
#include "../include/quadTree.hpp"
#include "../include/structs.hpp"
#include "../include/device.hpp"
#include "../include/logger.hpp"
#include "../include/requests.hpp"

#include <nlohmann/json.hpp>

int main(int argc, char* argv[]){
    Device* device = Device :: getInstance();
    Data* data = Data :: getInstance();
    Quadtree* data_tree;
    Request* requester;
    nlohmann::json zonesData;
    std:: string url;

    std:: string config_directory = "config";
    std:: string config_file = "device.json";
    std:: string log_file = "logs.log";
    std:: string log_directory = "logs";
    std:: string input_directory = "data";
    std:: string input_file = "data.json";
    int log_level = 2;

    bool api = true;

    bool naive;
    int opt;
    
    // Creating the long options
    static struct option long_options[] = {
            {"author",   no_argument, 0,  'a' },
            {"help", no_argument, 0, 'h'},
            {"input_dir", required_argument, 0, 'o'},
            {"input_file", required_argument, 0, 'i'},
            {"config_file", required_argument, 0, 'c'},
            {"config_dir", required_argument, 0, 'f'},
            {"naive", required_argument, 0, 'n'},
            {"log_file", required_argument, 0, 'e'},
            {"log_dir", required_argument, 0, 'd'},
            {"debug_level", required_argument, 0, 'l'},
            {0, 0, 0, 0}
    };

    while(opt != -1){
        int index = 0;
        opt = getopt_long(argc, argv, "aie:c:l:",long_options, &index);
        switch (opt){
            case 'a':
                std:: cout << "Author: Sebastian Torres" << std:: endl;
                return 0;
            case 'o':
                input_directory = optarg;
                api = false;
                break;
            case 'i':
                input_file = optarg;
                api = false;
                break;
            case 'f':
                config_directory = optarg;
                break;
            case 'c':
                config_file = optarg;
                break;
            case 'e':
                log_file = optarg;
                break;
            case 'l':
                log_level = std::stoi(optarg);
                break;
            case 'd':
                log_directory = optarg;
                break;
            default:
                break;
        }
    }
    // Init logger
    Logger::init(log_directory, log_file, log_level);
    std::shared_ptr<spdlog::logger> logger = Logger:: getLogger();
    logger->info("********** Starting Logs for Quad Tree Device **********");

    if(!config_file.empty()){
        std:: string config = config_directory + "/" + config_file;
        std::ifstream conf_file(config);
        if(!conf_file.good()){
            logger->error(config + "file doesn't exist");
            logger->error("Please provide a valid config file");
            return -1;
        }
        logger->info("Setting  configuration");
        device->setConfiguration(config);

        if (conf_file.is_open()){
            conf_file.close();
        }
    }

    if(!input_file.empty() && !api){
        std::string inputs = input_directory + "/" + input_file;
        std::ifstream file(inputs);
        if (!file.good()){
            logger->error(inputs + " file doesn't exist");
            return -1;
        }
        data->setData(inputs);
        
        if(file.is_open()){
            file.close();
        }
    }

    Point inital_location = *device->getLocation();
    std::vector<float> inital_size = device->getSizeOfArea();
    data_tree = new Quadtree(inital_size);

    if (api){
        requester = Request:: getInstance();
        url = device->getCurlUrl();
        std:: string area = convertAreaToString(inital_size);
        logger->info("URL: " + url + area);
        zonesData = nlohmann::json::parse(requester->getZones(url+area));
    }

    if (!api){
        for(auto& object: data->getData()){
            Zone zone =  Zone(object);
            data_tree->insert(zone);
        }
    } else {
        for(auto& object: zonesData){
            Zone zone = Zone(object);
            data_tree->insert(zone);
        }
    }

    logger->flush();
    // int a = 0;
    while(true){ 
        NodeBoundaries current_boundaries = data_tree->getBoundaries();
        Point current_location = *device->getLocation();
        float threshold = device->getThreshold();
        /*
        if(a == 5){
            return 0;
        }
        */
        if(needToRefresh(current_boundaries, current_location, threshold)){
            logger->critical("**** Need to refresh zones ***");
            std::vector<float> current_size = device->getSizeOfArea();
            delete data_tree;
            data_tree = new Quadtree(current_size);
            std:: string area = convertAreaToString(current_size);
            zonesData = nlohmann::json::parse(requester->getZones(url+area));
            for(auto& object: zonesData){
                Zone zone = Zone(object);
                data_tree->insert(zone);
            }
        }
        data_tree->search(current_location);
        std::this_thread::sleep_for(std::chrono::seconds(device->getInterval()));
        // a++;
    }

}
