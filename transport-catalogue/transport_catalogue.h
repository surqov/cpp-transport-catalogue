#pragma once
#include "geo.h"

#include <deque>
#include <string>
#include <unordered_map>
#include <string_view>
#include <iomanip>
#include <vector>
#include <numeric>
#include <sstream>
#include <iostream>

using namespace std::literals;

struct Stop {
    std::string_view name;
    geo::Coordinates coordinates; 
};

struct Bus {
    std::string_view name;
    std::vector<Stop*> stops;
};

enum class QueryType {
    NewStop,
    NewBus
};

struct Query {
    QueryType type;
    Bus bus;
    Stop stop;
};

struct BusInfo {
    std::string_view busname;
    bool founded = false;
    int stops_on_route = 0;
    int unique_stops = 0;
    double route_len = 0.0;
};

class bus_catalogue {
  private:
    //std::deque<Stop> stops;
    std::deque<Bus> buses;
    std::unordered_map<std::string_view, Stop*> stopname_to_stop;
    std::unordered_map<std::string_view, Bus*> busname_to_bus;
    
  public:
    bus_catalogue(std::vector<Query>& queries) {
        for (Query& query_ : queries) {
            if (query_.type == QueryType::NewStop) {
                stopname_to_stop[query_.stop.name] = &query_.stop;
                //stops.push_back(std::move(query_.stop));
            } else {
                busname_to_bus[query_.bus.name] = &query_.bus;
                //buses.push_back(std::move(query_.bus));
            }
        }
    }

    BusInfo GetBusInfo(const std::string_view& bus_name) const {
        BusInfo info_;
        info_.busname = bus_name;
        info_.founded = busname_to_bus.find(bus_name) != busname_to_bus.end();
        if (info_.founded) {
            info_.stops_on_route = busname_to_bus.at(bus_name)->stops.size();
            info_.unique_stops = 
            info_.route_len = std::transform_reduce(
                busname_to_bus.at(bus_name)->stops.begin(),
                busname_to_bus.at(bus_name)->stops.end() - 1,
                busname_to_bus.at(bus_name)->stops.begin() + 1,
                0.0,
                std::plus<double>(),
                [](const auto& lhs, const auto& rhs) {
                    return geo::ComputeDistance(lhs->coordinates, rhs->coordinates);
                }
            );
            }
        return info_;
    }
};