#include "input_reader.h"

std::vector<std::string> reader::SplitIntoWords(const std::string&& input) {
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

reader::Query reader::ParseToQuery(std::vector<std::string>&& string_container) {
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

void reader::SortByQueryType(std::vector<Query>& query_container) {
  std::sort(query_container.begin(), query_container.end(), [](const Query& lhs, const Query& rhs) {
    return lhs.type == QueryType::NewStop && rhs.type == QueryType::NewBus;
  });
}

const std::vector<reader::Query>& reader::reader::GetQueries() const {
    return queries;
}