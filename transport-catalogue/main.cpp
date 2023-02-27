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
        std::ostream out(NULL);
        std::fstream in("assert_files_curvature/tsC_case1_input.txt"s);
        input_reader::reader<std::fstream> queries(in);
        catalogue::transport_catalogue catalogue(queries.GetQueries());
        stat::stat_reader<std::fstream, std::ostream> output(in, catalogue, out);
    }
    return 0;
}