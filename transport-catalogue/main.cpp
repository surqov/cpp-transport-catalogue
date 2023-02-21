#include "input_reader.h"
#include "transport_catalogue.h"
#include "stat_reader.h"
#include "log_duration.h"

#include <fstream>

using namespace std::literals;

int main() {
    {
        //std::fstream input("assert_files/test_from_yandex.txt");
        //LOG_DURATION("File stream");
        //std::fstream input("assert_files/tsA_case2_input.txt");
        std::string line;
        std::stringstream input;
        while (std::getline(std::cin, line)) {
            input << line << "\n";
        }

        reader<std::stringstream> queries(input);
        bus_catalogue catalog(queries.GetQueries());
        stat<std::stringstream> output(input, catalog);
    }
    return 0;
}