#include "../include/quadTree.hpp"

Quadtree:: Quadtree(std::vector<float> nd_coord){
    nd_bndr = NodeBoundaries(nd_coord);
    topLeft = NULL;
    topRight = NULL;
    bottomLeft = NULL;
    bottomRight = NULL;
}

Quadtree::~Quadtree() {
    delete topRight;
    delete topLeft;
    delete bottomLeft;
    delete bottomRight;
}

void Quadtree:: insert(Zone z){
    std::vector<std::vector<float>> boundaries = split(nd_bndr);
    int quadrant = findQuadrant(boundaries, z);
    if(quadrant == -1){
        zns.push_back(z);
        return;
    }
    
    switch(quadrant){
        case 0:
            if(topRight == NULL){
                topRight = new Quadtree(boundaries[quadrant]);
            }
            topRight->insert(z);
            return;
        case 1:
            if(topLeft == NULL){
                topLeft = new Quadtree(boundaries[quadrant]);
            }
            topLeft->insert(z);
            return;
        case 2:
            if(bottomLeft == NULL){
                bottomLeft = new Quadtree(boundaries[quadrant]);
            }
            bottomLeft->insert(z);
            return;
        case 3:
            if(bottomRight == NULL){
                bottomRight = new Quadtree(boundaries[quadrant]);
            }
            bottomRight->insert(z);
            return;
        default:
            return;
    }
    return;
}

bool Quadtree:: search(Point location){
    std::vector<std::vector<float>> boundaries = split(nd_bndr);
    int quadrant = findPointQuadrant(boundaries, location);
    for(auto zn: zns){
        if (pointFits(zn, location)){
            std::cout << "Found" << std::endl;
            std:: cout << "Point("<< location.x << "," << location.y <<") - found in zone: " << zn.zone_id << std::endl;
            return true;
        }
    }

    std:: cout << quadrant+1 << "->";

    switch(quadrant){
        case 0:
            if(topRight == NULL){
                return false;
            }
            topRight->search(location);
            break;
        case 1:
            if(topLeft == NULL){
                return false;
            }
            topLeft->search(location);
            break;
        case 2:
            if(bottomLeft == NULL){
                return false;
            }
            bottomLeft->search(location);
            break;
        case 3:
            if(bottomRight == NULL){
                return false;
            }
            bottomRight->search(location);
            break;
        default:
            break;
    }
    return false;
}

void Quadtree:: printTree(){
    if(zns.size() == 0){
        std::cout << 0 << std:: endl;
    }
    for(int i = 0; i < zns.size(); i++){
        std:: cout << zns[i].zone_id << std::endl;
    }

    if(topRight != NULL){
        topRight->printTree();
    }
    if(topLeft != NULL){
        topLeft->printTree();
    }
    if(bottomLeft != NULL){
        bottomLeft->printTree();
    }
    if(bottomRight != NULL){
        bottomRight->printTree();
    }   
}

int Quadtree:: findQuadrant(std::vector<std::vector<float>> boundaries, Zone z){

    for(int i = 0; i < 4; i++){
        NodeBoundaries boundary(boundaries[i]);
        if (fits(boundary, z)){
            /*
            std:: cout << "---------------" << std::endl;
            std:: cout << boundary.bottom_x << std::endl;
            std:: cout << boundary.bottom_y << std::endl;
            std:: cout << boundary.top_x << std::endl;
            std:: cout << boundary.top_y << std::endl;
            */
            return i;
        }
    }
    return -1;
}

bool Quadtree:: fits(NodeBoundaries boundary, Zone z){

    if(z.shape == "CIRCLE"){
        return circleFitsInBoundary(boundary, z);
    } else if(z.shape == "TRIANGLE"){
        return triangleFitsInBoundary(boundary, z);
    } else if(z.shape == "RECTANGLE"){
        return rectangleFitsInBoundary(boundary, z);
    } 

    return false;
}

bool Quadtree:: pointFits(Zone z, Point location){

    if(z.shape == "CIRCLE"){
        return pointFitsInCircle(z, location);
    } else if(z.shape == "TRIANGLE"){
        return pointFitsInTriangle(z, location);
    } else if(z.shape == "RECTANGLE"){
        return pointFitsInRectangle(z, location);
    } 

    return false;
}

int Quadtree:: findPointQuadrant(std::vector<std::vector<float>> boundaries, Point location){

    for(int i = 0; i < 4; i++){
        NodeBoundaries boundary(boundaries[i]);
        if (pointFitsInBoundary(boundary, location)){
            return i;
        }
    }
    return -1;
}


std::vector<std::vector<float>> Quadtree:: split(NodeBoundaries boundary){
    std::vector<NodeBoundaries> tmp_zones;
    float mid_x = std::abs(boundary.bottom_x - boundary.top_x)/2;
    float mid_y = std::abs(boundary.bottom_y - boundary.top_y)/2;

    float left_bottom_x = boundary.bottom_x;
    float left_bottom_y = boundary.bottom_y;

    float left_mid_x = boundary.bottom_x;
    float left_mid_y = boundary.bottom_y + mid_y;

    float mid_mid_x = boundary.bottom_x + mid_x;
    float mid_mid_y = boundary.bottom_y + mid_y;

    float mid_top_x = boundary.bottom_x + mid_x;
    float mid_top_y = boundary.top_y;

    float mid_bottom_x = boundary.bottom_x + mid_x;
    float mid_bottom_y = boundary.bottom_y;

    float right_mid_x = boundary.top_x;
    float right_mid_y = boundary.bottom_y + mid_y;

    float right_top_x = boundary.top_x;
    float right_top_y = boundary.top_y;

    return {{mid_mid_x, mid_mid_y, right_top_x, right_top_y},
            {left_mid_x, left_mid_y, mid_top_x, mid_top_y}, 
            {left_bottom_x, left_bottom_y, mid_mid_x, left_mid_y}, 
            {mid_bottom_x, mid_bottom_y, right_mid_x, right_mid_y}};
}