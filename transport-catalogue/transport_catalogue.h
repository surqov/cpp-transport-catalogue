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
#include <iostream>
#include <set>
#include <algorithm>

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
    bool symmetry;
};

struct StopPairHasher {
    size_t operator()(const std::pair<Stop*, Stop*>& stop) const;
};

class transport_catalogue {
  private:
    std::deque<Stop> stops;
    std::deque<Bus> buses;
    std::unordered_map<std::string_view, Stop*> stopname_to_stop;
    std::unordered_map<std::string_view, Bus*> busname_to_bus;
    std::unordered_map<std::string_view, std::set<Bus*>> stops_to_bus;
    std::unordered_map<std::string_view, std::set<std::string_view>> stopname_to_busname;
    std::unordered_map<std::pair<Stop*, Stop*>, double, StopPairHasher> distances;
    
  public:
    transport_catalogue(std::vector<input_reader::Query>& queries);
    stat::BusInfo GetBusInfo(const std::string_view& bus_name) const;
    stat::StopInfo GetStopInfo(const std::string_view& stop_name) const;
};

}