#include "input_reader.h"

std::vector<std::string_view> input_reader::SplitIntoWords(const std::string_view& input) {
    std::vector<std::string_view> result;
    result.reserve(input.size() / 2);
    result.push_back(input.substr(0, input.find_first_of(' '))); // Bus or Stop
    result.push_back(input.substr(input.find_first_of(' ') + 1, input.find_first_of(':') - input.find_first_of(' ') - 1)); // Name of Bus or Stop
    std::string_view value = input.substr(input.find_first_of(':') + 1, input.size() - input.find_first_of(':') - 1); // Values to split

    bool symmetry = value.find(" - "s) != value.npos;

    int64_t pos = value.find_first_not_of(" ");
    const int64_t pos_end = value.npos;
    
    std::string separator_chars = result[0] == "Bus"s ? ">-"s : ","s; // Choose separator

    while (pos != pos_end) {
      int64_t separator_pos = value.find_first_of(separator_chars, pos);
      std::string_view name_ = separator_pos == pos_end ? value.substr(pos + 1)  : value.substr(pos, separator_pos - pos);
      result.push_back(name_.substr(name_.find_first_not_of(' '), name_.find_last_not_of(' ') - name_.find_first_not_of(' ') + 1));
      pos = value.find_first_not_of(">,-", separator_pos);
    }

    if (symmetry) result.insert(result.end(), result.rbegin() + 1, result.rend() - 2);

    return result;
}

input_reader::Query input_reader::ParseToQuery(const std::vector<std::string_view>& string_container, const std::unordered_map<std::string_view, catalogue::Stop*>& stops_map) {
  Query result;
  std::string_view key = string_container.front();
  if (key == "Stop"s) {
    result.type = QueryType::NewStop;
    result.stop.name = string_container[1];
    result.stop.coordinates.lat = std::stold(std::string(string_container[2]));
    result.stop.coordinates.lng = std::stold(std::string(string_container[3]));
    if (string_container.size() > 4) {
      for (size_t i = 4; i < string_container.size(); ++i) {
        std::string_view curr_line = string_container[i];
        std::string_view to_distance = curr_line.substr(0, curr_line.find_first_of('m'));
        std::string_view to_stop = curr_line.substr(curr_line.find("to "s) + 3, curr_line.size() - curr_line.find("to "s) - 3);
        result.stop.distances.push_back({to_stop, std::stod(std::string(to_distance))});
      }
    }
  } else {
    result.type = QueryType::NewBus;
    result.bus.name = string_container[1];
    for (auto it = string_container.begin() + 2; it != string_container.end(); ++it) {
      result.bus.stops.push_back(stops_map.at(*it));
    }
  }
  return result;
}

input_reader::QueryType input_reader::GetQueryTypeFromLine(const std::string_view& line) {
  return line.substr(0, line.find_first_of(' ')) == "Bus"s ? QueryType::NewBus : QueryType::NewStop;
}
