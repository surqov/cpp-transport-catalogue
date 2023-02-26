#include "geo.h"

bool geo::Coordinates::operator==(const geo::Coordinates& other) const {
    return lat == other.lat && lng == other.lng;
}

bool geo::Coordinates::operator!=(const geo::Coordinates& other) const {
    return !(*this == other);
}

double geo::ComputeDistance(geo::Coordinates from, geo::Coordinates to){
if (from == to) return 0;
static const double dr = M_PI / 180.;
return std::acos(std::sin(from.lat * dr) * std::sin(to.lat * dr)
            + std::cos(from.lat * dr) * std::cos(to.lat * dr) * std::cos(std::abs(from.lng - to.lng) * dr))
    * 6371000;
}