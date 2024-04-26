#ifndef ZONE_HPP
#define ZONE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>


struct Zone {
    std:: vector<float> coordinates;
    std:: string zone_id;
    std:: string shape;
    
    Zone(nlohmann::json& zone_data){
        if(zone_data.find("coordinates") != zone_data.end()){
            nlohmann::json coord_json = zone_data["coordinates"];
            if(coord_json.is_array()){
                for (const auto& coord : coord_json) {
                    coordinates.push_back(coord.get<float>());
                }
            }
        }

        if(zone_data.find("shape") != zone_data.end()){
            this->shape = zone_data["shape"];
        }

        if(zone_data.find("id") != zone_data.end()){
            this->zone_id = zone_data["id"];
        }
    }
};

#endif