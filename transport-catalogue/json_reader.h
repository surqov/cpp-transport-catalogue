#pragma once
#include "transport_catalogue.h"
#include "json.h"

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string_view>

using namespace std::literals;

namespace json_reader {

catalogue::QueryType GetQueryType(const json::Node& node);
catalogue::Query ParseToQuery(const json::Node& node, const std::unordered_map<std::string_view, catalogue::Stop*>& stops_map);
catalogue::Bus JsonToBus(const json::Document& doc_, const catalogue::transport_catalogue& catalogue_);
catalogue::Stop JsonToStop(const json::Node& doc_);

class reader {
    private:

    public:
}

}
/*
 * Здесь можно разместить код наполнения транспортного справочника данными из JSON,
 * а также код обработки запросов к базе и формирование массива ответов в формате JSON
 */