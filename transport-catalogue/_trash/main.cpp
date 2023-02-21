#include "log_duration.h"
#include "input_reader.h"
#include "transport_catalogue.h"
#include "stat_reader.h"

#include <sstream>
#include <fstream>

int main() {
    {
        std::fstream input("assert_files/tsA_case2_input.txt");
        LOG_DURATION("StringStream "s);
        reader<std::fstream> queries(input);
        bus_catalogue catalog(queries.GetQueries());
        stat<std::fstream> output(input, catalog);
    }

    return 0;
}