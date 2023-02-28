#pragma once
#include "transport_catalogue.h"

#include <string>
#include <vector>
#include <iostream>
#include <string_view>
#include <deque>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <iterator>

using namespace std::literals;

namespace input_reader {

std::vector<std::string_view> SplitIntoWords(const std::string_view& input);

catalogue::Query ParseToQuery(const std::vector<std::string_view>& string_container, const std::unordered_map<std::string_view, catalogue::Stop*>& stops_map);

catalogue::QueryType GetQueryTypeFromLine(const std::string_view& line);

template <class IStream>
class reader {
  private:
    std::vector<std::string> raw_in_queries;
    std::vector<std::string> raw_out_queries;

  public:
    reader (IStream& input, catalogue::transport_catalogue& catalogue) {
    //прочитаем список запросов на запись
      int num_of_lines;
      std::string line;
      std::getline(input, line);
      num_of_lines = std::stoi(line); // кол-во линий для записи в БД
      raw_in_queries.reserve(num_of_lines);

      std::vector<std::string> bus_req;
      bus_req.reserve(num_of_lines);

      // чтение запросов, сначала остановки и после маршруты - складываем в общую очередь запросов на запись в БД
      for (int i = 0; i < num_of_lines; ++i) {
        std::getline(input, line);
        if (GetQueryTypeFromLine(line) == catalogue::QueryType::NewStop) {
          raw_in_queries.push_back(std::move(line));
        } else {
          bus_req.push_back(std::move(line));
        }
      }
      std::move(bus_req.begin(), bus_req.end(), std::back_inserter(raw_in_queries)); 
      // теперь у нас есть сырые запросы в порядке сначала Stop и только после Bus

      FillCatalogue(catalogue);
      CalcPairDistances(catalogue);

      // прочитаем какие запросы у нас есть для вывода сразу, чтобы правильно построить каталог
      std::getline(input, line);
      num_of_lines = std::stoi(line); 
      raw_out_queries.reserve(num_of_lines);
      for (int i = 0; i < num_of_lines; ++i) {
        std::string name_ = line.substr(line.find_first_of(' ') + 1, line.find_last_not_of(' ') - line.find_first_of(' '));
        if (GetQueryTypeFromLine(line) == catalogue::QueryType::NewBus) {
          CalcRouteLen(catalogue, name_);
        }
        raw_out_queries.push_back(std::move(line));
      }
    }

    // наполняем каталог
    void FillCatalogue(catalogue::transport_catalogue& catalogue) {
      for (const std::string_view& raw_query : raw_in_queries) {
        catalogue::Query query_ = ParseToQuery(SplitIntoWords(raw_query), catalogue.stopname_to_stop);
        if (query_.type == catalogue::QueryType::NewStop) {
          catalogue.AddStop(std::move(query_.stop));
        } else {
          catalogue.AddBus(std::move(query_.bus));
        }
      }
    }

    //добавляем расстояния в каталог для пар
    void CalcPairDistances(catalogue::transport_catalogue& catalogue) {
      for (const auto& stop : catalogue.stops) {
      std::string_view name_ = stop.name;
      std::for_each(stop.distances.begin(),
                stop.distances.end(),
                [&catalogue, name_](const auto& pair_) {
                    catalogue.distances.insert({{
                            catalogue.stopname_to_stop[name_], catalogue.stopname_to_stop[pair_.first]}, 
                            pair_.second});
                    });
      }
    }

    //добавляем длины маршрутов по их географическим координатам и по фактическим длинам
    void CalcRouteLen(catalogue::transport_catalogue& catalogue, const std::string_view& bus_name) {
      double geo_len = 0.0;
      double fact_len = 0.0;

      for (const auto* lhs : catalogue.busname_to_bus.at(bus_name)->stops) {
        const auto* rhs = std::next(lhs);
        geo_len += geo::ComputeDistance(lhs->coordinates, rhs->coordinates);

        catalogue::Stop* lhs_stop = catalogue.stopname_to_stop.at(lhs->name);
        catalogue::Stop* rhs_stop = catalogue.stopname_to_stop.at(rhs->name);
        fact_len += catalogue.distances.find({lhs_stop, rhs_stop}) != catalogue.distances.end() ? 
                        catalogue.distances.at({lhs_stop, rhs_stop}) : 
                        catalogue.distances.at({rhs_stop, lhs_stop});
      }

      catalogue.bus_routes_geo.insert({catalogue.busname_to_bus.at(bus_name), geo_len});
      catalogue.bus_routes_geo.insert({catalogue.busname_to_bus.at(bus_name), fact_len});
    }

    std::vector<std::string>& GetRawOutQueries() {
      return raw_out_queries;
    }
};

}