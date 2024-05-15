#include "../include/helpers.hpp"

bool rectangleFitsInBoundary(NodeBoundaries boundary, Zone z){
    if(z.rectangle.bottomPoint.x >= boundary.bottom_x && z.rectangle.topPoint.x <= boundary.top_x){
        if(z.rectangle.bottomPoint.y >= boundary.bottom_y && z.rectangle.topPoint.y <= boundary.top_y){
            return true;
        }
    }
    return false;
}

bool circleFitsInBoundary(NodeBoundaries boundary, Zone z){
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

bool triangleFitsInBoundary(NodeBoundaries boundary, Zone z){
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


bool pointFitsInBoundary(NodeBoundaries boundary, Point location){
    if(location.x >= boundary.bottom_x && location.x <= boundary.top_x){
        if(location.y >= boundary.bottom_y && location.y <= boundary.top_y){
            return true;
        }
    }
    return false;
}


bool pointFitsInRectangle(Zone z, Point location){
    if(z.rectangle.bottomPoint.x <= location.x && z.rectangle.topPoint.x >= location.x){
        if(z.rectangle.bottomPoint.y <= location.y && z.rectangle.topPoint.y >= location.y){
            return true;
        }
    }
    return false;
}

bool pointFitsInCircle(Zone z, Point location){
    float left_x = z.circle.centerPoint.x - z.circle.radius;
    float lower_y = z.circle.centerPoint.y - z.circle.radius;
    float right_x = z.circle.centerPoint.x + z.circle.radius;
    float upper_y = z.circle.centerPoint.y + z.circle.radius;

    if(left_x <= location.x && right_x >= location.x){
        if(lower_y <= location.x && upper_y >= location.y){
            return true;
        }
    }
    return false;
}

bool pointFitsInTriangle(Zone z, Point location){
    int farthest_left = std::min(z.triangle.leftPoint.x, std:: min(z.triangle.centerPoint.x, z.triangle.rightPoint.x));
    int farthest_right = std::max(z.triangle.leftPoint.x, std:: max(z.triangle.centerPoint.x, z.triangle.rightPoint.x));

    int highest_point = std::max(z.triangle.leftPoint.y, std:: max(z.triangle.centerPoint.y, z.triangle.rightPoint.y));
    int lowest_point = std::min(z.triangle.leftPoint.y, std:: min(z.triangle.centerPoint.y, z.triangle.rightPoint.y));

    if (farthest_right >= location.x && farthest_left >= location.x){
        if(highest_point >= location.y && lowest_point <= location.x){
            return true;
        }
    }
    return false;
}

bool needToRefresh(NodeBoundaries boundary, Point location, float threshold){
    float distance_to_top_x = std::abs(boundary.top_x - location.x);
    float distance_to_top_y = std::abs(boundary.top_y - location.y);
    float distance_to_bottom_x = std::abs(location.x - boundary.bottom_x);
    float distance_to_bottom_y = std::abs(location.y - boundary.bottom_y);

     return (distance_to_bottom_x <= threshold || 
            distance_to_bottom_y <= threshold || 
            distance_to_top_x <= threshold || 
            distance_to_top_y <= threshold);
}

void printUsage(){
    std:: cout << "Usage: " << std::endl;
}

std:: string convertAreaToString(std:: vector<float> areas){
    std:: string lower_x = std::to_string(areas[0]);
    std:: string lower_y = std::to_string(areas[1]);
    std:: string upper_x = std::to_string(areas[2]);
    std:: string upper_y = std::to_string(areas[3]);
    return lower_x + "," + lower_y + "," + upper_x + "," + upper_y;
}