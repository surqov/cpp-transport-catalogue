#include "stat_reader.h"

std::ostream& stat::operator<<(std::ostream& os, catalogue::BusInfo info_) {
    os << "Bus "s << info_.busname << ": "s;
    if (!info_.found) {
        os << "not found"s;
    } else {
        os << info_.stops_on_route << " stops on route, "s << info_.unique_stops << " unique stops, "s 
        << info_.route_len  << " route length, "s << info_.curvature << " curvature"s;
    } 
    return os;
}

std::ostream& stat::operator<<(std::ostream& os, catalogue::StopInfo info_) {
    os << "Stop "s << info_.stopname << ": "s;
    if (!info_.found) {
        os << "not found"s;
    } else if (info_.buses_to_stop.size() == 0) {
        os << "no buses"s;
    } else {
        os << "buses"s;
        for (const auto& b : info_.buses_to_stop) {
          os << " "s << b;
        }
    }
    return os;
}