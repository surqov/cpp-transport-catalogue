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
    //TestTransportCatalogue();
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

    {// читаем json - выводим json
        std::fstream in_("input.json"s);
        std::ostream& out_(std::cout);
        catalogue::transport_catalogue catalogue;

        std::istream& in__ = static_cast<std::istream&>(in_);
        json::Document doc_ = json::Load(in__);
        json_reader::reader queries(doc_, catalogue);
        json_reader::JsonInfoPrint(queries.GetRawOutQueries(), catalogue, out_);
    }

    return 0;
}