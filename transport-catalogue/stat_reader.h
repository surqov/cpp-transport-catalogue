#pragma once
#include "transport_catalogue.h"
#include "input_reader.h"

#include <cassert>
#include <sstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <string_view>

using namespace std::literals;

std::ostream& operator<<(std::ostream& os, BusInfo info_) {
    os << "Bus "s << info_.busname << ": "s;
    if (!info_.founded) {
        os << "not found"s;
    } else {
        os << info_.stops_on_route << " stops on route, "s << info_.unique_stops << " unique stops, "s 
        << info_.route_len  << " route length"s;
    } 
    return os;
}

std::ostream& operator<<(std::ostream& os, StopInfo info_) {
    os << "Stop "s << info_.stopname << ": "s;
    if (!info_.founded) {
        os << "not found"s;
    } else if (info_.buses_to_stop.size() == 0) {
        os << "no buses"s;
    } else {
        os << "buses"s;
        for (const Bus* b : info_.buses_to_stop) {
          os << " "s << b->name;
        }
    }
    return os;
}

template <class IStream>
class stat {
  private: 
    std::vector<std::string> raw_queries;

  public:
    stat(IStream& input, bus_catalogue& catalog) {
      int num_of_lines;
      std::string line;
      std::getline(input, line);
      num_of_lines = std::stoi(line);
      raw_queries.reserve(num_of_lines);
      for (int i = 0; i < num_of_lines; ++i) {
        std::getline(input, line);
        raw_queries.push_back(line);
        std::string name_ = line.substr(line.find_first_of(' ') + 1, line.find_last_not_of(' ') - line.find_first_of(' '));
        if (GetQueryTypeFromLine(line) == QueryType::NewBus) {
          std::cout << catalog.GetBusInfo(name_) << std::endl;
        } else {
          std::cout << catalog.GetStopInfo(name_) << std::endl;
        }
      }
    }
};