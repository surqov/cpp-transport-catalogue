#include "input_reader.h"
#include "transport_catalogue.h"
#include <sstream>

int main() {
    std::istringstream input{
    "3\n"    
    "line1\n"
    "line2\n"
    "line3\n"
    };
    reader::reader<std::istringstream> istream_(input);
    transport::catalogue(istream);
    stat::report<std::istringstream, std::ostringstream> ostream(input);

}