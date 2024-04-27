#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <variant> 

#include "structs.hpp"
#include "helpers.hpp"

class Quadtree{
    public:
        Quadtree(std::vector<float> nd_coord);
        ~Quadtree();
        void insert(Zone z);
        bool search(Point location);
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
        int findPointQuadrant(std::vector<std::vector<float>> boundaries, Point location);
        bool pointFits(Zone z, Point location);
};

#endif