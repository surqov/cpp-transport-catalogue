#pragma once

#include "transport_catalogue.h"

#include <string>
#include <vector>
#include <iostream>
#include <string_view>

namespace reader {

enum class QueryType {
    NewBus,
    NewStop
};

struct Query {
    QueryType type;
    transport::Bus bus;
    transport::Stop stop;
};

const auto query_sorter = [](const Query& lhs, const Query& rhs) {
  return lhs.type == QueryType::NewStop && rhs.type != QueryType::NewStop;
};


template <class InStream>
class reader {
  private:
    std::vector<Query> sorted_queries;

  public:
    reader() = default;

    reader (InStream& input) {
      int num_of_lines;
      input >> num_of_lines;
      result.reserve(num_of_lines);
      std::string line;
      while (--num_of_lines != 0) {
          std::getline(input, line);
          
      }
      return result;
  }


std::vector<std::string> SplitIntoWords(const std::string& input);
std::vector<Query*> ParseToQuery(std::vector<std::string>& query);
std::vector<Query*> SortByQueryType(std::vector<Query*>& query);
}

}