#ifndef DATA_HPP
#define DATA_HPP

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

class Data{
    private:
        Data();
        static Data* data_set;
        json data;
        string file;
    public:
        static Data* getInstance();
        void setData(string _file);
        json getData();
        void printData();
        void writeData(string shape, vector<double> coord);
        void updateData();
};

#endif