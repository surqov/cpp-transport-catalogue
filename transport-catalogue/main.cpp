#include "input_reader.h"
#include "transport_catalogue.h"
#include "stat_reader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

using namespace std::literals;

int main() {
    {
        std::stringstream input;
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
        }
        
        reader<std::stringstream> queries(input);
        bus_catalogue catalog(queries.GetQueries());
        stat<std::stringstream> output(input, catalog);
    }
    return 0;
}