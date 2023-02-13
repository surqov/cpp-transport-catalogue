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

std::vector<std::string> SplitIntoWords(const std::string&& input);
Query ParseToQuery(std::vector<std::string>&& string_container);
void SortByQueryType(std::vector<Query>& query_container);

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

    const std::vector<Query>& GetQueries() const;
};

}