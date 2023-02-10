#include "geo.h"

#include <deque>
#include <string>
#include <unordered_map>
#include <string_view>
#include <iomanip>

const double ACCURACY = 1e-6;
const char precision = 6;

namespace transport { 

struct Stop {
    std::string name;
    geo::Coordinates coordinates; 
};

struct Bus {
    std::string name;
};

//функция хеширования до size_t для distances из долготы и широты домножением на единый множитель
struct StopHash {
    size_t operator()(const std::pair<Stop*, Stop*> stop){
        const double lat1 = stop.first->coordinates.lat;
        const double lng1 = stop.first->coordinates.lat;
        const double lat2 = stop.second->coordinates.lat;
        const double lng2 = stop.second->coordinates.lat;
        return lat1 * 37 + lng1 * 37 * 37 + lat2 * 37 * 37 * 37 + lng2 * 37 * 37 * 37 * 37;
    }
};

class catalogue {
  private:
    std::deque<Stop> stops;
    std::deque<Bus> buses;
    std::unordered_map<std::string_view, Stop*> stopname_to_stop;
    std::unordered_map<std::string_view, Bus*> busname_to_bus;
    std::unordered_map<std::pair<Stop*, Stop*>, int, StopHash> distances;
    
  public:
    catalogue();
    void AddStop();
    void FindStop() const;
    void AddBus();
    void FindBus() const;
    void GetBusInfo() const;
};

}