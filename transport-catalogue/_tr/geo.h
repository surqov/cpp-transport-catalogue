#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

namespace geo {
    
struct Coordinates {
    double lat;
    double lng;
    bool operator==(const Coordinates& other) const;
    bool operator!=(const Coordinates& other) const;
};

double ComputeDistance(Coordinates from, Coordinates to);

}