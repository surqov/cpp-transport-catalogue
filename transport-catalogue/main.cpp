#include "input_reader.h"
#include "transport_catalogue.h"

#include <sstream>

int main() {
    std::vector<reader::Query> test = {
        reader::ParseToQuery(reader::SplitIntoWords("Bus iBAbvykyUAujN: 7qpKAGotrE6 vi5OEmh8Txf > T893Gm > 0y4ho9GnEYu21UmoV42HZB0Mv > 7qpKAGotrE6vi5OEmh8Txf")),
        reader::ParseToQuery(reader::SplitIntoWords("Bus pUFj82Cqn3: QdPEP6 - w7V0IyAT4IqNKF7fFFyBq4B - h1OTf - aaDrzCkRa")),
        "Stop Mzx: 38.569868, 34.79233"))
    };

    std::istringstream input{
    

    reader::SortByQueryType(test);
    std::cout << "ok";
    return 0;
}