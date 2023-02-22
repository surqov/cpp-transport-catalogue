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
#include <set>
#include <algorithm>

using namespace std::literals;

struct Stop {
    std::string_view name;
    geo::Coordinates coordinates; 
};

struct Bus {
    std::string_view name;
    std::vector<Stop*> stops;
    bool symmetry;
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

struct StopInfo {
    std::string_view stopname;
    bool founded = false;
    std::set<Bus*> buses_to_stop;
};

class bus_catalogue {
  private:
    std::deque<Stop> stops;
    std::deque<Bus> buses;
    std::unordered_map<std::string_view, Stop*> stopname_to_stop;
    std::unordered_map<std::string_view, Bus*> busname_to_bus;
    std::unordered_map<std::string_view, std::set<Bus*>> stops_to_bus;
    
  public:
    bus_catalogue(std::vector<Query>& queries) {
        for (Query& query_ : queries) {
            if (query_.type == QueryType::NewStop) {
                std::string_view name_ = query_.stop.name;
                stops.push_back(std::move(query_.stop));
                stopname_to_stop[name_] = &stops.back();
            } else {
                std::string_view name_ = query_.bus.name;
                buses.push_back(std::move(query_.bus));
                busname_to_bus[name_] = &buses.back();
                std::for_each(busname_to_bus[name_]->stops.begin(),
                    busname_to_bus[name_]->stops.end(),
                    [this, name_](const auto& stop_) {
                        stops_to_bus[stop_->name].insert(busname_to_bus.at(name_));
                    });
            }
        }
    }

    BusInfo GetBusInfo(const std::string_view& bus_name) const {
        BusInfo info_;
        info_.busname = bus_name;
        info_.founded = busname_to_bus.find(bus_name) != busname_to_bus.end();
        if (info_.founded) {
            info_.stops_on_route = busname_to_bus.at(bus_name)->stops.size();
            info_.unique_stops = std::set(busname_to_bus.at(bus_name)->stops.begin(), busname_to_bus.at(bus_name)->stops.end()).size();
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

    StopInfo GetStopInfo(const std::string_view& stop_name) const {
        StopInfo info_;
        info_.stopname = stop_name;
        info_.founded = stops_to_bus.find(stop_name) != stops_to_bus.end();
        if (info_.founded) {
            info_.buses_to_stop = stops_to_bus.at(stop_name);
        }
        return info_;
    }
};