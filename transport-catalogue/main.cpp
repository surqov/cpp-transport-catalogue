#include "transport_catalogue.h"
#include "json_reader.h"
#include "json.h"
#include "input_reader.h"
#include "stat_reader.h"
#include "log_duration.h"
#include "test_functions.h"

#include <fstream>
#include <iostream>

using namespace std::literals;

int main() {
    TestTransportCatalogue();
    /*{ //читаем stat 
        std::fstream in("_tr/assert_files_curvature/yandex.txt"s);

        //std::istream &in(std::cin);
        std::ostream &out(std::cout);
        catalogue::transport_catalogue catalogue;

        LOG_DURATION("Time");
        input_reader::reader<decltype(in)> queries(in, catalogue);
        stat::stat_reader<decltype(out)> output(queries.GetRawOutQueries(), catalogue, out);

        ////завернуть все поля класса catalogue в приват и добавить безопасные геттеры сеттеры !!!!
        ///добавить функцию Load - единая точка входа для считывания запросов
        //сделать конструктор для stat не по string а по reader
    }*/

    {// читаем json
        std::istream& in_(std::cin);
        std::ostream& out_(std::cout);
        catalogue::transport_catalogue catalogue;

        json::Document doc_ = json::Load(in_);
        json_reader::reader queries(doc_, catalogue);
        stat::stat_reader<std::ostream&> output(queries.GetRawOutQueries(), catalogue, out_);
    }
    return 0;
}