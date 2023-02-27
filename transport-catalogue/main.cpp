#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"
#include "log_duration.h"

#include <fstream>
#include <iostream>

using namespace std::literals;

int main() {
    {
        std::fstream in("assert_files_curvature/tsC_case1_input.txt"s);
        std::ostream &out(std::cout);

        LOG_DURATION("Time");
        input_reader::reader<decltype(in)> queries(in);
        catalogue::transport_catalogue catalogue(queries.GetQueries());
        stat::stat_reader<decltype(in), decltype(out)> output(in, catalogue, out);
    }
    return 0;
}