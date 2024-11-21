#ifndef PROCESS_COMPLETABLE_FUTURES_HPP
#define PROCESS_COMPLETABLE_FUTURES_HPP

#include <future>
#include <vector>
#include <string>
#include <iostream>
#include "Places.hpp"
#include <windows.h>


std::wstring CompletableFuturesForOutputWeatherAndDescriptions(Places& places, int selectedIndex) {

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

#endif 