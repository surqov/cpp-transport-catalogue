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
        LOG_DURATION("Time");
        std::fstream in("assert_files_curvature/tsC_case1_input.txt"s);
        input_reader::reader queries = input_reader::LoadFromStream<std::fstream>(in);
        catalogue::transport_catalogue catalogue(queries.GetQueries());
        stat::stat_reader<std::fstream, std::fstream> output(in, catalogue, out);
    }
    return 0;
}