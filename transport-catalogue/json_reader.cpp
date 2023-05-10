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
        std::for_each(node.AsMap().at("road_distances"s).AsMap().begin(),
                    node.AsMap().at("road_distances"s).AsMap().end(),
                    [&result](const std::pair<std::string_view, double>& dist_){ // перебор по node д б
                        result.distances.push_back(dist_);
                    });
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
    }




}