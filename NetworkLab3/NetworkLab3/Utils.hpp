#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include "./include/json.hpp"

using json = nlohmann::json;

class Utils {
public:
    static json getJson(const std::string& url);
};

#endif