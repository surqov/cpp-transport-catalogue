#include "json_reader.h"

namespace json_reader {

    catalogue::QueryType GetQueryType(const json::Node& node) {
        if (node.AsMap().at("type"s).AsString() == "Stop"s)
            return catalogue::QueryType::NewStop;
        else if (node.AsMap().at("type"s).AsString() == "Bus"s)
            return catalogue::QueryType::NewBus;
        else 
            throw std::logic_error("Unknown QueryType"s);
    }

    catalogue::Query ParseToQuery(const json::Node& node, const std::unordered_map<std::string_view, catalogue::Stop*>& stops_map) {
        catalogue::Query result;
        if (GetQueryType(node) == catalogue::QueryType::NewStop) {
            result.type = catalogue::QueryType::NewStop;
            result.stop = JsonToStop(node);
        } else {
            result.type = catalogue::QueryType::NewBus;
            result.bus = JsonToBus(node, stops_map);
        }
        return result;
    }

    catalogue::Stop JsonToStop(const json::Node& node) {
        catalogue::Stop result;
        result.name = node.AsMap().at("name"s).AsString();
        result.coordinates = { node.AsMap().at("latitude"s).AsDouble(), node.AsMap().at("longitude"s).AsDouble() };
        result.distances.reserve(node.AsMap().at("road_distances"s).AsMap().size());

        for (const auto& [name_, dist_] : node.AsMap().at("road_distances"s).AsMap()) {
            result.distances.push_back({name_, dist_.AsDouble()});
        }

        return result;
    }

    catalogue::Bus JsonToBus(const json::Node& node, const std::unordered_map<std::string_view, catalogue::Stop*>& stops_map) {
        catalogue::Bus result;
        result.name = node.AsMap().at("name"s).AsString();
        result.stops.reserve(node.AsMap().at("stops"s).AsArray().size());
        std::for_each(node.AsMap().at("stops"s).AsArray().begin(),
                    node.AsMap().at("stops"s).AsArray().end(),
                    [&result, &stops_map](const json::Node& node_){
                        result.stops.push_back(stops_map.at(node_.AsString()));
                    });

        if (node.AsMap().at("is_roundtrip"s).AsBool()) 
            result.stops.insert(result.stops.end(), result.stops.rbegin() + 1, result.stops.rend() - 2);
        
        return result;
    }

    reader::reader(json::Document& doc, catalogue::transport_catalogue& catalogue) {
        std::vector<catalogue::Query> bus_req;
        in_queries.reserve(doc.GetRoot().AsMap().at("base_requests"s).AsArray().size());
        bus_req.reserve(doc.GetRoot().AsMap().at("base_requests"s).AsArray().size());
        out_queries.reserve(doc.GetRoot().AsMap().at("stat_requests"s).AsArray().size());

        for (const json::Node& req : doc.GetRoot().AsMap().at("base_requests"s).AsArray()) {
            if (GetQueryType(req) == catalogue::QueryType::NewStop) {
                in_queries.emplace_back(ParseToQuery(req, catalogue.stopname_to_stop));
            } else {
                bus_req.emplace_back(ParseToQuery(req, catalogue.stopname_to_stop));
            }
        }

        std::move(bus_req.begin(), bus_req.end(), std::back_inserter(in_queries));

        FillCatalogue(catalogue);
        CalcPairDistances(catalogue);

        for (const json::Node& node_ : doc.GetRoot().AsMap().at("stat_requests"s).AsArray()) {
            catalogue::Query out_query;
            out_query.id = node_.AsMap().at("id"s).AsInt();
            out_query.name = node_.AsMap().at("name"s).AsString();
            out_query.type = GetQueryType(node_);
            if ((out_query.type == catalogue::QueryType::NewBus) && catalogue.FindBus(out_query.name)) {
                CalcRouteLen(catalogue, out_query.name);
            }
            out_queries.push_back(std::move(out_query));
      }



    }

    void reader::FillCatalogue(catalogue::transport_catalogue& catalogue) {
      for (catalogue::Query& query_ : in_queries) {
        if (query_.type == catalogue::QueryType::NewStop) {
          catalogue.AddStop(std::move(query_.stop));
        } else {
          catalogue.AddBus(std::move(query_.bus));
        }
      }
    }

    //добавляем расстояния в каталог для пар
    void reader::CalcPairDistances(catalogue::transport_catalogue& catalogue) {
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

    void reader::CalcRouteLen(catalogue::transport_catalogue& catalogue, const std::string_view& bus_name) {
      double geo_len = 0.0;
      double fact_len = 
        std::transform_reduce(catalogue.busname_to_bus.at(bus_name)->stops.begin(),
          catalogue.busname_to_bus.at(bus_name)->stops.end() - 1,
          catalogue.busname_to_bus.at(bus_name)->stops.begin() + 1,
          0.0,
          std::plus<double>(),
          [&geo_len, &catalogue] (const auto& lhs, const auto& rhs) {
            geo_len += geo::ComputeDistance(lhs->coordinates, rhs->coordinates);

            catalogue::Stop* lhs_stop = catalogue.stopname_to_stop.at(lhs->name);
            catalogue::Stop* rhs_stop = catalogue.stopname_to_stop.at(rhs->name);
            return catalogue.distances.find({lhs_stop, rhs_stop}) != catalogue.distances.end() ? 
                            catalogue.distances.at({lhs_stop, rhs_stop}) : 
                            catalogue.distances.at({rhs_stop, lhs_stop});
      });

      catalogue.bus_routes_geo.insert({catalogue.busname_to_bus.at(bus_name), geo_len});
      catalogue.bus_routes_fact.insert({catalogue.busname_to_bus.at(bus_name), fact_len});
    }

    std::vector<catalogue::Query>& reader::GetRawOutQueries() {
      return out_queries;
    }
}