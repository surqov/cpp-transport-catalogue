#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"
#include "log_duration.h"
#include "test_functions.h"

#include <fstream>
#include <iostream>

using namespace std::literals;

int main() {
    TestTransportCatalogue();
    {
        std::fstream in("assert_files_curvature/yandex.txt"s);

        //std::istream &in(std::cin);
        std::ostream &out(std::cout);
        catalogue::transport_catalogue catalogue;

        LOG_DURATION("Time");
        input_reader::reader<decltype(in)> queries(in, catalogue);
        stat::stat_reader<decltype(out)> output(queries.GetRawOutQueries(), catalogue, out);

        ////завернуть все поля класса catalogue в приват и добавить безопасные геттеры сеттеры !!!!
        ///добавить функцию Load - единая точка входа для считывания запросов
        //сделать конструктор для stat не по string а по reader
    }
    return 0;
}