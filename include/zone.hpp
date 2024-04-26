#ifndef ZONE_HPP
#define ZONE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <variant> 

#include "helpers.hpp"

struct Point{
    int x;
    int y;
    Point(int x, int y){
        this->x = x;
        this->y = y;
    }
    Point(){}
};

struct Triangle{
    Point leftPoint;
    Point centerPoint;
    Point rightPoint;
    Triangle(std::vector<float> coords){
        this->leftPoint = Point(coords[0], coords[1]);
        this->centerPoint = Point(coords[2], coords[3]);
        this->rightPoint = Point(coords[4], coords[5]);
    }
    Triangle(){}
};

struct Circle{
    Point centerPoint;
    float radius;
    Circle(std::vector<float> coords){
        this->centerPoint = Point(coords[0], coords[1]);
        this->radius = coords[2];
    }
    Circle(){}
};


struct Rectangle{
    Point bottomPoint;
    Point topPoint;
    Rectangle(std::vector<float> coords){
        this->bottomPoint = Point(coords[0], coords[1]);
        this->topPoint = Point(coords[2], coords[3]);
    }
    Rectangle(){}
};


struct Zone {
    std:: vector<float> coordinates;
    std:: string zone_id;
    std:: string shape;
    Rectangle rectangle;
    Circle circle;
    Triangle triangle;
    
    
    Zone(nlohmann::json& zone_data){
        if(zone_data.find("shape") != zone_data.end()){
            this->shape = toUpperCase(zone_data["shape"]);
        }

        if(zone_data.find("id") != zone_data.end()){
            this->zone_id = zone_data["id"];
        }

        if(zone_data.find("coordinates") != zone_data.end()){
            nlohmann::json coord_json = zone_data["coordinates"];
            if(coord_json.is_array()){
                for (const auto& coord : coord_json) {
                    coordinates.push_back(coord.get<float>());
                }
            }
        }
        if (coordinates.size() != 0){
            if (this->shape == "CIRCLE"){
                this->circle = Circle(coordinates);
            } else if (this->shape == "TRIANGLE"){
                this->triangle = Triangle(coordinates);
            } else if (this->shape == "RECTANGLE"){
                this->rectangle = Rectangle(coordinates);
            } else {
                std:: cout << "Shape is not recognized" << std::endl;
            }
        }
    }
};

#endif