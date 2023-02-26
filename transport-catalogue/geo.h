#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

namespace geo {

int EART_RADIUS = 6371000;
double PI = M_PI;
    
struct Coordinates {
    double lat;
    double lng;
    bool operator==(const Coordinates& other) const;
    bool operator!=(const Coordinates& other) const;
};

inline double ComputeDistance(Coordinates from, Coordinates to);

}