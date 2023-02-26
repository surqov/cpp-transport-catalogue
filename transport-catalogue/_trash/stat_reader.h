#pragma once
#include "transport_catalogue.h"

#include <cassert>
#include <sstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <string_view>

class bus_catalogue;

using namespace std::literals;

struct BusInfo {
    std::string_view busname;
    int stops_on_route;
    int unique_stops;
    double route_len;
    bool founded;
};

std::ostream& operator<<(std::ostream& os, BusInfo& info_) {
    os << info_.busname << ": "s;
    if (!info_.founded) {
        os << "not found\n"s;
    } else {
        os << info_.stops_on_route << " stops on route, "s << info_.unique_stops << "unique stops, "s 
        << info_.route_len  << " route lenght\n"s;
    } 
    return os;
}

template <class IStream>
class stat {
  private:
    std::vector<std::string> raw_queries;

  public:
    stat(IStream& input, bus_catalogue& catalogue_) {
      int num_of_lines;
      std::string line;
      std::getline(input, line);
      num_of_lines = std::stoi(line);
      raw_queries.reserve(num_of_lines);
      for (int i = 0; i < num_of_lines; ++i) {
        std::getline(input, line);
        raw_queries.push_back(line);
        std::string_view bus_name = s.substr(s.find_first_of(' ') + 1, s.find_last_not_of(' ') - s.find_first_of(' ') - 1);
        std::cout << catalogue_.GetBusInfo(bus_name);
      }
    }
};