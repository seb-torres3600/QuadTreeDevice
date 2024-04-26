#include <iostream>
#include <fstream>

#include <unistd.h>
#include <getopt.h>

#include "../include/data.hpp"
#include "../include/zone.hpp"
#include "../include/quadTree.hpp"

#include <nlohmann/json.hpp>

int main(int argc, char* argv[]){
    int opt;
    std:: string input_file;
    Data* data = Data :: getInstance();


    // Creating the long options
    static struct option long_options[] = {
            {"name",   no_argument,         0,  'n' },
            {"input_json", required_argument, 0, 'i'},
            {0, 0, 0, 0}
    };

    while(opt != -1){
        int index = 0;
        opt = getopt_long(argc, argv, "n:",long_options, &index);
        switch (opt){
            case 'n':
                std:: cout << "Sebastian Torres" << std:: endl;
                break;
            case 'i':
                input_file = optarg;
                break;
            default:
                break;
        }
    }

    if(!input_file.empty()){
        std::ifstream file(input_file);
        if (!file.good()){
            std:: cout << "ERROR: "<<input_file <<" file doesn't exist" << std:: endl;
            return -1;
        }
        data->setData(input_file);
    }

    vector<Zone> zones;
    for(auto& object: data->getData()){
        Zone tmp_zone =  Zone(object);
        zones.push_back(tmp_zone);
    }

    Quadtree* data_tree = new Quadtree({0.0,0.0, 20.0, 16.0});

    /*
    for(auto z: zones[1].coordinates){
        std:: cout << z << std::endl;
    }
    std:: cout << "====" << std::endl;
    std:: cout << "====" << std::endl;
    */

    
    for(auto zone: zones){
        data_tree->insert(zone);
    }
    
    data_tree->search({4.5, 9.5});
    data_tree->search({10.0, 7.5});
    data_tree->search({14.0, 11.0});
    data_tree->search({1.0,1.0});
    data_tree->search({19.5, 0.5});
}