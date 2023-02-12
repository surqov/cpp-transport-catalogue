#include "input_reader.h"
#include "transport_catalogue.h"

#include <sstream>

int main() {
    std::istringstream input{
        "6\n"
        "Bus iBAbvykyUAujN: 7qpKAGotrE6 vi5OEmh8Txf > T893Gm > 0y4ho9GnEYu21UmoV42HZB0Mv > 7qpKAGotrE6vi5OEmh8Txf\n"
        "Bus pUFj82Cqn3: QdPEP6 - w7V0IyAT4IqNKF7fFFyBq4B - h1OTf - aaDrzCkRa\n"
        "Stop Mzx: 38.569868, 34.79233\n"
        "Stop f4mV: 38.492512, 34.665361\n"
        "Stop m: 38.559248, 34.908154\n"
        "Stop HL: 38.568337, 34.759169\n"
    };
    reader::reader<std::istringstream> queries(input);
    std::cout << "ok";
    return 0;
}