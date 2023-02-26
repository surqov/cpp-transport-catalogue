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
#include <sstream>

const double ACCURACY = 1e-6;
const char precision = 6;

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

class bus_catalogue {
  private:
    std::deque<Stop> stops;
    std::deque<Bus> buses;
    //std::unordered_map<std::string_view, Stop*> stopname_to_stop;
   // std::unordered_map<std::string_view, Bus*> busname_to_bus;
    //std::unordered_map<std::pair<Stop*, Stop*>, double, StopHash> distances;
    
  public:
    bus_catalogue(const std::vector<Query>& queries) {
        for (const Query& query_ : queries) {
            if (query_.type == QueryType::NewStop) {
                stops.push_back(std::move(query_.stop));
            } else {
                buses.push_back(std::move(query_.bus));
            }
        }
    }

    BusInfo GetBusInfo(const std::string_view& bus_name) const {
        BusInfo info_;
        info_.busname = bus_name;
        info_.founded = true;
        return info_;
    }
};