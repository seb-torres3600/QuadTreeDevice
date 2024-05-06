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
 
NodeBoundaries Quadtree:: getBoundaries(){
    return nd_bndr;
}

void Quadtree:: insert(Zone z){
    std::vector<std::vector<float>> boundaries = split(nd_bndr);
    int quadrant = findQuadrant(boundaries, z);
    if(quadrant == -1){
        logger->info("Inserting zone ", std::to_string(z.zone_id));
        zns.push_back(z);
        return;
    }
    
    switch(quadrant){
        case 0:
            if(topRight == NULL){
                topRight = new Quadtree(boundaries[quadrant]);
            }
            topRight->insert(z);
            break;
        case 1:
            if(topLeft == NULL){
                topLeft = new Quadtree(boundaries[quadrant]);
            }
            topLeft->insert(z);
            break;
        case 2:
            if(bottomLeft == NULL){
                bottomLeft = new Quadtree(boundaries[quadrant]);
            }
            bottomLeft->insert(z);
            break;
        case 3:
            if(bottomRight == NULL){
                bottomRight = new Quadtree(boundaries[quadrant]);
            }
            bottomRight->insert(z);
            break;
        default:
            return;
    }
    return;
}

bool Quadtree:: search(Point location){
    Point center = getCenterPoint(nd_bndr);
    int quadrant = findPointQuadrant(center, location);
    int zone_number = 0;
    for(auto zn: zns){
        if (pointFits(zn, location)){
            logger->info("Point(" + std::to_string(location.x) + "," + std::to_string(location.y) +") - found in zone: " + std::to_string(zn.zone_id));
            return true;
        }
    }

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
    } else{
        logger->error("Unknown shape");
    }

    return false;
}

int Quadtree:: findPointQuadrant(Point center, Point location){
    /*
        Find what quadrant the point fits in 
    */
    // if location on right side of boundary
    if(center.x <= location.x){
        // if location above center line then we are in quadrant 1
        if(center.y <= location.y){
            return 1;
        }
        // otherwise we are in quadrant 4
        return 4;
    // if location on left side     
    } else {
        // if we are above center line, we are in quadrant 2
        if(center.y <= location.y){
            return 2;
        }
        // otherwise in quadrant 3
        return 3;
    }
    // otherwise we don't fit in any quadrant
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

Point Quadtree:: getCenterPoint(NodeBoundaries boundary){
    float mid_x = std::abs(boundary.bottom_x - boundary.top_x)/2;
    float mid_y = std::abs(boundary.bottom_y - boundary.top_y)/2;
    float mid_mid_x = boundary.bottom_x + mid_x;
    float mid_mid_y = boundary.bottom_y + mid_y;
    
    return Point(mid_mid_x, mid_mid_y);

}