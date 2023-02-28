#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"
#include "log_duration.h"

#include <fstream>
#include <iostream>

using namespace std::literals;

int main() {
    {
        std::istream &in(std::cin);
        std::ostream &out(std::cout);

        LOG_DURATION("Time");
        input_reader::reader<decltype(in)> queries(in);
        catalogue::transport_catalogue catalogue(queries.GetQueries());
        stat::stat_reader<decltype(in), decltype(out)> output(in, catalogue, out);
    }
    return 0;
}