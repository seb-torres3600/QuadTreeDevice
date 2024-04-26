#include "../include/data.hpp"


Data* Data :: data_set = NULL;

Data :: Data(){
};

Data* Data :: getInstance(){
    if (data_set == NULL){
        data_set = new Data();
    }
    return data_set;
}

void Data :: setData(string _file){
    file = _file;
    std::ifstream f(file);
    data = json::parse(f);

}

json Data :: getData(){
    return data;
}

void Data :: printData(){
    std:: cout << data.dump() << std:: endl;
}

void Data :: updateData(){
    std :: ifstream f(file);
    data = json::parse(f);
}

void Data :: writeData(string shape, vector<double> coord){
    json tmp;
    tmp["id"] = to_string(data.size()+1);
    tmp["shape"] = shape;
    tmp["coordinates"] = coord;
    data.push_back(tmp);
    std::ofstream outputFile(file);

    if (outputFile.is_open()){
        outputFile << data.dump(4);
        outputFile.close();
    }
}