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

bool Quadtree:: search(std::vector<float> point){
    std::vector<std::vector<float>> boundaries = split(nd_bndr);
    int quadrant = findPointQuadrant(boundaries, point);
    for(auto zn: zns){
        std::vector<float> coords = zn.coordinates;
        if(point[0] >= coords[0] && point[0] <= coords[2]){
            if(point[1] >= coords[1] && point[1] <= coords[3]){
                std::cout << "Found" << std::endl;
                std:: cout << "Point("<< point[0] << "," << point[1] <<") - found in zone: " << zn.zone_id << std::endl;
                return true;
            }
        }
    }
    std:: cout << quadrant+1 << "->";

    switch(quadrant){
        case 0:
            if(topRight == NULL){
                return false;
            }
            topRight->search(point);
            break;
        case 1:
            if(topLeft == NULL){
                return false;
            }
            topLeft->search(point);
            break;
        case 2:
            if(bottomLeft == NULL){
                return false;
            }
            bottomLeft->search(point);
            break;
        case 3:
            if(bottomRight == NULL){
                return false;
            }
            bottomRight->search(point);
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
        return circleFits(boundary, z);
    } else if(z.shape == "TRIANGLE"){
        return triangleFits(boundary, z);
    } else if(z.shape == "RECTANGLE"){
        return rectangleFits(boundary, z);
    } 

    return false;
}

bool Quadtree:: rectangleFits(NodeBoundaries boundary, Zone z){
    if(z.rectangle.bottomPoint.x >= boundary.bottom_x && z.rectangle.topPoint.x <= boundary.top_x){
        if(z.rectangle.bottomPoint.y >= boundary.bottom_y && z.rectangle.topPoint.y <= boundary.top_y){
            return true;
        }
    }
    return false;
}

bool Quadtree:: circleFits(NodeBoundaries boundary, Zone z){
    float left_x = z.circle.centerPoint.x - z.circle.radius;
    float lower_y = z.circle.centerPoint.y - z.circle.radius;
    float right_x = z.circle.centerPoint.x + z.circle.radius;
    float upper_y = z.circle.centerPoint.y + z.circle.radius;

    if(left_x >= boundary.bottom_x && right_x <= boundary.top_x){
        if(lower_y >= boundary.top_x && upper_y <= boundary.top_y){
            return true;
        }
    }
    return false;
}

bool Quadtree:: triangleFits(NodeBoundaries boundary, Zone z){
    int farthest_left = std::min(z.triangle.leftPoint.x, std:: min(z.triangle.centerPoint.x, z.triangle.rightPoint.x));
    int farthest_right = std::max(z.triangle.leftPoint.x, std:: max(z.triangle.centerPoint.x, z.triangle.rightPoint.x));

    int highest_point = std::max(z.triangle.leftPoint.y, std:: max(z.triangle.centerPoint.y, z.triangle.rightPoint.y));
    int lowest_point = std::min(z.triangle.leftPoint.y, std:: min(z.triangle.centerPoint.y, z.triangle.rightPoint.y));

    if (farthest_left >= boundary.bottom_x && farthest_right <= boundary.top_x){
        if(highest_point <= boundary.top_y && lowest_point >= boundary.bottom_y){
            return true;
        }
    }
    return false;
}

int Quadtree:: findPointQuadrant(std::vector<std::vector<float>> boundaries, std::vector<float> point){

    for(int i = 0; i < 4; i++){
        NodeBoundaries boundary(boundaries[i]);
        if (pointFits(boundary, point)){
            return i;
        }
    }
    return -1;
}

bool Quadtree:: pointFits(NodeBoundaries boundary, std::vector<float> point){
    if(point[0] >= boundary.bottom_x && point[0] <= boundary.top_x){
        if(point[1] >= boundary.bottom_y && point[1] <= boundary.top_y){
            return true;
        }
    }
    return false;
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