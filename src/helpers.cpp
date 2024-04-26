#include "../include/helpers.hpp"

std::string toUpperCase(const std::string& str) {
    std::string upperStr;
    std::locale loc;
    for (char c : str) {
        upperStr += std::toupper(c, loc);
    }
    return upperStr;
}
