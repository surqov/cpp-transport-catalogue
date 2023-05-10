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
    std::vector<std::string> raw_queries; // где используется?

  public:
    stat_reader(const std::vector<std::string>& queries, catalogue::transport_catalogue& catalog, OStream& output) {
      for (const auto& line : queries) {
        std::string name_ = line.substr(line.find_first_of(' ') + 1, line.find_last_not_of(' ') - line.find_first_of(' '));
        if (input_reader::GetQueryTypeFromLine(line) == catalogue::QueryType::NewBus) {
          output << catalog.GetBusInfo(name_) << "\n";
        } else {
          output << catalog.GetStopInfo(name_) << "\n";
        }
      }
    }

    stat_reader(const std::vector<catalogue::Query>& queries, catalogue::transport_catalogue& catalog, OStream& output) {
      for (const catalogue::Query& query_ : queries) {
        if (query_.type == catalogue::QueryType::NewBus) {
          output << catalog.GetBusInfo(query_.name) << "\n";
        } else {
          output << catalog.GetStopInfo(query_.name) << "\n";
        }
      }
    }
};

}