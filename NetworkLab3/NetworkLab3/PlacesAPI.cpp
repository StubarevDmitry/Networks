#include "PlacesAPI.hpp"
#include "Utils.hpp"
#include <algorithm>
#include <curl/curl.h>
#include "./include/json.hpp"
#include <codecvt>
#include <future>
#include <iostream>
#include <string>
#include <locale>
#include <iomanip>
#include <sstream>

PlacesAPI::PlacesAPI(const std::map<std::string, std::string>& apiKeys) : apiKeys(apiKeys) {}

std::string PlacesAPI::getWeather(int placeID) {
    std::string reqStr = "https://api.openweathermap.org/data/2.5/weather?lat="
        + std::to_string(coords[placeID].second)
        + "&lon=" + std::to_string(coords[placeID].first) + "&appid=" + apiKeys["weather"];
    json jsonObject = Utils::getJson(reqStr);

    int temp = jsonObject["main"]["temp"].get<int>() - 273;
    int feelsLike = jsonObject["main"]["feels_like"].get<int>() - 273;
    int maxTemp = jsonObject["main"]["temp_max"].get<int>() - 273;
    int minTemp = jsonObject["main"]["temp_min"].get<int>() - 273;
    std::string weather = jsonObject["weather"][0]["description"].get<std::string>();
    int speed = jsonObject["wind"]["speed"].get<int>();

    return "temperature: " + std::to_string(temp) + " degrees Celsius\n" +
        "feels Like: " + std::to_string(feelsLike) + " degrees Celsius\n" +
        "maximum temperature: " + std::to_string(maxTemp) + " degrees Celsius\n" +
        "minimum temperature: " + std::to_string(minTemp) + " degrees Celsius\n" +
        "weather: " + weather + "\n" +
        "wind speed: " + std::to_string(speed) + "\n";
}

std::vector<std::wstring> PlacesAPI::placesInArea(int placeID) {
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

    std::vector<std::future<std::wstring>> futures;

    for (int i = 0; i < places; i++) {
        json elementObj = jsonArray[i];
        std::string name = elementObj["properties"]["name"].get<std::string>();
        std::string xid = elementObj["properties"]["xid"].get<std::string>();

        futures.push_back(std::async(std::launch::async, [name, xid, this]() -> std::wstring {
            std::string description = placeDescription(xid);
            if (name.empty() || description.empty()) {
                return L"";
            }

            // Преобразование строк из UTF-8 в UTF-16 
            int nameLen = MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, NULL, 0);
            std::wstring wname(nameLen, 0);
            MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, &wname[0], nameLen);

            int descLen = MultiByteToWideChar(CP_UTF8, 0, description.c_str(), -1, NULL, 0);
            std::wstring wdescription(descLen, 0);
            MultiByteToWideChar(CP_UTF8, 0, description.c_str(), -1, &wdescription[0], descLen);

            return L"Название: " + wname + L"\n" + L"\n" +
                L"Описание: " + wdescription + L"\n" +
                L"---------------------------------------------------------------------------" + L"\n";
            }));
    }

    // соберает все асинхронные задачи (:
    for (auto& future : futures) {
        std::wstring result = future.get();
        if (!result.empty()) {
            placesAndDescription.push_back(result);
        }
    }

    return placesAndDescription;
}

std::string PlacesAPI::placeDescription(const std::string& xid) {
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

std::string replaceSpaces(const std::string& str) {
    std::string result;
    for (char ch : str) {
        if (ch == ' ') {
            result += "%20";
        }
        else {
            result += ch;
        }
    }
    return result;
}

std::string to_utf8(const std::wstring& str)
{
    static std::wstring_convert<std::codecvt_utf8<wchar_t>> u8;
    return u8.to_bytes(str);
}

std::vector<std::wstring> PlacesAPI::getPlaces(const std::wstring& place) {

    std::string urlPlaceName = to_utf8(place);

    std::string placeWithoutSpace = replaceSpaces(urlPlaceName);

    std::string url = "https://graphhopper.com/api/1/geocode?q=" + placeWithoutSpace
        + "&locale=ru&limit=100&key=" + apiKeys["location"];

    std::vector<std::wstring> places;
    
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
            variotive += L", Субъект: " + wstate;
        }

        if (osmValue != "") {
            int osmValueLen = MultiByteToWideChar(CP_UTF8, 0, osmValue.c_str(), -1, NULL, 0);
            std::wstring wosmValue(osmValueLen, 0);
            MultiByteToWideChar(CP_UTF8, 0, osmValue.c_str(), -1, &wosmValue[0], osmValueLen);
            variotive += L", Тип объекта: " + wosmValue;
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

        places.push_back(std::to_wstring(count) + L" ) Страна: " + wcountry 
            + L", Название: " + wname + variotive);
        coords[count] = { lng, lat };
        count++;
    }
    return places;
}