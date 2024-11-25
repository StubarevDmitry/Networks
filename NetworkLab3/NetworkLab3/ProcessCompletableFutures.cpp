
#include "PlacesAPI.hpp"
#include "ProcessCompletableFutures.h"
#include <iostream>

std::wstring CompletableFuturesForOutputWeatherAndDescriptions(PlacesAPI& places, int selectedIndex) {

    

    std::future<std::vector<std::wstring>> placesAndDescriptionEvent = std::async(std::launch::async, [&places, selectedIndex]() {
        return places.placesInArea(selectedIndex);
        });

    std::future<std::string> weatherEvent = std::async(std::launch::async, [&places, selectedIndex]() {
        return places.getWeather(selectedIndex);
        });

    // Объединяем результаты двух CompletableFuture
    auto combinedFuture = std::async(std::launch::async, [](std::future<std::string>&& weatherFuture, std::future<std::vector<std::wstring>>&& placesAndDescriptionFuture) {
        std::string weather = weatherFuture.get();

        std::vector<std::wstring> placesAndDescription = placesAndDescriptionFuture.get();

        int weatherLen = MultiByteToWideChar(CP_UTF8, 0, weather.c_str(), -1, NULL, 0);
        std::wstring wweather(weatherLen, 0);
        MultiByteToWideChar(CP_UTF8, 0, weather.c_str(), -1, &wweather[0], weatherLen);

        std::wstring full = L"Погода: \n\n" + wweather + L"\n\n Интересные места: \n\n";

        for (const auto& place : placesAndDescription) {
            full = full + place + L"\n";
        }

        return full;
        }, std::move(weatherEvent), std::move(placesAndDescriptionEvent));

    return combinedFuture.get();
}

void processElement(const json& elementObj, std::vector<std::wstring>& placesAndDescription, PlacesAPI& places) {
    std::string name = elementObj["properties"]["name"].get<std::string>();
    std::string xid = elementObj["properties"]["xid"].get<std::string>();
    std::string description = places.placeDescription(xid);
    
    if (name.empty() || description.empty()) {
        return;
    }

    // Преобразование строк из UTF-8 в UTF-16
    int nameLen = MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, NULL, 0);
    std::wstring wname(nameLen, 0);
    MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, &wname[0], nameLen);

    int descLen = MultiByteToWideChar(CP_UTF8, 0, description.c_str(), -1, NULL, 0);
    std::wstring wdescription(descLen, 0);
    MultiByteToWideChar(CP_UTF8, 0, description.c_str(), -1, &wdescription[0], descLen);

    std::wstring result = L"Название: " + wname + L"\n" + L"\n" +
        L"Описание: " + wdescription + L"\n" +
        L"---------------------------------------------------------------------------" + L"\n";

    placesAndDescription.push_back(result);
}