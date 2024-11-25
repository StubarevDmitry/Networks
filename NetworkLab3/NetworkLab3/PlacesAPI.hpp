#ifndef PLACES_HPP
#define PLACES_HPP

#include <string>
#include <map>
#include <vector>
#include <Windows.h>
#include "./include/json.hpp"


using json = nlohmann::json;

class PlacesAPI {
private:
    static const int MAX_PLACES_COUNT = 20;
    std::map<std::string, std::string> apiKeys;
    std::map<int, std::pair<double, double>> coords;

public:
    PlacesAPI(const std::map<std::string, std::string>& apiKeys);

    std::string getWeather(int placeID);

    std::vector<std::wstring> placesInArea(int placeID);

    std::string placeDescription(const std::string& xid);
    
    std::vector<std::wstring> getPlaces(const std::wstring& place);
};

#endif