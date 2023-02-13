#pragma once

#include "transport_catalogue.h"

#include <string>
#include <vector>
#include <iostream>
#include <string_view>

using namespace std::literals;

namespace reader {

enum class QueryType {
    NewStop,
    NewBus
};

struct Query {
    QueryType type;
    transport::Bus bus;
    transport::Stop stop;
};

std::vector<std::string> SplitIntoWords(const std::string&& input) {
    std::vector<std::string> result;
    result.reserve(input.size() / 2);
    result.push_back(input.substr(0, input.find_first_of(':')));
    std::string value = input.substr(input.find_first_of(':') + 1, input.size() - input.find_first_of(':') - 1);
    size_t pos = 0;
    while (pos != value.npos) {
      pos = value.find_first_of(">,-");
      result.push_back(value.substr(1, pos - 2));
      value.erase(0, pos + 1);
    }
    return result;
}

Query ParseToQuery(std::vector<std::string>&& string_container) {
  Query result;
  std::string key = string_container.front().substr(0, string_container.front().find_first_of(' '));
  string_container.front().erase(0, string_container.front().find_first_of(' ') + 1);
  if (key == "Bus"s) {
    result.type = QueryType::NewBus;
    result.bus.name = string_container[0];
    result.bus.stops.reserve(string_container.size());
    for (auto it = string_container.begin() + 1; it != string_container.end(); ++it) {
      result.bus.stops.push_back(*it);
    }
  } else {
    result.type = QueryType::NewStop;
    result.stop.name = string_container[0];
    result.stop.coordinates.lat = std::stold(string_container[1]);
    result.stop.coordinates.lng = std::stold(string_container[2]);
  } 
  return result;
}

void SortByQueryType(std::vector<Query>& query_container) {
  std::sort(query_container.begin(), query_container.end(), [](const Query& lhs, const Query& rhs) {
    return lhs.type == QueryType::NewStop && rhs.type == QueryType::NewBus;
  });
}

template <class InStream>
class reader {
  private:
    std::vector<Query> queries;

  public:
    reader (InStream& input) {
      std::vector<Query> temp_container;
      int num_of_lines;
      std::string line;
      std::getline(input, line);
      num_of_lines = std::stoi(line);
      temp_container.reserve(num_of_lines);
      while (num_of_lines != 0) {
          std::getline(input, line);
          Query temp_query = ParseToQuery((SplitIntoWords(std::move(line))));
          queries.push_back(std::move(temp_query));
          --num_of_lines;
      }
      SortByQueryType(queries);
    }

    const std::vector<Query>& GetQueries() const {
      return queries;
    }
};

}