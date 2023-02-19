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
    os << std::setprecision(precision);
    os << info_.busname << ": "s;
    if (!info_.founded) {
        os << "not found\n"s;
    } else {
        os << info_.stops_on_route << " stops on route, "s << info_.unique_stops << "unique stops, "s 
        << info_.route_len  << " route lenght\n";
    } 
    return os;
}


template <class IStream, class OStream>
class BusManager {
private:
    map<string, vector<string>> buses_to_stops_, stops_to_buses_;
                                //#1 Авт - Ост     #2 Ост - Авт
public:
    void AddBus(const string& bus, const vector<string>& stops) {
        buses_to_stops_[bus] = stops;
        for (const string& stop : stops) {
            stops_to_buses_[stop].push_back(bus);
        }  
    } 

    BusesForStopResponse GetBusesForStop(const string& stop) const {
        BusesForStopResponse r;
        r.stop = stop;
        if (stops_to_buses_.count(stop) > 0) {
            r.buses = stops_to_buses_.at(stop);
        }         
        return r;
    }

    StopsForBusResponse GetStopsForBus(const string& bus) const {
        StopsForBusResponse r;
        r.bus = bus;
        r.stops_to_buses = stops_to_buses_;
        if (buses_to_stops_.count(bus) > 0) {
            r.stops = buses_to_stops_.at(bus);
        }        
        return r;
    }

    AllBusesResponse GetAllBuses() const {
        AllBusesResponse r; 
        r.buses_to_stops = buses_to_stops_;
        r.stops_to_buses = stops_to_buses_;
        return r;
    }
};

int main() {
    #ifndef FILES
        freopen("inp.txt", "r", stdin);
        freopen("outp.txt", "w", stdout);
    #endif

    int query_count;
    Query q;

    cin >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        cin >> q;
        switch (q.type) {
            case QueryType::NewBus:
                bm.AddBus(q.bus, q.stops);
                break;
            case QueryType::BusesForStop:
                cout << bm.GetBusesForStop(q.stop) << endl;
                break;
            case QueryType::StopsForBus:
                cout << bm.GetStopsForBus(q.bus) << endl;
                break;
            case QueryType::AllBuses:
                cout << bm.GetAllBuses() << endl;
                break;
        }
    }
}

}