#pragma once
#include "transport_catalogue.h"

#include <string>
#include <vector>
#include <iostream>
#include <string_view>
#include <deque>
#include <functional>
#include <unordered_map>

using namespace std::literals;

namespace input_reader {

std::vector<std::string_view> SplitIntoWords(const std::string_view& input);

catalogue::Query ParseToQuery(const std::vector<std::string_view>& string_container, const std::unordered_map<std::string_view, catalogue::Stop*>& stops_map);

catalogue::QueryType GetQueryTypeFromLine(const std::string_view& line);

template <class IStream>
class reader {
  private:
    std::vector<std::string> raw_queries;
    std::vector<catalogue::Query> queries;
    std::unordered_map<std::string_view, catalogue::Stop*> stops_map;
    std::unordered_map<std::string_view, catalogue::Bus*> buses_map;

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
        if (GetQueryTypeFromLine(line) == catalogue::QueryType::NewStop) {
          raw_queries.push_back(std::move(line));
        } else {
          bus_req.push_back(std::move(line));
        }
      }
      std::move(bus_req.begin(), bus_req.end(), std::back_inserter(raw_queries));

      //сделаем вектор запросов
      queries.reserve(raw_queries.size());
      for (auto it = raw_queries.begin(); it != raw_queries.end(); ++it){
        if (GetQueryTypeFromLine(*it) == catalogue::QueryType::NewStop) {
          queries.push_back(std::move(ParseToQuery(SplitIntoWords(*it), stops_map)));
          stops_map.insert({queries.back().stop.name, &queries.back().stop});
        } else {
          queries.push_back(std::move(ParseToQuery(SplitIntoWords(*it), stops_map)));
        }
      }
    }

    std::vector<catalogue::Query>& GetQueries() {
      return queries;
    };
};

}