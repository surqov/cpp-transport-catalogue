#pragma once

#include "transport_catalogue.h"

#include <string>
#include <vector>
#include <iostream>
#include <string_view>
#include <deque>
#include <functional>
#include <pair>

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

std::pair<bool, std::vector<std::string_view>> SplitIntoWords(const std::string_view& input) {
    std::vector<std::string_view> result;
    result.reserve(input.size() / 2);
    result.push_back(input.substr(0, input.find_first_of(' '))); // Bus or Stop
    result.push_back(input.substr(input.find_first_of(' ') + 1, input.find_first_of(':') - input.find_first_of(' ') - 1)); // Name of Bus or Stop
    std::string_view value = input.substr(input.find_first_of(':') + 1, input.size() - input.find_first_of(':') - 1); // Values to split

    bool symmetry = value.find('-');

    int64_t pos = value.find_first_not_of(" ");
    const int64_t pos_end = value.npos;
    
    while (pos != pos_end) {
      int64_t separator_pos = value.find_first_of(">,-", pos);
      result.push_back(separator_pos == pos_end ? value.substr(pos) : value.substr(pos, separator_pos - pos));
      pos = value.find_first_not_of(">,-", separator_pos);
    }

    return {symmetry, result};
}

Query ParseToQuery(std::vector<std::string>&& string_container, std::vector<Query>& queries) {
  Query result;
  std::string key = string_container.front();
  string_container.front().erase(0, string_container.front().find_first_of(' ') + 1);
  if (key == "Stop"s) {
    result.type = QueryType::NewStop;
    result.stop.name = string_container[1];
    result.stop.coordinates.lat = std::stold(string_container[1]);
    result.stop.coordinates.lng = std::stold(string_container[2]);
  } else {
    result.type = QueryType::NewBus;
    result.bus.name = string_container[1];
    

  } 
  return result;
}

void SortByQueryType(std::vector<std::string_view>& lines) {
  std::sort(lines.begin(), lines.end(), [](const std::string_view& lhs, const std::string_view& rhs) {
    return lhs.substr(0, lhs.find_first_of(' ')) == "Stop" && rhs.substr(0, rhs.find_first_of(' ')) == "Bus";
  });
}

QueryType GetQueryTypeFromLine(const std::string_view& line) {
  return line.substr(0, input.find_first_of(' ')) == "Bus"s ? QueryType::NewBus : QueryType::NewStop;
}

struct QueryHasher {
    size_t operator()(const Query& query){
        const QueryType& type_ = query.type;
        const std::string type_name = type_ == QueryType::NewBus ? "Bus"s : "Stop"s;
        const std::string& name_ = type_ == QueryType::NewBus ? query.bus.name : query.stop.name;
        return std::hash(type_name) + std::hash(name_);
    }
};

template <class InStream>
class reader {
  private:
    std::vector<Query> queries;
    std::unordered_map<std::string, *Stop> stops_map;
    
  public:
    reader (InStream& input) {
      //прочитаем список запросов на запись
      int num_of_lines;
      std::string line;
      std::getline(input, line);
      num_of_lines = std::stoi(line);
      std::deque<std::string> query_lines;
      query_lines.reserve(num_of_lines);
      for (int i = 0; i < num_of_lines; ++i) {
        std::getline(input, line);
        if (GetQueryTypeFromLine(line) == QueryType::NewStop) {
          query_lines.push_front(std::move(line));
        } else {
          query_lines.push_back(std::move(line));
        }
      }
      //сделаем вектор запросов
      queries.reserve(query_lines.size());
      for (auto it = query_lines.begin(); it != query_lines.end(); ++i){

        }
      }
    }

    const std::vector<Query>& GetQueries() const {
      return queries;
    }
};
}