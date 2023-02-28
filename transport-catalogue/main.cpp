#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"
#include "log_duration.h"
//#include "test_functions.h"

#include <fstream>
#include <iostream>

using namespace std::literals;

int main() {
    {
        //std::fstream in("assert_files_curvature/yandex.txt"s);

        std::istream &in(std::cin);
        std::ostream &out(std::cout);
        catalogue::transport_catalogue catalogue;

        LOG_DURATION("Time");
        input_reader::reader<decltype(in)> queries(in, catalogue);
        stat::stat_reader<decltype(out)> output(queries.GetRawOutQueries(), catalogue, out);
    }
    return 0;
}