#include "Places.hpp"
#include "Utils.hpp"
#include <algorithm>
#include <codecvt>

Places::Places(const std::map<std::string, std::string>& apiKeys) : apiKeys(apiKeys) {}

std::string Places::getWeather(int placeID) {
    std::string reqStr = "https://api.openweathermap.org/data/2.5/weather?lat=" + std::to_string(coords[placeID].second)
        + "&lon=" + std::to_string(coords[placeID].first) + "&appid=" + apiKeys["weather"];
    json jsonObject = Utils::getJson(reqStr);

    int temp = jsonObject["main"]["temp"].get<int>() - 273;
    int feelsLike = jsonObject["main"]["feels_like"].get<int>() - 273;
    int maxTemp = jsonObject["main"]["temp_max"].get<int>() - 273;
    int minTemp = jsonObject["main"]["temp_min"].get<int>() - 273;

    return "temp: " + std::to_string(temp) + "C\n" +
        "feelsLike: " + std::to_string(feelsLike) + "C\n" +
        "maxTemp: " + std::to_string(maxTemp) + "C\n" +
        "minTemp: " + std::to_string(minTemp) + "C\n";
}

std::vector<std::wstring> Places::placesInArea(int placeID) {
    std::vector<std::wstring> placesAndDescription;
    std::string api_key = apiKeys["placesArea"];

    std::string lat_min = std::to_string(coords[placeID].second);
    std::replace(lat_min.begin(), lat_min.end(), ',', '.');

    std::string lat_max = std::to_string(coords[placeID].second + 0.01);
    std::replace(lat_max.begin(), lat_max.end(), ',', '.');

    std::string lon_min = std::to_string(coords[placeID].first);
    std::replace(lon_min.begin(), lon_min.end(), ',', '.');

    std::string lon_max = std::to_string(coords[placeID].first + 0.01);
    std::replace(lon_max.begin(), lon_max.end(), ',', '.');

    std::string url = "http://api.opentripmap.com/0.1/ru/places/bbox?lon_min=" + lon_min +
        "&lat_min=" + lat_min + "&lon_max=" + lon_max +
        "&lat_max=" + lat_max + "&kinds=interesting_places&format=geojson&apikey=" + api_key;

    json jsonObject = Utils::getJson(url);
    json jsonArray = jsonObject["features"];
    int places = std::min<size_t>(jsonArray.size(), MAX_PLACES_COUNT);
    for (int i = 0; i < places; i++) {
        json elementObj = jsonArray[i];
        std::string name = elementObj["properties"]["name"].get<std::string>();
        std::string xid = elementObj["properties"]["xid"].get<std::string>();
        std::string description = placeDescription(xid);
        if (name.empty() || description.empty()) {
            continue;
        }

        // Преобразование строк из UTF-8 в UTF-16
        int nameLen = MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, NULL, 0);
        std::wstring wname(nameLen, 0);
        MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, &wname[0], nameLen);

        int descLen = MultiByteToWideChar(CP_UTF8, 0, description.c_str(), -1, NULL, 0);
        std::wstring wdescription(descLen, 0);
        MultiByteToWideChar(CP_UTF8, 0, description.c_str(), -1, &wdescription[0], descLen);

        placesAndDescription.push_back(L"Название: " + wname + L"\n" + L"\n" +
            L"Описание: " + wdescription + L"\n" +
            L"---------------------------------------------------------------------------" + L"\n");
    }
    return placesAndDescription;
}

std::string Places::placeDescription(const std::string& xid) {
    std::string api_key = apiKeys["placesArea"];
    std::string url = "http://api.opentripmap.com/0.1/ru/places/xid/" + xid + "?apikey=" + api_key;
    json jsonObject = Utils::getJson(url);
    std::string description;
    try {
        description = jsonObject["wikipedia_extracts"]["text"].get<std::string>();
    }
    catch (json::exception& e) {
        description = "";
    }
    return description;
}

std::vector<std::wstring> Places::getPlaces(const std::string& place) {
    std::vector<std::wstring> places;
    std::string url = "https://graphhopper.com/api/1/geocode?q=" + place + "&locale=ru&limit=100&key=" + apiKeys["location"];

    json jsonObject = Utils::getJson(url);
    json jsonArray = jsonObject["hits"];
    int count = 1;
    for (const auto& element : jsonArray) {
        std::string country = element["country"].get<std::string>();
        std::string city = element.value("city", "");
        std::string state = element.value("state", "");
        std::string osmValue = element.value("osm_value", "");
        std::string street = element.value("street", "");
        std::string name = element["name"].get<std::string>();
        double lng = element["point"]["lng"].get<double>();
        double lat = element["point"]["lat"].get<double>();

        std::wstring variotive;

        if (city != "") {
            int cityLen = MultiByteToWideChar(CP_UTF8, 0, city.c_str(), -1, NULL, 0);
            std::wstring wcity(cityLen, 0);
            MultiByteToWideChar(CP_UTF8, 0, city.c_str(), -1, &wcity[0], cityLen);
            variotive += L", Город: " + wcity;
        }

        if (state != "") {
            int stateLen = MultiByteToWideChar(CP_UTF8, 0, state.c_str(), -1, NULL, 0);
            std::wstring wstate(stateLen, 0);
            MultiByteToWideChar(CP_UTF8, 0, state.c_str(), -1, &wstate[0], stateLen);
            variotive += L", Штат: " + wstate;
        }

        if (osmValue != "") {
            int osmValueLen = MultiByteToWideChar(CP_UTF8, 0, osmValue.c_str(), -1, NULL, 0);
            std::wstring wosmValue(osmValueLen, 0);
            MultiByteToWideChar(CP_UTF8, 0, osmValue.c_str(), -1, &wosmValue[0], osmValueLen);
            variotive += L", OSM Value: " + wosmValue;
        }

        if (street != "") {
            int streetLen = MultiByteToWideChar(CP_UTF8, 0, street.c_str(), -1, NULL, 0);
            std::wstring wstreet(streetLen, 0);
            MultiByteToWideChar(CP_UTF8, 0, street.c_str(), -1, &wstreet[0], streetLen);
            variotive += L", Улица: " + wstreet;
        }

        // Преобразование строк из UTF-8 в UTF-16
        int countryLen = MultiByteToWideChar(CP_UTF8, 0, country.c_str(), -1, NULL, 0);
        std::wstring wcountry(countryLen, 0);
        MultiByteToWideChar(CP_UTF8, 0, country.c_str(), -1, &wcountry[0], countryLen);

        int nameLen = MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, NULL, 0);
        std::wstring wname(nameLen, 0);
        MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, &wname[0], nameLen);

        places.push_back(std::to_wstring(count) + L") Страна: " + wcountry + L", Название: " + wname + variotive);
        coords[count] = { lng, lat };
        count++;
    }
    return places;
}