#include "../include/data.hpp"


Data* Data :: data_set = NULL;

Data :: Data(){

}

Data :: ~Data(){
    delete data_set;
}

Data* Data :: getInstance(){
    if (data_set == NULL){
        data_set = new Data();
    }
    return data_set;
}

void Data :: setData(std:: string _file){
    file = _file;
    std::ifstream f(file);
    data = nlohmann::json::parse(f);
}

nlohmann::json Data :: getData(){
    return data;
}

void Data :: printData(){
    std:: cout << data.dump() << std:: endl;
}

void Data :: updateData(){
    std :: ifstream f(file);
    data = nlohmann::json::parse(f);
}

void Data :: writeData(std::string shape, std::vector<double> coord){
    nlohmann::json tmp;
    tmp["id"] = std::to_string(data.size()+1);
    tmp["shape"] = shape;
    tmp["coordinates"] = coord;
    data.push_back(tmp);
    std::ofstream outputFile(file);

    if (outputFile.is_open()){
        outputFile << data.dump(4);
        outputFile.close();
    }
}