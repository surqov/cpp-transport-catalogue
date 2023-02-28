#pragma once
#include "input_reader.h"
#include "transport_catalogue.h"

#include <cassert>
#include <sstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <string_view>
#include <set>

using namespace std::literals;

namespace stat {

std::ostream& operator<<(std::ostream& os, catalogue::BusInfo info_);

std::ostream& operator<<(std::ostream& os, catalogue::StopInfo info_);

template <class OStream>
class stat_reader {
  private: 
    std::vector<std::string> raw_queries;

  public:
    stat_reader(catalogue::transport_catalogue& catalog, OStream& output) {
      int num_of_lines;
      std::string line;
      std::getline(input, line);
      num_of_lines = std::stoi(line);
      raw_queries.reserve(num_of_lines);
      for (int i = 0; i < num_of_lines; ++i) {
        std::getline(input, line);
        raw_queries.push_back(line);
        std::string name_ = line.substr(line.find_first_of(' ') + 1, line.find_last_not_of(' ') - line.find_first_of(' '));
        if (input_reader::GetQueryTypeFromLine(line) == catalogue::QueryType::NewBus) {
          output << catalog.GetBusInfo(name_) << "\n";
        } else {
          output << catalog.GetStopInfo(name_) << "\n";
        }
      }
    }
};

}