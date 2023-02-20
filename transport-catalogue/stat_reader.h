#pragma once

#include "transport_catalogue.h"

#include <cassert>
#include <sstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std::literals;

namespace stat {

enum class QueryType {
    BusInfo
};

struct Query {
    QueryType type;
    std::string busname;
};

struct BusInfo {
    std::string_view busname;
    int stops_on_route;
    int unique_stops;
    double route_len;
    bool founded;
};

std::ostream& operator<<(std::ostream& os, BusInfo& info_) {
    os << info_.busname << ": "s;
    if (!info_.founded) {
        os << "not found\n"s;
    } else {
        os << info_.stops_on_route << " stops on route, "s << info_.unique_stops << "unique stops, "s 
        << info_.route_len  << " route lenght\n";
    } 
    return os;
}

}