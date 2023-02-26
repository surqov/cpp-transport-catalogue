#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"
#include "log_duration.h"

#include <fstream>
#include <sstream>
#include <iostream>

using namespace std::literals;

int main() {
    {
        //LOG_DURATION("Time");
        std::fstream input("assert_files_curvature/tsC_case1_input.txt"s);
        input_reader::reader<std::fstream> queries(input);
        catalogue::transport_catalogue catalogue(queries.GetQueries());
        stat::stat_reader<std::fstream> output(input, catalogue);
    }
    return 0;
}