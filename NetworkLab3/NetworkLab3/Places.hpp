#ifndef PLACES_HPP
#define PLACES_HPP

#include <string>
#include <map>
#include <vector>
#include <Windows.h>
#include "./include/json.hpp"

using json = nlohmann::json;

class Places {
private:
    static const int MAX_PLACES_COUNT = 20;
    std::map<std::string, std::string> apiKeys;
    std::map<int, std::pair<double, double>> coords;

    std::string placeDescription(const std::string& xid);

public:
    Places(const std::map<std::string, std::string>& apiKeys);

    std::string getWeather(int placeID);
    std::vector<std::wstring> placesInArea(int placeID);
    std::vector<std::wstring> getPlaces(const std::string& place);
};

#endif