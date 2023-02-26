#pragma once

#include "geo.h"

#include <string>
#include <vector>
#include <iostream>
#include <string_view>
#include <deque>
#include <functional>
#include <unordered_map>

using namespace std::literals;

struct Stop {
    std::string_view name;
    geo::Coordinates coordinates; 
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

std::vector<std::string_view> SplitIntoWords(const std::string_view& input) {
    std::vector<std::string_view> result;
    result.reserve(input.size() / 2);
    result.push_back(input.substr(0, input.find_first_of(' '))); // Bus or Stop
    result.push_back(input.substr(input.find_first_of(' ') + 1, input.find_first_of(':') - input.find_first_of(' ') - 1)); // Name of Bus or Stop
    std::string_view value = input.substr(input.find_first_of(':') + 1, input.size() - input.find_first_of(':') - 1); // Values to split

    bool symmetry = value.find(" - "s) != value.npos;

    int64_t pos = value.find_first_not_of(" ");
    const int64_t pos_end = value.npos;
    
    while (pos != pos_end) {
      int64_t separator_pos = value.find_first_of(">,-", pos);
      std::string_view name_ = separator_pos == pos_end ? value.substr(pos + 1)  : value.substr(pos, separator_pos - pos);
      result.push_back(name_.substr(name_.find_first_not_of(' '), name_.find_last_not_of(' ') - name_.find_first_not_of(' ') + 1));
      pos = value.find_first_not_of(">,-", separator_pos);
    }

    if (symmetry) result.insert(result.end(), result.rbegin(), result.rend() - 2);

    return result;
}

Query ParseToQuery(const std::vector<std::string_view>& string_container, const std::unordered_map<std::string_view, Stop*>& stops_map) {
  Query result;
  std::string_view key = string_container.front();
  if (key == "Stop"s) {
    result.type = QueryType::NewStop;
    result.stop.name = string_container[1];
    result.stop.coordinates.lat = std::stold(std::string(string_container[2]));
    result.stop.coordinates.lng = std::stold(std::string(string_container[3]));
  } else {
    result.type = QueryType::NewBus;
    result.bus.name = string_container[1];
    for (auto it = string_container.begin() + 2; it != string_container.end(); ++it) {
      result.bus.stops.push_back(stops_map.at(*it));
    }
  }
  return result;
}

QueryType GetQueryTypeFromLine(const std::string_view& line) {
  return line.substr(0, line.find_first_of(' ')) == "Bus"s ? QueryType::NewBus : QueryType::NewStop;
}

template <class IStream>
class reader {
  private:
    std::vector<std::string> raw_queries;
    std::vector<Query> queries;
    std::unordered_map<std::string_view, Stop*> stops_map;

  public:
    reader (IStream& input) {
      //прочитаем список запросов на запись
      int num_of_lines;
      std::string line;
      std::getline(input, line);
      num_of_lines = std::stoi(line);
      raw_queries.reserve(num_of_lines);

      std::vector<std::string> bus_req;
      bus_req.reserve(num_of_lines);

      for (int i = 0; i < num_of_lines; ++i) {
        std::getline(input, line);
        if (GetQueryTypeFromLine(line) == QueryType::NewStop) {
          raw_queries.push_back(std::move(line));
        } else {
          bus_req.push_back(std::move(line));
        }
      }
      std::move(bus_req.begin(), bus_req.end(), std::back_inserter(raw_queries));

      //сделаем вектор запросов
      queries.reserve(raw_queries.size());
      for (auto it = raw_queries.begin(); it != raw_queries.end(); ++it){
        if (GetQueryTypeFromLine(*it) == QueryType::NewStop) {
          queries.push_back(std::move(ParseToQuery(SplitIntoWords(*it), stops_map)));
          stops_map.insert({queries.back().stop.name, &queries.back().stop});
        } else {
          queries.push_back(std::move(ParseToQuery(SplitIntoWords(*it), stops_map)));
        }
      }
    }

    const std::vector<Query>& GetQueries() const {
      return queries;
    }
};