#include "transport_catalogue.h"

#include <string>
#include <vector>
#include <iostream>

namespace reader {

enum class QueryType {
    NewBus,
    NewStop
};

struct Query {
    QueryType type;
    transport_data::Bus bus;
    transport_data::Stop stop;
};

std::vector<std::string> SplitIntoWords(std::istream& input);


    
}