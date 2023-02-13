#include "log_duration.h"
#include "input_reader.h"
#include "transport_catalogue.h"

#include <sstream>
#include <fstream>

int main() {
    std::ifstream file( "assert_files/tsA_case2_input.txt");
    reader::reader<std::ifstream> queries(file);

    return 0;
}