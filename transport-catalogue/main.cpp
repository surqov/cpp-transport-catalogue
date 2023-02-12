#include "log_duration.h"
#include "input_reader.h"
#include "transport_catalogue.h"

#include <sstream>
#include <fstream>

int main() {
    std::ifstream file( "assert_files/tsA_case1_input.txt");
    {
        LOG_DURATION("Loaded queries for: "s);
        reader::reader<std::ifstream> queries(file);
        std::cout << "ok";
    }
    return 0;
}