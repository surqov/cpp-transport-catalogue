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
#include <unordered_set>

using namespace std::literals;

namespace catalogue {
struct Stop {
    std::string_view name;
    geo::Coordinates coordinates; 
    std::vector<std::pair<std::string_view, double>> distances;
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
    bool found = false;
    int stops_on_route = 0;
    int unique_stops = 0;
    double route_len = 0.0;
    double curvature = 0.0;
};

struct StopInfo {
    std::string_view stopname;
    bool found = false;
    std::set<std::string_view> buses_to_stop; //не unordered т.к. сортируем вывод лексиграфически
};

struct StopPairHasher {
    size_t operator()(const std::pair<Stop*, Stop*>& stop) const;
};

struct BusHasher {
    size_t operator()(const Bus* bus) const;
};

class transport_catalogue {
   public:
    std::deque<Stop> stops; 
    std::deque<Bus> buses; 
    std::unordered_map<std::string_view, Stop*> stopname_to_stop;
    std::unordered_map<std::string_view, Bus*> busname_to_bus; 
    std::unordered_map<std::string_view, std::set<std::string_view>> stopname_to_busesnames;
    std::unordered_map<Stop*, std::unordered_set<Bus*, BusHasher>> stop_to_buses;
    std::unordered_map<std::pair<Stop*, Stop*>, double, StopPairHasher> distances;
    std::unordered_map<Bus*, double, BusHasher> bus_routes_geo;
    std::unordered_map<Bus*, double, BusHasher> bus_routes_fact;
    
    transport_catalogue() = default;

    void AddBus(Bus&& bus);
    void AddStop(Stop&& stop);

    bool FindBus(const std::string_view& bus_name) const noexcept;
    bool FindStop(const std::string_view& stop_name) const noexcept;

    BusInfo GetBusInfo(const std::string_view& bus_name) const;
    StopInfo GetStopInfo(const std::string_view& stop_name) const;
};

}