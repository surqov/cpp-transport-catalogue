#pragma once
#include "json.h"
#include "transport_catalogue.h"

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string_view>
#include <iostream>

using namespace std::literals;

namespace json_reader {

catalogue::QueryType GetQueryType(const json::Node& node);
catalogue::Query ParseToQuery(const json::Node& node, const std::unordered_map<std::string_view, catalogue::Stop*>& stops_map);
catalogue::Bus JsonToBus(const json::Node& node, const std::unordered_map<std::string_view, catalogue::Stop*>& stops_map);
catalogue::Stop JsonToStop(const json::Node& node);

json::Dict BusInfoToJson(const catalogue::BusInfo& bus_info_);
json::Dict StopInfoToJson(const catalogue::StopInfo& stop_info_);

void JsonInfoPrint(const std::vector<catalogue::Query>& out_queries, const catalogue::transport_catalogue& catalogue, std::ostream& out);

class reader {
    private:
    std::vector<json::Node> in_queries;
    std::vector<catalogue::Query> out_queries;

    public:
        reader(json::Document& doc, catalogue::transport_catalogue& catalogue);
        void FillCatalogue(catalogue::transport_catalogue& catalogue);
        void CalcPairDistances(catalogue::transport_catalogue& catalogue);
        void CalcRouteLen(catalogue::transport_catalogue& catalogue, const std::string_view& bus_name);
        std::vector<catalogue::Query>& GetRawOutQueries();
};


}