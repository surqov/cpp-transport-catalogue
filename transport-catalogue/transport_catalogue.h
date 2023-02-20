#pragma once

#include "geo.h"
#include "input_reader.h"
#include "stat_reader.h"

#include <deque>
#include <string>
#include <unordered_map>
#include <string_view>
#include <iomanip>
#include <vector>
#include <numeric>

const double ACCURACY = 1e-6;
const char precision = 6;

namespace transport { 

struct Stop {
    std::string_view name;
    geo::Coordinates coordinates; 
};

struct Bus {
    std::string_view name;
    std::vector<Stop*> stops;
};

//функция хеширования до size_t для distances из долготы и широты домножением на единый множитель
struct StopHash {
    size_t operator()(const std::pair<Stop*, Stop*> stop){
        const double lat1 = stop.first->coordinates.lat;
        const double lng1 = stop.first->coordinates.lat;
        const double lat2 = stop.second->coordinates.lat;
        const double lng2 = stop.second->coordinates.lat;
        return lat1 * 37 + lng1 * 37 * 37 + lat2 * 37 * 37 * 37 + lng2 * 37 * 37 * 37 * 37;
    }
};



}