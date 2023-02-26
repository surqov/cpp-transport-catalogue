#include "transport_catalogue.h"


size_t catalogue::StopPairHasher::operator()(const std::pair<Stop*, Stop*>& stop) const {
    const double lat1 = stop.first->coordinates.lat;
    const double lng1 = stop.first->coordinates.lat;
    const double lat2 = stop.second->coordinates.lat;
    const double lng2 = stop.second->coordinates.lat;

    return lat1 * 37 + lng1 * 37 * 37 + lat2 * 37 * 37 * 37 + lng2 * 37 * 37 * 37 * 37;
}

catalogue::transport_catalogue::transport_catalogue(std::vector<Query>& queries) {
    for (Query& query_ : queries) {
        if (query_.type == QueryType::NewStop) {
            std::string_view name_ = query_.stop.name;
            stops.push_back(std::move(query_.stop));
            stopname_to_stop[name_] = &stops.back();
        } else {
            std::string_view name_ = query_.bus.name;
            buses.push_back(std::move(query_.bus));
            busname_to_bus[name_] = &buses.back();
            std::for_each(busname_to_bus[name_]->stops.begin(),
                busname_to_bus[name_]->stops.end(),
                [this, name_](const auto& stop_) {
                    stops_to_bus[stop_->name].insert(busname_to_bus.at(name_));
                    stopname_to_busname[stop_->name].insert(busname_to_bus.at(name_)->name);
                });
        }
    }
    for (const auto& stop : stops) {
        std::string_view name_ = stop.name;
        std::for_each(stop.distances.begin(),
                    stop.distances.end(),
                    [this, name_](const auto& pair_) {
                        distances.insert({{
                                stopname_to_stop[name_], stopname_to_stop[pair_.first]}, 
                                pair_.second});
                        });
    }
}

catalogue::BusInfo catalogue::transport_catalogue::GetBusInfo(const std::string_view& bus_name) const {
    BusInfo info_;
    info_.busname = bus_name;
    info_.founded = busname_to_bus.find(bus_name) != busname_to_bus.end();
    if (info_.founded) {
        info_.stops_on_route = busname_to_bus.at(bus_name)->stops.size();
        info_.unique_stops = std::set(busname_to_bus.at(bus_name)->stops.begin(), busname_to_bus.at(bus_name)->stops.end()).size();
        double route_len = std::transform_reduce(
            busname_to_bus.at(bus_name)->stops.begin(),
            busname_to_bus.at(bus_name)->stops.end() - 1,
            busname_to_bus.at(bus_name)->stops.begin() + 1,
            0.0,
            std::plus<double>(),
            [](const auto& lhs, const auto& rhs) {
                return geo::ComputeDistance(lhs->coordinates, rhs->coordinates);
            }
        );

        double fact_len = std::transform_reduce(
            busname_to_bus.at(bus_name)->stops.begin(),
            busname_to_bus.at(bus_name)->stops.end() - 1,
            busname_to_bus.at(bus_name)->stops.begin() + 1,
            0.0,
            std::plus<double>(),
            [this](const auto& lhs, const auto& rhs) {
                Stop* lhs_stop = stopname_to_stop.at(lhs->name);
                Stop* rhs_stop = stopname_to_stop.at(rhs->name);
                return distances.find({lhs_stop, rhs_stop}) != distances.end() ? 
                        distances.at({lhs_stop, rhs_stop}) : 
                        distances.at({rhs_stop, lhs_stop});
            }
        );
        info_.curvature = fact_len / route_len;
        info_.route_len = fact_len;
        }
    return info_;
}

catalogue::StopInfo catalogue::transport_catalogue::GetStopInfo(const std::string_view& stop_name) const {
    StopInfo info_;
    info_.stopname = stop_name;
    info_.founded = stopname_to_stop.find(stop_name) != stopname_to_stop.end();
    if (info_.founded && stopname_to_busname.find(stop_name) != stopname_to_busname.end()) {
        
        info_.buses_to_stop = stopname_to_busname.at(stop_name);
    }
    return info_;
}