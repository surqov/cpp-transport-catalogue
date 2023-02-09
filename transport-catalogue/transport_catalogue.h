#include "geo.h"

#include <deque>
#include <string>
#include <unordered_map>
#include <string_view>

namespace transport {

struct Stop {
    std::string name;
    geo::Coordinates coordinates; 
};

struct Bus {
    std::string name;
    std::vector<Stop*> route;
};

//функция хеширования до size_t для distances из долготы и широты домножением на единый множитель

class catalogue {
  private:
    std::deque<Stop> stops;
    std::deque<Bus> buses;
    std::unordered_map<std::string_view, Stop*> stopname_to_stop;
    std::unordered_map<std::string_view, Bus*> busname_to_bus;
    std::unordered_map<std::pair<Stop*, Stop*>, int64_t, !!HASHER!!> distances;

  public:
    void AddStop();
    void FindStop();
    void AddBus();
    void FindBus();
    void GetBusInfo();
};

}