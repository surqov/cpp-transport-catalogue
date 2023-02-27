#pragma once
#include "transport_catalogue.h"

#include <string>
#include <vector>
#include <iostream>
#include <string_view>
#include <deque>
#include <functional>
#include <unordered_map>
#include <tuple>

using namespace std::literals;

namespace input_reader {



std::vector<std::string_view> SplitIntoWords(const std::string_view& input);

catalogue::Query ParseToQuery(const std::vector<std::string_view>& string_container, const std::unordered_map<std::string_view, catalogue::Stop*>& stops_map);

catalogue::QueryType GetQueryTypeFromLine(const std::string_view& line);

template <class IStream>
std::tuple<std::vector<std::string>,
          std::vector<catalogue::Query>,
          std::unordered_map<std::string_view, catalogue::Stop*>> LoadFromStream(IStream& instream) {
  std::vector<std::string> raw_queries;
  std::vector<catalogue::Query> queries;
  std::unordered_map<std::string_view, catalogue::Stop*> stops_map;

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

  return std::make_tuple(raw_queries, queries, stops_map);
}

class reader {
  private:
    std::vector<std::string> raw_queries;
    std::vector<catalogue::Query> queries;
    std::unordered_map<std::string_view, catalogue::Stop*> stops_map;

  public:
    reader() = default;

    reader(std::tuple<std::vector<std::string>&, std::vector<catalogue::Query>& r_queries,
          const std::unordered_map<std::string_view, catalogue::Stop*>& r_stops_map) noexcept : 
          raw_queries(std::move(r_raw_queries)), queries(std::move(r_queries)), stops_map(std::move(r_stops_map)) {}

    reader& operator=(const BasicIterator& rhs) :
    

    std::vector<catalogue::Query>& GetQueries() {
      return queries;
    };
};

}