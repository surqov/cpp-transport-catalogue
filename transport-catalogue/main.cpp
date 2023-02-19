#include "log_duration.h"
#include "input_reader.h"
#include "transport_catalogue.h"

#include <sstream>
#include <fstream>

int main() {
    std::stringstream input{
    "5\n"
    "Bus long: l3A - kv8A5AKGj iN - RCMbeebIuxiDV - 3\n"
    "Stop l3A: 38.460784, 34.937663\n"
    "Stop kv8A5AKGj iN: 38.716203, 34.779089\n"
    "Stop RCMbeebIuxiDV: 38.700196, 34.821645\n"
    "Stop 3: 38.708395, 34.867487\n"
    };
    {
        LOG_DURATION("StringStream "s);
        reader::reader<std::stringstream> queries(input);
        auto l_4 = queries.GetQueries();
        std::cout << "";
    }

    std::fstream input2("assert_files/tsA_case1_input.txt");
    {
        LOG_DURATION("FileStream "s);
        reader::reader<std::fstream> queries(input2);
        auto l_4 = queries.GetQueries();
        std::cout << "";
    }

    return 0;
}