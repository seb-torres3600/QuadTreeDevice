#ifndef DATA_HPP
#define DATA_HPP

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

#include "logger.hpp"

class Data{
    private:
        Data();
        ~Data();
        static Data* data_set;
        nlohmann::json data;
        std::string file;
        std::shared_ptr<spdlog::logger> logger = Logger::getLogger();

    public:
        static Data* getInstance();
        void setData(std::string _file);
        nlohmann::json getData();
        void printData();
        void writeData(std::string shape, std::vector<double> coord);
        void updateData();
};

#endif