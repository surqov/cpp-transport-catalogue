#include "transport_catalogue.h"


size_t catalogue::StopPairHasher::operator()(const std::pair<Stop*, Stop*>& stop) const {
    const double lat1 = stop.first->coordinates.lat;
    const double lng1 = stop.first->coordinates.lat;
    const double lat2 = stop.second->coordinates.lat;
    const double lng2 = stop.second->coordinates.lat;

    return lat1 * 37 + lng1 * 37 * 37 + lat2 * 37 * 37 * 37 + lng2 * 37 * 37 * 37 * 37;
}

size_t catalogue::BusHasher::operator()(const catalogue::Bus* bus) const {
    return std::hash<std::string_view>{}(bus->name);
}

void catalogue::transport_catalogue::AddBus(Bus&& bus){
    std::string_view name_ = bus.name;
    buses.push_back(std::move(bus));
    busname_to_bus[name_] = &buses.back();
    std::for_each(busname_to_bus[name_]->stops.begin(),
                    busname_to_bus[name_]->stops.end(),
                    [this, name_](const auto& stop_) {
                    stop_to_buses[stop_].insert(busname_to_bus.at(name_));
                    stopname_to_busesnames[stop_->name].insert(busname_to_bus.at(name_)->name);
                    });
}

void catalogue::transport_catalogue::AddStop(Stop&& stop) {
    std::string_view name_ = stop.name;
    stops.push_back(std::move(stop));
    stopname_to_stop[name_] = &stops.back();
}

bool catalogue::transport_catalogue::FindBus(const std::string_view& bus_name) const noexcept {
    try {
        return busname_to_bus.find(bus_name) != busname_to_bus.end();
    } catch (...) {
        return false;
    }
}

bool catalogue::transport_catalogue::FindStop(const std::string_view& stop_name) const noexcept {
    try {
        return stopname_to_stop.find(stop_name) != stopname_to_stop.end();
    } catch (...) {
        return false;
    }
}

catalogue::BusInfo catalogue::transport_catalogue::GetBusInfo(const std::string_view& bus_name) const {
    BusInfo info_;
    info_.busname = bus_name;
    info_.found = busname_to_bus.find(bus_name) != busname_to_bus.end();
    if (info_.found) {
        info_.stops_on_route = busname_to_bus.at(bus_name)->stops.size();
        info_.unique_stops = std::set(busname_to_bus.at(bus_name)->stops.begin(), busname_to_bus.at(bus_name)->stops.end()).size();
        const double fact_len = bus_routes_fact.at(busname_to_bus.at(bus_name));
        const double geo_len = bus_routes_geo.at(busname_to_bus.at(bus_name));
        info_.curvature = fact_len / geo_len;
        info_.route_len = fact_len;
        }
    else {
        throw std::out_of_range("not found"s);
    }
    return info_;
}

catalogue::StopInfo catalogue::transport_catalogue::GetStopInfo(const std::string_view& stop_name) const {
    StopInfo info_;
    info_.stopname = stop_name;
    info_.found = stopname_to_stop.find(stop_name) != stopname_to_stop.end();
    if (info_.found && stopname_to_busesnames.find(stop_name) != stopname_to_busesnames.end()) {
        info_.buses_to_stop = stopname_to_busesnames.at(stop_name);
    } else {
        throw std::out_of_range("not found"s);
    }
    return info_;
}