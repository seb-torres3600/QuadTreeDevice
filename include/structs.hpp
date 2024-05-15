#ifndef ZONE_HPP
#define ZONE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>


struct NodeBoundaries {
    float bottom_x;
    float bottom_y;
    float top_x;
    float top_y;
    NodeBoundaries(){}
    NodeBoundaries(std::vector<float> coords){
        bottom_x = coords[0];
        bottom_y = coords[1];
        top_x = coords[2];
        top_y = coords[3];
    }
};

struct Point{
    float x;
    float y;
    Point(std:: vector<float> coords){
        this->x = coords[0];
        this->y = coords[1];
    }
    Point(float x, float y){
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
    int zone_id;
    std:: string shape;
    Rectangle rectangle;
    Circle circle;
    Triangle triangle;

    Zone(nlohmann::json& zone_data){
        if(zone_data.find("shape") != zone_data.end()){
            this->shape = zone_data.at("shape").get<std::string>();
            // Make shape string to upper for consistency regardless of input
            std::transform(this->shape.begin(), this->shape.end(), this->shape.begin(), ::toupper);
        }

        if(zone_data.find("id") != zone_data.end()){
            this->zone_id = zone_data.at("id").get<int>();
        }

        if(zone_data.find("coordinates") != zone_data.end()){
            nlohmann::json coord_json = zone_data.at("coordinates");
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
                std:: cout << "Unknown shape: " << this->shape << std::endl;
            }
        }
    }
};

#endif