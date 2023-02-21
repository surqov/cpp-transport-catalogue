#include "input_reader.h"
#include "transport_catalogue.h"
#include "stat_reader.h"

#include <fstream>

using namespace std::literals;

int main() {
    std::fstream input("assert_files/tsA_case1_input.txt");

    reader<std::fstream> queries(input);
    bus_catalogue catalog(queries.GetQueries());
    stat<std::fstream> output(input);

    return 0;
}