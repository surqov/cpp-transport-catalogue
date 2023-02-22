#include "input_reader.h"
#include "transport_catalogue.h"
#include "stat_reader.h"
#include "log_duration.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

using namespace std::literals;

int main() {
    {
        /*std::stringstream input;
        std::string line;
        int num = 0;
        std::getline(std::cin, line);
        input << line << "\n";
        num = std::stoi(line);
        while (num-- > 0) {
            std::getline(std::cin, line);
            input << line << "\n";
        }
        num = 0;
        std::getline(std::cin, line);
        input << line << "\n";
        num = std::stoi(line);
        while (num-- > 0) {
            std::getline(std::cin, line);
            input << line << "\n";
        }*/
        LOG_DURATION("READ");
        std::fstream input("assert_files_stop/tsB_case2_input.txt"s);
        reader<std::fstream> queries(input);
        bus_catalogue catalog(queries.GetQueries());
        stat<std::fstream> output(input, catalog);
    }
    return 0;
}