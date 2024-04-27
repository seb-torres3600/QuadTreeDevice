#include <iostream>
#include <fstream>

#include <unistd.h>
#include <getopt.h>

#include "../include/data.hpp"
#include "../include/quadTree.hpp"
#include "../include/structs.hpp"

#include <nlohmann/json.hpp>

int main(int argc, char* argv[]){
    int opt;
    std:: string input_file;
    Data* data = Data :: getInstance();


    // Creating the long options
    static struct option long_options[] = {
            {"author",   no_argument,         0,  'a' },
            {"input_json", required_argument, 0, 'i'},
            {0, 0, 0, 0}
    };

    while(opt != -1){
        int index = 0;
        opt = getopt_long(argc, argv, "a:",long_options, &index);
        switch (opt){
            case 'a':
                std:: cout << "Author: Sebastian Torres" << std:: endl;
                return 0;
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

    for(auto zone: zones){
        data_tree->insert(zone);
    }
    
    data_tree->search(Point(4.5, 9.5));
}