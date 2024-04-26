#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>

#include "zone.hpp"



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

class Quadtree{
    public:
        Quadtree(std::vector<float> nd_coord);
        ~Quadtree();
        void insert(Zone z);
        bool search(std:: vector<float> point);
        void printTree();
    private:
        NodeBoundaries nd_bndr;
        std:: vector<Zone> zns;

        Quadtree* topLeft;
        Quadtree* topRight;
        Quadtree* bottomLeft;
        Quadtree* bottomRight;

        std::vector<std::vector<float>> split(NodeBoundaries boundary);
        int findQuadrant(std::vector<std::vector<float>> boundaries, Zone z);
        bool fits(NodeBoundaries boundary, Zone z);
        bool pointFits(NodeBoundaries boundary, std::vector<float> point);
        int findPointQuadrant(std::vector<std::vector<float>> boundaries, std::vector<float> point);
};

#endif