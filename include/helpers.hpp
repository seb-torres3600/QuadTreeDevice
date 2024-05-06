#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <iostream>
#include <string>
#include <locale>

#include "structs.hpp"
#include "logger.hpp"


bool rectangleFitsInBoundary(NodeBoundaries boundary, Zone z);
bool circleFitsInBoundary(NodeBoundaries boundary, Zone z);
bool triangleFitsInBoundary(NodeBoundaries boundary, Zone z);

bool pointFitsInRectangle(Zone z, Point location);
bool pointFitsInCircle(Zone z,  Point location);
bool pointFitsInTriangle(Zone z,  Point location);

bool pointFitsInBoundary(NodeBoundaries boundary, Point location);

bool needToRefresh(NodeBoundaries boundary, Point location, float threshold);

#endif