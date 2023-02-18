#include "log_duration.h"
#include "input_reader.h"
#include "transport_catalogue.h"

#include <sstream>
#include <fstream>

int main() {
    std::stringstream {
        "3\n"
        "Bus long: 97ChPsiaFJ xk - nhfM4\n"
        "Stop 97ChPsiaFJ xk: 38.590881, 34.724362\n"
        "Stop nhfM4: 38.653078, 34.910424\n"
    };
    {
        LOG_DURATION("Loaded queries for"s);
        reader::reader<std::ifstream> queries(file);
        auto l_4 = queries.GetQueries();
        std::cout << "";
    }
    return 0;
}