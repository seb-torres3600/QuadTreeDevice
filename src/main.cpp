#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

#include <unistd.h>
#include <getopt.h>

#include "../include/data.hpp"
#include "../include/quadTree.hpp"
#include "../include/structs.hpp"
#include "../include/device.hpp"
#include "../include/logger.hpp"

#include <nlohmann/json.hpp>

int main(int argc, char* argv[]){
    Device* device = Device :: getInstance();
    Data* data = Data :: getInstance();
    Quadtree* data_tree;

    std:: string config_directory = "config";
    std:: string config_file = "device.json";
    std:: string log_file = "logs.log";
    std:: string log_directory = "logs";
    std:: string input_directory = "data";
    std:: string input_file = "data.json";

    std:: vector<Zone> zones;
    bool naive;
    int opt;
    
    // Creating the long options
    static struct option long_options[] = {
            {"author",   no_argument, 0,  'a' },
            {"help", no_argument, 0, 'h'},
            {"input_dir", required_argument, 0, 'o'},
            {"input_file", required_argument, 0, 'i'},
            {"config_file", required_argument, 0, 'c'},
            {"config_dir", required_argument, 0, 'p'},
            {"naive", required_argument, 0, 'n'},
            {"log_file", required_argument, 0, 'l'},
            {"log_dir", required_argument, 0, 'd'},
            {0, 0, 0, 0}
    };

    while(opt != -1){
        int index = 0;
        opt = getopt_long(argc, argv, "a:i:c:l:",long_options, &index);
        switch (opt){
            case 'a':
                std:: cout << "Author: Sebastian Torres" << std:: endl;
                return 0;
            case 'o':
                input_directory = optarg;
                break;
            case 'i':
                input_file = optarg;
                break;
            case 'p':
                config_directory = optarg;
                break;
            case 'c':
                config_file = optarg;
                break;
            case 'l':
                log_file = optarg;
                break;
            case 'd':
                log_directory = optarg;
                break;
            default:
                break;
        }
    }
    // Init logger
    Logger::init(log_directory, log_file);
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
        device->setConfiguration(config);

        if (conf_file.is_open()){
            conf_file.close();
        }
    }

    if(!input_file.empty()){
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


    for(auto& object: data->getData()){
        Zone tmp_zone =  Zone(object);
        zones.push_back(tmp_zone);
    }
    
    Point inital_location = *device->getLocation();
    data_tree = new Quadtree(device->getSizeOfArea());

    for(auto zone: zones){
        data_tree->insert(zone);
    }

    int a = 0;
    while(1){ 
        NodeBoundaries current_boundaries = data_tree->getBoundaries();
        Point current_location = *device->getLocation();
        float threshold = device->getThreshold();
        if(a == 10){
            return 0;
        }
        if(needToRefresh(current_boundaries, current_location, threshold)){
            // Grab relative point from database
            // Create new tree
            // make point into zones
            // insert zones
        }
        data_tree->search(current_location);
        std::this_thread::sleep_for(std::chrono::seconds(5));
        a++;
    }

    if(naive){
        for(auto zone: zones){
            if(pointFitsInRectangle(zone, Point(3.4, 3.5))){
                logger->error("Zone id: " + std::to_string(zone.zone_id));
                break;
            }
        }
    }

}
